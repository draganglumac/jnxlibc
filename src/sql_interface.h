#ifndef __SQL_INTERFACE_H__
#define __SQL_INTERFACE_H__
#include <mysql/mysql.h>
///function pointer callback
typedef void (*sql_callback)(MYSQL_RES*);
///Warning: SQL will need to be included into project as dependency
///sql_interface_setup sets the HOST USERNAME PASSWOD
int sql_interface_setup(char *host,char *username, char *password);
///sql_query takes query string and callback
int sql_query(char *query,void (*sql_callback)(MYSQL_RES*));
void sql_close(void);
#endif