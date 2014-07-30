#include "jnxfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#define __USE_XOPEN_EXTENDED
#include <ftw.h>
#include <libgen.h>
#include <assert.h>
#include "jnxlog.h"
#include "jnxcheck.h"
size_t jnx_file_read(char* path, char **buffer,char *flags) {
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
  size_t size = ftell(fp);
  rewind(fp);
  (*buffer) = calloc(size, sizeof(char));
  fread((*buffer), 1, size, fp);
  fclose(fp);
  return size;
}
jnx_hashmap *jnx_file_read_kvp(char *path, size_t max_buffer, char *delimiter) {
  JNXCHECK(path);
  JNXCHECK(max_buffer);
  JNXCHECK(delimiter);
  FILE *file;
  if((file = fopen(path, "r+")) == NULL) {
    return NULL;
  }
  char buffer[max_buffer];
  memset(buffer,0,max_buffer);
  jnx_hashmap *map = jnx_hash_create(max_buffer);

  while(fgets(buffer, sizeof(buffer), file) != NULL) 
  {
    char *key = strtok(buffer,delimiter);
    char *value = strtok(NULL,delimiter);

    if(value == NULL) {
      continue;
    }
    size_t vs = strlen(value);

    if(value[vs -1] != '\0') {
      value[vs -1] = '\0';
    }
    char *st = malloc(strlen(key));
    char *sv = malloc(strlen(value));
    bzero(st,strlen(key) +1);
    bzero(sv,strlen(value) +1);
    strncpy(st,key,strlen(key));
    strncpy(sv,value,strlen(value));
    jnx_hash_put(map,st,sv);
  }
  fclose(file);
  return map;
}
size_t jnx_file_write(char* path, char* data, size_t data_size,char *flags) {
  JNXCHECK(path);
  JNXCHECK(data);
  JNXCHECK(data_size);
  JNXCHECK(flags);
  FILE* fp;
  if ((fp = fopen(path, flags)) == NULL) {
    perror("file: ");
    return -1;
  }
  size_t size = fwrite(data, 1,data_size, fp);
  fclose(fp);
  return size;
}
int jnx_file_recursive_delete_callback(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
  remove(fpath);
  return 0;
}
int jnx_file_recursive_delete(char* path, int depth) {
  JNXCHECK(path);
  JNXCHECK(depth);
  if( nftw(path,jnx_file_recursive_delete_callback, depth, FTW_DEPTH) != 0) {
    perror("jnx_file_recursive_delete");
    return -1;
  }
  return 0;
}
static int jnx_file_path_exists(char *path) {
  JNXCHECK(path);
  size_t s = 512;
  char buffer[s];
  getcwd(buffer,s);
  if(buffer == NULL) {
    JNX_LOGC(JLOG_ALERT,"jnx_file_path_exists: Unable to validate cwd\n");
    return 0;
  }
  if(chdir(path) != 0) {
    return 0;
  }
  chdir(buffer);
  return 1;
}
static char *jnx_file_random_dir(char *basepath) {
  JNXCHECK(basepath);
  srand(time(NULL));
  unsigned long int n = 0;
  do {
    n = rand() % 10;
  } while(n == 0);
  int i;
  for(i = 1; i < 10; ++i) {
    n *= 10;
    n += rand() %10;
  }

  char *s = malloc(sizeof(char) * 256);
  sprintf(s,"%s/%zu",basepath,n);
  return s;
}
int jnx_file_mktempdir(char *dirtemplate, char **path) {
  JNXCHECK(dirtemplate);
  if(jnx_file_path_exists(dirtemplate)) {
    char *tempdir=jnx_file_random_dir(dirtemplate);
    if((mkdir(tempdir, S_IRWXU  | S_IRWXG | S_IROTH | S_IXOTH)) != 0) {
      JNX_LOGC(JLOG_ALERT,"jnx_file_mktempdir: Error making temporary directory [%s]\n",strerror(errno));
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
int jnx_file_exists(char *file) {
  JNXCHECK(file);
  struct stat st;
  int ret = stat(file,&st);

  if(S_ISDIR(st.st_mode)) {
    return 0;
  }
  if(ret == 0) {
    return 1;
  }else {
    return 0;
  }
}
