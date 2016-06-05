/*
 * =====================================================================================
 *
 *       Filename:  jnxfile.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/10/14 22:14:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (joensax@hush.com),
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "jnxfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <io.h> 
#include <assert.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include<windows.h>
#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif
jnx_size jnx_file_read(jnx_char* path, jnx_char **buffer,jnx_char *flags) {
  JNXCHECK(path);
  JNXCHECK(buffer);
  FILE* fp;
  if ((fp = fopen(path, flags)) == NULL) {
    perror("file: ");
    return 0;
  }
  if(fseek(fp, 0, SEEK_END) != 0) {
    perror("file: ");
    exit(1);
  }
  jnx_size size = ftell(fp);
  rewind(fp);
  (*buffer) = calloc(size + 1, sizeof(jnx_char));
  bzero((*buffer),size +1);
  fread((*buffer), 1, size, fp);
  fclose(fp);
  return size;
}
jnx_hashmap *jnx_file_read_kvp(jnx_char *path, jnx_size max_buffer, jnx_char *delimiter) {
  JNXCHECK(path);
  JNXCHECK(max_buffer);
  JNXCHECK(delimiter);
  FILE *file;
  if((file = fopen(path, "r+")) == NULL) {
    return NULL;
  }
  jnx_char buffer[2048];
  memset(buffer,0, 2048);
  jnx_hashmap *map = jnx_hash_create(max_buffer);

  while(fgets(buffer, sizeof(buffer), file) != NULL) 
  {
    jnx_char *key = strtok(buffer,delimiter);
    jnx_char *value = strtok(NULL,delimiter);

    if(value == NULL) {
      continue;
    }
    jnx_size vs = strlen(value);

    if(value[vs -1] != '\0') {
      value[vs -1] = '\0';
    }
    jnx_char *st = malloc(strlen(key));
    jnx_char *sv = malloc(strlen(value));
    bzero(st,strlen(key) +1);
    bzero(sv,strlen(value) +1);
    strncpy(st,key,strlen(key));
    strncpy(sv,value,strlen(value));
    jnx_hash_put(map,st,sv);
  }
  fclose(file);
  return map;
}
jnx_size jnx_file_write(jnx_char* path, jnx_char* data, jnx_size data_size,jnx_char *flags) {
  JNXCHECK(path);
  JNXCHECK(data);
  JNXCHECK(data_size);
  JNXCHECK(flags);
  FILE* fp;
  if ((fp = fopen(path, flags)) == NULL) {
    perror("file: ");
    return -1;
  }
  jnx_size size = fwrite(data, 1,data_size, fp);
  fclose(fp);
  return size;
}

jnx_int32 jnx_file_recursive_delete(jnx_char* path, int32_t depth) {
  JNXCHECK(path);
  JNXCHECK(depth);
  char buffer[1024];
  memset(buffer, 0, 2048);
  sprintf(buffer, "RMDIR /S %s", path);
  system(buffer);
  return 0;
}
static jnx_int32 jnx_file_path_exists(jnx_char *path) {
	if (_access(path, 0) == 0) {
		struct stat status;
		stat(path, &status);
		return (status.st_mode & S_IFDIR) != 0;
	}
	return 0;
}
static jnx_char *jnx_file_random_dir(jnx_char *basepath) {
  JNXCHECK(basepath);
  srand(time(NULL));
  jnx_long_int n = 0;
  do {
    n = rand() % 10;
  } while(n == 0);
  jnx_int32 i;
  for(i = 1; i < 10; ++i) {
    n *= 10;
    n += rand() %10;
  }

  jnx_char *s = malloc(sizeof(jnx_char) * 256);
  sprintf(s,"%s/%ld",basepath,n);
  return s;
}
jnx_int32 jnx_file_mktempdir(jnx_char *dirtemplate, jnx_char **path) {
  JNXCHECK(dirtemplate);
  if(jnx_file_path_exists(dirtemplate)) {
    jnx_char *tempdir=jnx_file_random_dir(dirtemplate);

	if(CreateDirectory((const unsigned short*) tempdir, NULL) == 0) {
      JNXLOG(LDEBUG,"jnx_file_mktempdir: Error making temporary directory [%s]\n",strerror(errno));
      *path = NULL;
      free(tempdir);
      return 1;
    } else {
      *path = tempdir;
      return 0;
    }
  }
  return 1;
}
jnx_int32 jnx_file_exists(jnx_char *file) {
  JNXCHECK(file);
  struct stat st;
  jnx_int32 ret = stat(file,&st);

  if(S_ISDIR(st.st_mode)) {
    return 0;
  }
  if(ret == 0) {
    return 1;
  }else {
    return 0;
  }
}
