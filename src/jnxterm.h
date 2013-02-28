/*
 * =====================================================================================
 *
 *       Filename:  jnxterm.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/30/13 20:33:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#define JNX_COL_BLACK   0
#define JNX_COL_RED     1
#define JNX_COL_GREEN   2
#define JNX_COL_YELLOW  3
#define JNX_COL_BLUE    4
#define JNX_COL_MAGENTA 5
#define JNX_COL_CYAN    6
#define JNX_COL_WHITE   7

void jnx_term_default();
void jnx_term_color(int fg_col);
void jnx_printf_in_color(int fg_col, const char* format, ...);

void jnx_term_load(int state);
void jnx_term_override_stdout(char *path);
void jnx_term_reset_stdout(void);
