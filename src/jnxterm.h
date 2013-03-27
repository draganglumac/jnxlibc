/**
 * @file jnxterm.h
 * @brief terminal manipulation of colours, stdout and loading spinnders
 *
 * #include <jnxc_headers/jnxterm.h>
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

/**
 * @fn jnx_term_default
 * @brief resets to default terminal colors
 */
void jnx_term_default();

/**
 * @fn jnx_term_color(int fg_col)
 * @brief changes the terminal color 
 * @param fg_col select a color between 0-7 from JNX_COL
 */
void jnx_term_color(int fg_col);

/**
 * @fn jnx_term_printf_in_color(int fg_col, const char* format, ...)
 * @brief prints a single line in selected color
 * @param fg_col select a color between 0-7 from JNX_COL
 * @param format formatted string
 * @param ... formatted string optional arguments
 */
void jnx_term_printf_in_color(int fg_col, const char* format, ...);
/**
 * @fn jnx_term_load(int state)
 * @brief starts/stops a loading spinner in the terminal on another thread
 * @param state toggles between 0 and 1 for off/on
 */
void jnx_term_load(int state);

/**
 * @fn jnx_term_override_stdout(char *path)
 * @brief redirects stdout to a file
 * @param path path of the output file, requires real file path
 */
void jnx_term_override_stdout(char *path);
/**
 * @fn jnx_term_reset_stdout
 * @brief resets the stdout back to the current terminal
 */
void jnx_term_reset_stdout(void);

#endif
