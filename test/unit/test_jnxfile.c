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
#include "jnxlog.h"
#include "jnxfile.h"
#include "jnxterm.h"
void test_recursive_delete() {
  system("mkdir -p bob/james/frank");
  system("touch bob/a.txt");
  system("touch bob/james/b.txt");
  system("touch bob/james/frank/c.txt");
  system("touch bob/james/frank/d.txt");
  system("touch bob/james/e.txt");
  system("touch bob/f.txt");
  system("touch bob/g.txt");
  jnx_file_recursive_delete("bob",3);
  struct stat _sb;
  assert(stat("bob",&_sb) != 0);

}
void test_jnx_file() {
  system("echo 'whats up' > tmp.txt");
  char *buffer;
  int size = jnx_file_read("tmp.txt",&buffer,"r");
  char *test_string = "whats up\n";
  while(*buffer != '\0') {
    char a = *buffer;
    char b = *test_string;
    if(a != b) {
      JNXLOG(LDEBUG,"%X %X do not match\n",a,b);
    }
    assert(a == b);
    test_string++;
    buffer++;
  }
  system("rm tmp.txt");
}
void test_jnx_dir() {
  char *path;
  char l[] = "/tmp";
  int ret = jnx_file_mktempdir(l,&path);
  assert(ret == 0);
  remove(path);
  free(path);
}
int test_jnxfile() {
  JNXLOG(LDEBUG,"Running file tests...\n");
  JNXLOG(LDEBUG,"- test recursive delete");
  test_recursive_delete();
  JNXLOG(LDEBUG,"OK");
  JNXLOG(LDEBUG,"- test file read");
  test_jnx_file();
  JNXLOG(LDEBUG,"OK");
  JNXLOG(LDEBUG,"- test temp directory");
  test_jnx_dir();
  JNXLOG(LDEBUG,"OK");
  return 0;
}
