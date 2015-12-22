/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/21/14 18:00:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxterm.h"
#include "jnxunixsocket.h"
#include <assert.h>
#include <pthread.h>
#include <string.h>

// Globals
static counter = 0;

// Helpers
int set_up_stderror_redirect(int fds[2]) {
	int stderr_copy = dup(STDERR_FILENO);
	dup2(fds[1], STDERR_FILENO);
	close(fds[0]);
	return stderr_copy;
}
void restore_stderr(int old_stderr_fd) {
	dup2(old_stderr_fd, STDERR_FILENO);
}
void error_reader(int fds[2], char *match, size_t match_length) {
	close(fds[1]);
	char buffy[1024];
	memset(buffy, 0, 1024);
	read(fds[0], buffy, 1024);
	//	jnx_term_printf_in_color(JNX_COL_YELLOW, "buffy=%s\n", buffy);
	assert(strncmp(buffy, match, match_length) == 0);
	close(fds[0]);
}
void verify_socket(jnx_unix_socket *s, char *path, ssize_t stype) {
	assert(s);
	struct stat statbuf;
	fstat(s->socket, &statbuf);
	assert(S_ISSOCK(statbuf.st_mode));
	assert((s->address).sun_family == AF_UNIX);
	assert(strncmp((s->address).sun_path, path, sizeof(path)) == 0);
	assert(s->stype == stype);
}
void run_error_test(void(*test)(), char *expected_error, int expected_err_size) {
	int fds[2] = {-1, -1};
	pipe(fds);
	pid_t child_pid;
	if ((child_pid = fork()) != -1) {
		if (child_pid == 0) {
			error_reader(fds, expected_error, expected_err_size);
			exit(0);
		}
		else {
			int stderr_copy = set_up_stderror_redirect(fds);
			test();
			int child_stat = 0;
			wait(child_stat);
			restore_stderr(stderr_copy);
		}
	}
}

