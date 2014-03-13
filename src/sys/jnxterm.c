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

static int ISLOADING_BAR = 0;
static jnx_thread* bar_loader_thread;
static int ISLOADING_SPIN = 0;
static jnx_thread* loader_thread;
static int fd;
static fpos_t pos;
static char *loading_bar[11] = {
    "Loading:[#          ]",
    "Loading:[ #         ]",
    "Loading:[  #        ]",
    "Loading:[   #       ]",
    "Loading:[    #      ]",
    "Loading:[     #     ]",
    "Loading:[      #    ]",
    "Loading:[       #   ]",
    "Loading:[        #  ]",
    "Loading:[         # ]",
    "Loading:[          #]",

};
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
void jnx_term_override_stdout(char *path) {
    fflush(stdout);
    fgetpos(stdout,&pos);
    fd = dup(fileno(stdout));
    freopen(path,"w",stdout);

}
void jnx_term_reset_stdout() {
    fflush(stdout);
    dup2(fd,fileno(stdout));
    close(fd);
    clearerr(stdout);
    fsetpos(stdout,&pos);
}
void *loading_loop(void *ptr) {
    struct timespec _nano;
    _nano.tv_sec = 0;
    _nano.tv_nsec = (100 * 1000000);

    char spin_status[4] = {'|','/','-','\\'};
    int counter = 0;
    char *loading_text = "Loading";
    printf("\033[s%s",loading_text);

    while(ISLOADING_SPIN == 1) {
        printf("\033[u\033[2K%s %c",loading_text, spin_status[counter%4]);
        fflush(stdout);
        ++counter;
        nanosleep(&_nano,NULL);
    }
    return NULL;
}
void *loading_loop_bar() {
    struct timespec _nano;
    _nano.tv_sec = 0;
    _nano.tv_nsec = (100 * 1000000);
    int c = 0;
    printf("\033[s");
    while(ISLOADING_BAR) {
        if(c == 11) {
            c = 0;
        }
        printf("\033[2K\033[u%s",loading_bar[c]);
        fflush(stdout);
        nanosleep(&_nano,NULL);
        ++c;
    }
    return NULL;
}
void jnx_term_load_spinner(int state) {
    ISLOADING_SPIN = state;
    if(ISLOADING_SPIN == 1) {
        loader_thread = jnx_thread_create(loading_loop,NULL);
    } else {
        void *data;
        jnx_thread_join(loader_thread,&data);
        printf("\033[u\033[2K\n");
    }
}
void jnx_term_load_bar(int state) {
    ISLOADING_BAR = state;
    if(ISLOADING_BAR == 1) {
        bar_loader_thread = jnx_thread_create(loading_loop_bar,NULL);
    } else {
        void *data;
        jnx_thread_join(bar_loader_thread,&data);
        printf("\033[u\033[2K\n");
    }
}
