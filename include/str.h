#ifndef _STR_H_
#define _STR_H_

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>

extern int h,w;

void get_input_line(char *string, int len);

char *get_token(char *str, int* prev);


#endif
