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
 *         Author:  alexsimonjones@gmail.com, 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __JNX_TERM_H__
#define __JNX_TERM_H__
#define JNX_COL_BLACK   0
#define JNX_COL_RED     1
#define JNX_COL_GREEN   2
#define JNX_COL_YELLOW  3
#define JNX_COL_BLUE    4
#define JNX_COL_MAGENTA 5
#define JNX_COL_CYAN    6
#define JNX_COL_WHITE   7

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_term_default
 *  Description:  Sets terminal color to default
 * =====================================================================================
 */
void jnx_term_default();
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_term_color
 *  Description:  Sets the terminal color
 * =====================================================================================
 */
void jnx_term_color(int fg_col);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_term_printf_in_color
 *  Description:  Prints given the target colour with arguments
 * =====================================================================================
 */
void jnx_term_printf_in_color(int fg_col, const char* format, ...);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_term_load
 *  Description:  Starts a loading spinner in the current terminal, does not block io
 * =====================================================================================
 */
void jnx_term_load(int state);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_term_override_stdout
 *  Description:  Redirects stdout into a file
 * =====================================================================================
 */
void jnx_term_override_stdout(char *path);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_term_reset_stdout
 *  Description:  Pipes the stdout back to the current terminal
 * =====================================================================================
 */
void jnx_term_reset_stdout(void);

#endif
