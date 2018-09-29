/*
	Copyright (C) 2018 Paul Arutyunov
*/


#include <stdio.h>
#include <ncurses.h>
#include "def.h"
#include "idraw.h"
#include "str.h"

unsigned char drawing_space[MAXH][MAXW];
unsigned char asset[256][16];
int h,w;

void initColors();

int main()
{
	extern unsigned char drawing_space[MAXH][MAXW];
	extern char command[];
	int k;
	int color;
	extern int display_guide;
	int i;
	int j;
	int l;
	extern int h, w;
	unsigned char b1,b2;
	int current_tile=0;
	unsigned char a = 0;
	unsigned int x = 0;
	unsigned int y = 0;
	h = 8;
	w = 8;

	initscr();
	if (has_colors() == FALSE) {
		endwin();
		printf("\nYour terminal does not support color. Life is dull without color, how can you live without it?");
		printf("\nTerminating :(\n");
		return 1;
	}
	noecho();
	start_color();
	keypad(stdscr, 1);
	curs_set(0);
	initColors();
	display_guide=0;

	color = 3;

	draw_boxes();
	
	while(1)
	{
		displayCanvas();
		
		for (i = 0; i < 4; i++) /* Display color panel */
		{
			attron(COLOR_PAIR(i+1)); 
			move(3,10+i*4);
			printw("%d",i);
			if (i == color) {
			 attron(COLOR_PAIR(4));
			 mvaddch(2,10+i*4,'v');
			} else {
			 attron(COLOR_PAIR(4));
			 mvaddch(2,10+i*4,' ');
			}
		}

		/* Display hex data */
		for (i = 0; i < h; i++)
		{
			for (l = 0; l < w/8; l++)
			{
				b1 = 0;
				b2 = 0;
				for (j = 0; j < 8; j++)
				{
					a = drawing_space[i][j+l*8];
					b1 = b1 | ( (a>>1) << (7-j) );
					b2 = b2 | ( (a&1) << (7-j) );
				}
				move(5+i,14+w*2+l*8);
				printw("%.2X, %.2X",b1,b2);
				/*asset[current_tile][i] = b1;
				asset[current_tile][i]= b2;*/
			}
		}
		
		x = x%w;
		y = y%h;
		move(5+y,10+x*2);
		
		/*Change color to prevent the cursor
		 * from hiding the pixel underneath */
		attron(COLOR_PAIR(drawing_space[y][x]+1));
		printw("[]");
		k = getch();

		switch (k)	/* Input */
		{
			case KEY_UP:
			 y--;
			 break;

			case KEY_DOWN:
			 y++;
			 break;

			case KEY_LEFT:
			 x--;
			 break;

			case KEY_RIGHT:
			 x++;
			 break;

			case ' ':	/* Plot */
			 drawing_space[y][x] = color;
			 break;

			case 'r':	/* Redraw */
			 clear();
			 draw_boxes();
			 break;

			case 'h':	/* Help */
			 clear();
			 print_help();
			 break;

			case 'f':	/* Fill the whole tile with one
					   color (NOT flood fill)*/
			 for (i = 0; i<h; i++) for (j = 0; j<w; j++) 
						drawing_space[i][j] = color;
			 break;

			case 'q':
			 curs_set(1);
			 attron(COLOR_PAIR(1));
			 move(h+7,9);
			 printw("Do you really want to quit? Y/N ");
			
			 if (getch() == 'y') {
			  endwin();
			  return 0;
			 } else {
			  curs_set(0);
			  clear();
			  draw_boxes();
			 }
			 break;

			case 'g':	/* Show guide */
			 if (!display_guide)
			  display_guide = 1;
			 else
			  display_guide = 0;
			 break;

			case '\n':
			 move(h+7,9);
			 attron(COLOR_PAIR(4));
			 printw("> ");
			 get_input_line(command);
			break;

			case 'S':
			 move(h+7,9);
			 attron(COLOR_PAIR(2));
			 printw("Height: %d, Width: %d - use arrow keys to change\n         Press Shift-S when finished",h,w);
			 curs_set(1);

			 while ((k=getch())!='S') {
				if (k==KEY_DOWN) h+=8;
				if (k==KEY_UP && h>8) h-=8;
				if (k==KEY_RIGHT) w+=8;
				if (k==KEY_LEFT && w>8) w-=8;
				clear();
			 	move(h+7,9);
			 	attron(COLOR_PAIR(2));
			 	printw("Height: %d, Width: %d - use arrow keys to change",h,w);
			 	attron(COLOR_PAIR(4));
			 	move(h+8,9);
			 	attron(COLOR_PAIR(2));
			 	printw("Press Shift+S again when finished");
				draw_boxes();
				displayCanvas();
			 }

			 curs_set(0);
			 clear();
			 draw_boxes();
			 break;

			default: 
			 if (k >= '1' && k <= '4')
			  color = k-'1';
			 break;
		}
		refresh();
	}

	endwin();
	return 0;
}

void initColors()
{
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);
	init_pair(3, COLOR_WHITE, COLOR_GREEN);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);

	init_color(COLOR_YELLOW, 150,650,400);
	init_color(COLOR_GREEN, 200,400,250);
	init_color(COLOR_BLACK, 100,200,150);
}
