/*
 * =====================================================================================
 *
 *       Filename:  jnxterm.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  01/30/13 20:33:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include <string.h>
#include "jnxterm.h"
#include "jnxthread.h"
#define JNX_TERM_RESET     0
#define JNX_TERM_BRIGHT    1
#define JNX_TERM_DIM       2
#define JNX_TERM_UNDERLINE 3
#define JNX_TERM_BLINK     4
#define JNX_TERM_REVERSE   7
#define JNX_TERM_HIDDEN    8

void text_and_background_color(int attr, int fg, int bg) {
  printf("%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
}
void text_color(int attr, int fg) {
  printf("%c[%d;%dm", 0x1B, attr, fg + 30);
}
int get_width() {
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  printf("columns %d\n", w.ws_col);
  return w.ws_col;
}
void jnx_term_default() {
  printf("%c[0m", 0x1B);
}
void jnx_term_color(int fg_col) {
  if ( JNX_COL_BLACK <= fg_col && fg_col <= JNX_COL_WHITE ) {
    text_color(JNX_TERM_RESET, fg_col);
  }
}
void jnx_term_printf_in_color(int fg_col, const char* format, ...) {
  jnx_term_color(fg_col);

  va_list ap;

  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);

  jnx_term_default();
}
