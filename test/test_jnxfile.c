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
#include <sys/types.h>
#include <sys/stat.h>
#include "../src/jnxfile.h"
#include "../src/jnxterm.h"
int main(int args, char **argv)
{
	system("mkdir -p bob/james/frank");
	system("touch bob/a.txt");
	system("touch bob/james/b.txt");
	system("touch bob/james/frank/c.txt");
	system("touch bob/james/frank/d.txt");
	system("touch bob/james/e.txt");
	system("touch bob/f.txt");
	system("touch bob/g.txt");
	
	
	jnx_file_recursive_delete("bob",3);
	return 0;
}
