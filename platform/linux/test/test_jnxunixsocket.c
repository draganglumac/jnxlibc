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
	JNX_LOGC(JLOG_DEBUG,"Test creation of unix sockets");

	jnx_unix_socket *a = jnx_unix_socket_stream_create("/tmp/stream_a");
	(a, "/tmp/stream_a", SOCK_STREAM);
	jnx_unix_socket *b = jnx_unix_socket_datagram_create("/tmp/stream_b");
	verify_socket(b, "/tmp/stream_b", SOCK_DGRAM);

	jnx_socket_destroy(&a);
	assert(a == NULL);
	jnx_socket_destroy(&b);
	assert(b == NULL);

	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
// stream_send negative scenarios
void stream_send_connect_fails() {
	jnx_unix_socket *s = jnx_unix_socket_stream_create("/tmp/stream_sun");
	int retval = jnx_unix_socket_stream_send(s, "stream send", 12);
	assert(retval == 0);
	jnx_unix_socket_destroy(&s);
}
extern ssize_t write_to_stream_socket(jnx_unix_socket*, char*, ssize_t);
void stream_send_fails() {
	jnx_unix_socket *s = jnx_unix_socket_stream_create("/tmp/stream_sun");
	int retval = write_to_stream_socket(s, "stream send", 12);
	assert(retval == 0);
	jnx_unix_socket_destroy(&s);
}
void test_negative_send_stream_scenarios() {
	JNX_LOGC(JLOG_DEBUG,"Test negative send stream scenarios");
	fflush(stdout);
	run_error_test(stream_send_connect_fails, "connect:", 8);
	run_error_test(stream_send_fails, "send:", 5);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
// datagram_send negative scenarios
void datagram_send_fails() {
	jnx_unix_socket *s = jnx_unix_socket_datagram_create("/tmp/datagram_sun");
	int retval = jnx_unix_socket_datagram_send(s, "datagram send", 13);
	assert(retval == 0);
	jnx_unix_socket_destroy(&s);
}
void test_negative_send_datagram_scenarios() {
	JNX_LOGC(JLOG_DEBUG,"Test negative send datagram scenarios");
	fflush(stdout);
	run_error_test(datagram_send_fails, "sendto:", 7);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
// stream_listen negative scenarios
extern int bind_stream_socket(jnx_unix_socket*);
extern int listen_on_stream_socket(jnx_unix_socket*, ssize_t);
extern jnx_unix_socket *accept_stream_socket_connection(jnx_unix_socket*);
extern int read_stream_socket(jnx_unix_socket*,char**,int*len);
void stream_bind_fails() {
	jnx_unix_socket *s = jnx_unix_socket_stream_create("/tmp/stream_sun");
	jnx_unix_socket *s2 = jnx_unix_socket_stream_create("/tmp/stream_sun");

	int retval = bind_stream_socket(s);
	retval = bind_stream_socket(s2);
	assert(retval == -1);
	jnx_unix_socket_destroy(&s);
	jnx_unix_socket_destroy(&s2);
}
void stream_listen_fails() {
	jnx_unix_socket *s = jnx_unix_socket_stream_create("/tmp/sun_stream"); 
	int retval = listen_on_stream_socket(s, 5);
	assert(retval == -1);
	jnx_socket_destroy(&s);
}
void stream_accept_fails() {
	jnx_unix_socket *s = jnx_unix_socket_stream_create("/tmp/stream_sun");
	jnx_unix_socket *retval = accept_stream_socket_connection(s);
	assert(retval == NULL);
	jnx_unix_socket_destroy(&s);
}
void stream_read_fails() {
	jnx_unix_socket *s = jnx_unix_socket_stream_create("/tmp/stream_sun");
	char *out;
	int len;
	int retval = read_stream_socket(s, &out, &len);
	assert(retval == -1);
	jnx_unix_socket_destroy(&s);
}
void test_negative_stream_listen_scenarios() {
	JNX_LOGC(JLOG_DEBUG,"Test negative stream listen and receive scenarios");
	fflush(stdout);
	run_error_test(stream_bind_fails, "bind:", 5);
	run_error_test(stream_listen_fails, "listen:", 7);
	run_error_test(stream_accept_fails, "accept:", 7);
	run_error_test(stream_read_fails, "read:", 5);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
int stream_callback(char *out, size_t len, jnx_unix_socket *rs) {
	assert(strncmp(out, "Hello world from stream socket!", 31) == 0);
	return 808;
}
void test_stream_ipc_comms() {
	JNX_LOGC(JLOG_DEBUG,"Test stream unix socket inter-process communication.");
	fflush(stdout);
	jnx_unix_socket *ss = jnx_unix_socket_stream_create("/tmp/stream_sun");
	jnx_unix_socket *cs = jnx_unix_socket_stream_create("/tmp/stream_sun");

	pid_t child_pid;
	if ((child_pid = fork()) != -1) {
		if (child_pid == 0) {
			sleep(1);
			jnx_unix_socket_stream_send(cs, "Hello world from stream socket!", 31);
			jnx_unix_socket_destroy(&cs);
			exit(0);
		}
		else {
			jnx_unix_socket_stream_listen(ss, 5, stream_callback);
			jnx_unix_socket_destroy(&ss);
		}
	}
	jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}
// Test runner
int main(int argc, char **argv) {
	JNX_LOGC(JLOG_DEBUG,"Starting unix socket tests\n");
	test_create_and_destroy();
	test_negative_send_stream_scenarios();
	test_negative_send_datagram_scenarios();
	test_negative_stream_listen_scenarios();
	test_stream_ipc_comms();
	return 0;
}
