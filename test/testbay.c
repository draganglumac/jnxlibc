#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/sql_interface.h"

void _callback(MYSQL_RES *res)
{
	
}
int main(int argc, char **argv) {

	sql_interface_setup("localhost","jonesax","trinity");
	sql_callback c = &_callback;
	
	sql_query("use test;",c);
    return 0;
}
