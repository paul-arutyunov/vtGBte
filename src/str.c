#include "include/str.h"

void clear_line()
{
	extern int maxcol;
	int i;
	attron(COLOR_PAIR(4));
	move(h+7, 9);
	for (i=0; i<(maxcol-1); i++) addch(' ');
}

void get_input_line(char *string, int len)
{
	int c;
	int i = 0;
	int j = 0;
	int width = 0;
	int height = 0;

	getmaxyx(stdscr, height, width);
	curs_set(1);
	clear_line();
	move(h+7,11);

	if (strlen(string)>0) {
		for (i=0; i<strlen(string); i++) mvaddch(h+7,11+i,string[i]);
	}
	while ((c = getch()) != '\n' && c != EOF)
	{
		move(h+7,11+i);
		if (c == KEY_BACKSPACE && i>0) {
			i--;
			mvaddch(h+7,11+i,' ');
			move(h+7,11+i);
			string[i]='\0';
		}
		else if( !iscntrl(c) && strlen(string) < len) {
			string[i] = c;
			mvaddch(h+7,11+i,c);
			string[++i] = '\0';
		}
	}
	curs_set(0);
}


char *get_token(char *str, int *prev)
{
	static char token[32];
	memset(token, '\0', 31);

	int i = 0;

	while (str[i] != ' ' && str[i] != '\0') {
		token[i] = str[*prev+i];
		i++;
	}

	*prev = i+1;
	return token;
}
