#include "include/str.h"

//char command[20];

void get_input_line(char *string, int len)
{
	int c;
	int i = 0;
	int j = 0;

	curs_set(1);
	move(h+7,11);
	printw("                          ");
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
	char *token;

	int i = 0;

	while (str[i] != ' ' || str[i] != '\0') {
		token[i] = str[*prev+i];
		i++;
	}

	*prev = i+1;
	return token;
}
