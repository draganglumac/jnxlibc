#ifndef __JNXFILE_H__
#define __JNXFILE_H__

///Returns a callocd string with the file data, you will assume responsibility for freeing 
char* jnx_file_tostr(char*path);
int jnx_file_fromstr(char*path,char*data);
#endif