// Tests
void test_create_and_destroy() {
	JNXLOG(LDEBUG,"Test creation of unix sockets");

	jnx_unix_socket *a = jnx_unix_stream_socket_create("stream_a");
	(a, "stream_a", SOCK_STREAM);
	jnx_unix_socket *b = jnx_unix_datagram_socket_create("stream_b");
	verify_socket(b, "stream_b", SOCK_DGRAM);

	jnx_socket_destroy(&a);
	assert(a == NULL);
	jnx_socket_destroy(&b);
	assert(b == NULL);

	JNXLOG(LDEBUG,"OK");
}
// stream_send negative scenarios
void stream_send_connect_fails() {
	jnx_unix_socket *s = jnx_unix_stream_socket_create("stream_sun");
	int retval = jnx_unix_stream_socket_send(s, "stream send", 12);
	assert(retval == 0);
	jnx_unix_socket_destroy(&s);
}
extern ssize_t write_to_stream_socket(jnx_unix_socket*, char*, ssize_t);
void stream_send_fails() {
	jnx_unix_socket *s = jnx_unix_stream_socket_create("stream_sun");
	int retval = write_to_stream_socket(s, "stream send", 12);
	assert(retval == 0);
	jnx_unix_socket_destroy(&s);
}
void test_negative_send_stream_scenarios() {
	JNXLOG(LDEBUG,"Test negative send stream scenarios");
	fflush(stdout);
	run_error_test(stream_send_connect_fails, "jnx unix stream socket connect:", 31);
	run_error_test(stream_send_fails, "jnx unix stream socket send:", 28);
	JNXLOG(LDEBUG,"OK");
}
// datagram_send negative scenarios
void datagram_send_fails() {
	jnx_unix_socket *s = jnx_unix_datagram_socket_create("datagram_sun");
	int retval = jnx_unix_datagram_socket_send(s, "datagram send", 13);
	assert(retval == 0);
	jnx_unix_socket_destroy(&s);
}
void test_negative_send_datagram_scenarios() {
	JNXLOG(LDEBUG,"Test negative send datagram scenarios");
	fflush(stdout);
	run_error_test(datagram_send_fails, "jnx unix datagram socket sendto:", 32);
	JNXLOG(LDEBUG,"OK");
}
// stream_listen negative scenarios
extern int bind_stream_socket(jnx_unix_socket*);
extern int listen_on_stream_socket(jnx_unix_socket*, ssize_t);
extern jnx_unix_socket *accept_stream_socket_connection(jnx_unix_socket*);
extern int read_stream_socket(jnx_unix_socket*,char**,int*len);
void stream_bind_fails() {
	jnx_unix_socket *s = jnx_unix_stream_socket_create("stream_sun");
	jnx_unix_socket *s2 = jnx_unix_stream_socket_create("stream_sun");

	int retval = bind_stream_socket(s);
	retval = bind_stream_socket(s2);
	assert(retval == -1);
	jnx_unix_socket_destroy(&s);
	jnx_unix_socket_destroy(&s2);
}
void stream_listen_fails() {
	jnx_unix_socket *s = jnx_unix_stream_socket_create("sun_stream"); 
	int retval = listen_on_stream_socket(s, 5);
	assert(retval == -1);
	jnx_socket_destroy(&s);
}
void stream_accept_fails() {
	jnx_unix_socket *s = jnx_unix_stream_socket_create("stream_sun");
	jnx_unix_socket *retval = accept_stream_socket_connection(s);
	assert(retval == NULL);
	jnx_unix_socket_destroy(&s);
}
void stream_read_fails() {
	jnx_unix_socket *s = jnx_unix_stream_socket_create("stream_sun");
	char *out;
	int len;
	int retval = read_stream_socket(s, &out, &len);
	assert(retval == -1);
	jnx_unix_socket_destroy(&s);
}
void test_negative_stream_listen_scenarios() {
	JNXLOG(LDEBUG,"Test negative stream listen and receive scenarios");
	fflush(stdout);
	run_error_test(stream_bind_fails, "jnx unix stream socket bind:", 28);
	run_error_test(stream_listen_fails, "jnx unix stream socket listen:", 30);
	run_error_test(stream_accept_fails, "jnx unix stream socket accept:", 30);
	run_error_test(stream_read_fails, "jnx unix stream socket read:", 28);
	JNXLOG(LDEBUG,"OK");
}
int stream_callback(char *out, size_t len, jnx_unix_socket *rs) {
	counter++;
	assert(strncmp(out, "Hello world from stream socket!", 31) == 0);
	jnx_term_printf_in_color(JNX_COL_WHITE, ".");
	fflush(stdout);
	if (counter < 5)
		return 0;
	else 
		return 5;
}
void test_stream_ipc_comms() {
	JNXLOG(LDEBUG,"Test unix stream socket inter-process communication.\n");
	fflush(stdout);
	remove("stream_sun");
	jnx_unix_socket *ss = jnx_unix_stream_socket_create("stream_sun");
	counter = 0;

	pid_t child_pid;
	if ((child_pid = fork()) != -1) {
		if (child_pid == 0) {
			int i;
			jnx_unix_socket *cs = NULL; 
			for(i = 0; i < 5; i++) {
				sleep(1);
				cs = jnx_unix_stream_socket_create("stream_sun");
				jnx_unix_stream_socket_send(cs, "Hello world from stream socket!", 31);
				jnx_unix_socket_destroy(&cs);
			}
			exit(0);
		}
		else {
			jnx_unix_stream_socket_listen(ss, 5, stream_callback);
			
			jnx_unix_socket_destroy(&ss);
		}
	}
	JNXLOG(LDEBUG,"OK");
}
int check_binary_stream(uint8_t *out, size_t len, jnx_unix_socket *rs) {
	uint8_t expected[] = {'a', 'b', 'c', '\0', '\n', '\t', 0x01, 0xff, 0x13, '\f', '\v'};
	assert(sizeof(expected) == len);
	int i;
	jnx_term_printf_in_color(JNX_COL_WHITE, ".");
	for (i=0; i < len; i++) {
		assert(out[i] == expected[i]);
	}
	return 11;
}
void test_binary_data_in_stream_ipc_comms() {
	JNXLOG(LDEBUG, "Test binary data sent and received via stream socket\n");
	fflush(stdout);
	remove("binary_stream");
	jnx_unix_socket *ss = jnx_unix_stream_socket_create("binary_stream");
	uint8_t buffer[] = {'a', 'b', 'c', '\0', '\n', '\t', 0x01, 0xff, 0x13, '\f', '\v'};
	pid_t child_pid;
	if ((child_pid = fork()) != -1) {
		if (child_pid == 0) {
			jnx_unix_socket *cs = 0;
			sleep(1);
			cs = jnx_unix_stream_socket_create("binary_stream");
			jnx_unix_stream_socket_send(cs, buffer, sizeof(buffer));
			jnx_unix_socket_destroy(&cs);
			exit(0);
		}
		else {
			jnx_unix_stream_socket_listen(ss, 5, check_binary_stream);
			jnx_term_printf_in_color(JNX_COL_WHITE, "closing server socket\n");
			jnx_unix_socket_destroy(&ss);	
		}
	}
	JNXLOG(LDEBUG,"OK");
}
int check_large_stream(uint8_t *out, size_t len, jnx_unix_socket *rs) {
	counter++;
	int i;
	jnx_term_printf_in_color(JNX_COL_WHITE, ".");
	fflush(stdout);
	if (counter < 5)
		for (i = 0; i < len; i++) {
			assert(out[i] == i % 256);
		}
	if (counter < 4)
		return 0;
	else 
		return 4;	
}
void test_large_data_in_stream_ipc_comms() {
	JNXLOG(LDEBUG, "Test large data sent and received via stream socket\n");
	fflush(stdout);
	remove("large_stream");
	jnx_unix_socket *ss = jnx_unix_stream_socket_create("large_stream");

	uint8_t buffer[2000];
	int i;
	for (i = 0; i < 2000; i++) {
		buffer[i] = i % 256;
	}

	counter = 0;
	pid_t child_pid;
	if ((child_pid = fork()) != -1) {
		if (child_pid == 0) {
			jnx_unix_socket *cs = 0;
			int lengths[4] = {1023, 1024, 1025, 2000};
			int i;
			for (i=0; i<4; i++) {
				sleep(1);
				cs = jnx_unix_stream_socket_create("large_stream");
				jnx_unix_stream_socket_send(cs, buffer, lengths[i]);
				jnx_unix_socket_destroy(&cs);
			}
			exit(0);
		}
		else {
			jnx_unix_stream_socket_listen(ss, 5, check_large_stream);
			jnx_term_printf_in_color(JNX_COL_WHITE, "closing server socket\n");
			jnx_unix_socket_destroy(&ss);	
		}
	}
	JNXLOG(LDEBUG,"OK");
}
// datagram_listen negative scenarios
extern int bind_datagram_socket(jnx_unix_socket*);
extern int receive_from_datagram_socket(jnx_unix_socket*,jnx_unix_socket**,char**,int*);
void datagram_bind_fails() {
	jnx_unix_socket *s = jnx_unix_datagram_socket_create("datagram_sun");
	jnx_unix_socket *s2 = jnx_unix_datagram_socket_create("datagram_sun");

	int retval = bind_datagram_socket(s);
	retval = bind_datagram_socket(s2);
	assert(retval == -1);
	jnx_unix_socket_destroy(&s);
	jnx_unix_socket_destroy(&s2);
}
void datagram_receive_fails() {
	jnx_unix_socket *s = jnx_unix_datagram_socket_create("datagram_sun");
	s->socket = -1;
	jnx_unix_socket *rs;
	char *out;
	int len;

	int retval = receive_from_datagram_socket(s, &rs, &out, &len);
	assert(retval == -1);
	assert(rs == NULL);
}
void test_negative_datagram_listen_scenarios() {
	JNXLOG(LDEBUG,"Test negative datagram listen scenarios");
	fflush(stdout);
	run_error_test(datagram_bind_fails, "jnx unix datagram socket bind:", 30);
	run_error_test(datagram_receive_fails, "jnx unix datagram socket recvfrom:", 34);
	JNXLOG(LDEBUG,"OK");
}
int datagram_callback(char *out, size_t len, jnx_unix_socket *rs) {
	counter++;
	assert(strncmp(out, "Hello world from datagram socket!", 33) == 0);
	jnx_term_printf_in_color(JNX_COL_WHITE, ".");
	fflush(stdout);
	if (counter < 5)
		return 0;
	else 
		return 5;
}
void test_datagram_ipc_comms() {
	JNXLOG(LDEBUG,"Test unix datagram socket inter-process communication.\n");
	fflush(stdout);
	remove("datagram_sun");
	jnx_unix_socket *ss = jnx_unix_datagram_socket_create("datagram_sun");
	counter = 0;

	pid_t child_pid;
	if ((child_pid = fork()) != -1) {
		if (child_pid == 0) {
			int i;
			jnx_unix_socket *cs = NULL; 
			for(i = 0; i < 5; i++) {
				sleep(1);
				cs = jnx_unix_datagram_socket_create("datagram_sun");
				jnx_unix_datagram_socket_send(cs, "Hello world from datagram socket!", 33);
				jnx_unix_socket_destroy(&cs);
			}
			exit(0);
		}
		else {
			jnx_unix_datagram_socket_listen(ss, datagram_callback);
			jnx_term_printf_in_color(JNX_COL_WHITE, "closing server socket\n");
			jnx_unix_socket_destroy(&ss);
		}
	}
	JNXLOG(LDEBUG,"OK");
}
void test_binary_data_in_datagram_ipc_comms() {
	JNXLOG(LDEBUG, "Test binary data sent and received via datagram socket\n");
	fflush(stdout);
	remove("binary_datagram");
	jnx_unix_socket *ss = jnx_unix_datagram_socket_create("binary_datagram");
	uint8_t buffer[] = {'a', 'b', 'c', '\0', '\n', '\t', 0x01, 0xff, 0x13, '\f', '\v'};
	pid_t child_pid;
	if ((child_pid = fork()) != -1) {
		if (child_pid == 0) {
			jnx_unix_socket *cs = 0;
			sleep(1);
			cs = jnx_unix_datagram_socket_create("binary_datagram");
			jnx_unix_datagram_socket_send(cs, buffer, sizeof(buffer));
			jnx_unix_socket_destroy(&cs);
			exit(0);
		}
		else {
			jnx_unix_datagram_socket_listen(ss, check_binary_stream);
			jnx_term_printf_in_color(JNX_COL_WHITE, "closing server socket\n");
			jnx_unix_socket_destroy(&ss);	
		}
	}
	JNXLOG(LDEBUG,"OK");
}
int check_large_datagram(uint8_t *out, size_t len, jnx_unix_socket *rs) {
	counter++;
	int i;
	assert(len <= 1024);
	jnx_term_printf_in_color(JNX_COL_WHITE, ".");
	fflush(stdout);
	if (counter < 5)
		for (i = 0; i < len; i++) {
			assert(out[i] == i % 256);
		}
	if (counter < 4)
		return 0;
	else 
		return 4;	
}
void test_large_data_in_datagram_ipc_comms() {
	JNXLOG(LDEBUG, "Test large data sent and received via datagram socket\n");
	fflush(stdout);
	remove("large_datagram");
	jnx_unix_socket *ss = jnx_unix_datagram_socket_create("large_datagram");

	uint8_t buffer[2000];
	int i;
	for (i = 0; i < 2000; i++) {
		buffer[i] = i % 256;
	}

	counter = 0;
	pid_t child_pid;
	if ((child_pid = fork()) != -1) {
		if (child_pid == 0) {
			jnx_unix_socket *cs = 0;
			int lengths[4] = {1023, 1024, 1025, 2000};
			int i;
			for (i=0; i<4; i++) {
				sleep(1);
				cs = jnx_unix_datagram_socket_create("large_datagram");
				jnx_unix_datagram_socket_send(cs, buffer, lengths[i]);
				jnx_unix_socket_destroy(&cs);
			}
			exit(0);
		}
		else {
			jnx_unix_datagram_socket_listen(ss, check_large_stream);
			jnx_term_printf_in_color(JNX_COL_WHITE, "closing server socket\n");
			jnx_unix_socket_destroy(&ss);	
		}
	}
	JNXLOG(LDEBUG,"OK");
}

// Test runner
int test_jnxunixsocket() {
	JNXLOG(LDEBUG,"Starting unix socket tests\n");
	test_stream_ipc_comms();
	test_binary_data_in_stream_ipc_comms();
	test_large_data_in_stream_ipc_comms();
	test_datagram_ipc_comms();
	test_binary_data_in_datagram_ipc_comms();
	test_large_data_in_datagram_ipc_comms();
	test_create_and_destroy();
	test_negative_send_stream_scenarios();
	test_negative_send_datagram_scenarios();
	test_negative_stream_listen_scenarios();
	test_negative_datagram_listen_scenarios();
	return 0;
}
