#include "str.h"

/* Had to wrote token parsing code myself. Standard strtok() is unsafe */

char command[20];

void get_input_line(char *string)
{
	int c;
	int i = 0;
	int j = 0;

	curs_set(1);
	while ((c = getch()) != '\n')
	{
		if (c == '\b' && i>=0) {
			string[i]='\0';
			i--;
		} else {
			string[i] = c;
			i++;
		}
		move(h+7,11);
		string[i+1] = '\0';
		while (string[j]!='\0') addch(string[j++]);
	}

	string[i]='\0';

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
