/*
 * =====================================================================================
 *
 *       Filename:  test_jnxhash.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/27/13 16:20:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "jnxfile.h"
#include "jnxnetwork.h"
#include "jnxterm.h"
#include "jnxcheck.h"
#include "jnxlog.h"
int main(int args, char **argv) {
    JNX_LOGC(JLOG_DEBUG,"Running test for jnxnetwork");

	unsigned int detected_family;
	char *detected_ip;

	int success = jnx_network_hostname_to_ip(AF_INET6,"localhost",&detected_ip, \
			&detected_family);
	JNX_LOGC(JLOG_DEBUG,"Detected ipv%d with ip of %s\n",detected_family == AF_INET6 ? 6:4, \
			detected_ip);
	JNXCHECK(detected_ip);
	JNXCHECK(detected_family != -1);
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
    return 0;
}
