/*
	Copyright (C) 2018 Paul Arutyunov
*/


#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <ncurses.h>
#include "include/def.h"
#include "include/draw.h"
#include "include/file.h"
#include "include/str.h"

unsigned char drawing_space[MAXH][MAXW];
unsigned char clipboard[MAXH][MAXW];
unsigned char asset[256][16];
int assign[] = {0, 1, 2, 3}; /* Assign pallette */

#define esc 27

int h,w;
int maxcol, maxrow;

void initColors();
void updateCanvas(int tile);

void signalHandler(int signum) {
	endwin();
	printf("\033[?1003l\n");
	exit(signum);
}

int main(int argc, char *argv[])
{
	extern unsigned char drawing_space[MAXH][MAXW];
	extern unsigned char clipboard[MAXH][MAXW];
	extern char command[];
	int k;
	int color;
	extern int display_guide;
	int i;
	int j;
	int l;
	extern int maxcol, maxrow;
	int gstartx, gstarty;
	extern int h, w;
	unsigned char b1,b2;
	MEVENT event;
	uint8_t current_tile=0;
	unsigned char a = 0;
	unsigned int x = 0;
	unsigned int y = 0;
	extern int assign[];
	int plotting=0;
	char filename[STRING_LENGTH+1]={0};
		/*changed from pointer to a fixed string because
		the pointer didn't point to any allocated memory
		which should cause segfault
		*/
	h = 8;
	w = 8;
  set_escdelay(0);

	if (argc>2){
		printf("Too many arguments, dude! \n");
		printf("Press any key to quit...");
		getchar();
		return 2;
	}

	initscr();
	if (has_colors() == FALSE) {
		endwin();
		printf("\nYour terminal does not support color. Life is dull without color, how can you live without it?");
		printf("\nTerminating :(\n");
		return 1;
	}
	if (can_change_color() == FALSE) {
		printw("Hey!\nYour terminal seems to do not \nsupport color re-definitions.\n");
		printw("As this program uses neat custom palette,\n you might experience eye bleeding.\n");
		printw("Press any key to continue.");
		getch();
	}

	noecho();
	start_color();
	keypad(stdscr, 1);
	curs_set(0);
	cbreak();
	initColors();
	display_guide=0;	/* don't */
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	printf("\033[?1003h\n");
	mouseinterval(0);

	signal(SIGINT, signalHandler);

	color = 3;

	draw_boxes();

	if (argc==2) {
		strncpy(filename,argv[1],STRING_LENGTH);
		if (initFile(filename)==SUCCESS) loadAsset();
		updateCanvas(current_tile);
	}

	while(1)
	{
		getmaxyx(stdscr, maxrow, maxcol);
		displayCanvas();

		for (i = 0; i < 4; i++) /* Display color panel */
		{
			attron(COLOR_PAIR(assign[i]+1));
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


		move(h+6,9);
		printw("#%d     ",current_tile);

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
					/*b1 = b1 | ( (a>>1) << (7-j) );
					b2 = b2 | ( (a&1) << (7-j) );*/
					b1 = b1 | (a&1) << (7-j);
					b2 = b2 | (a>>1) << (7-j);
				}
				move(5+i,14+w*2+l*8);
				printw("%.2X, %.2X",b1,b2);
				asset[current_tile+(h/8*l)][i*2+1] = b2;
				asset[current_tile+(h/8*l)][i*2] = b1;
			}
		}

    if (x < w && y < h && x >= 0 && y >= 0) {
      x = x%w;
      y = y%h;
      move(5+y,10+x*2);

      /*Change color to prevent the cursor
       * from hiding the pixel underneath */
      attron(COLOR_PAIR(assign[drawing_space[y][x]]+1));
      printw("[]");
    }
		k = getch();

		switch (k)	/* Input */
		{
			case 'u':
			 updateCanvas(current_tile);
			 break;

			case KEY_MOUSE:
			 if (getmouse(&event)==OK)
			 {
				x = (event.x-10) >> 1;
				y = (event.y-5);
        if (y >= h || x >= w) break;

				y=y%h;
				x=x%w;

				if (event.bstate & BUTTON1_PRESSED)
				 plotting=1;

				if (event.bstate & BUTTON1_RELEASED)
				 plotting=0;

				if (plotting) drawing_space[y][x] = color;

				/*move(h+7,9);
				printw("x: %d, y: %d        ", x,y);*/
			 }

			 break;

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

			case ',':
			 current_tile-=(h/8)*(w/8);
			 updateCanvas(current_tile);
			 break;

			case '.':
			 current_tile+=(h/8)*(w/8);
			 updateCanvas(current_tile);
			 break;

			case '<':
			 if (assign[color]==0) assign[color]=3;
			 else assign[color] -= 1;
			 break;

			case '>':
			 assign[color] = (assign[color]+1) % 4;
			 break;

			case 'f':	/* Fill the whole tile with one
					   color (NOT flood fill)*/
			 for (i = 0; i<h; i++) for (j = 0; j<w; j++)
						drawing_space[i][j] = color;
			 break;

      case esc:
			case 'q':
			 curs_set(1);
			 attron(COLOR_PAIR(1));
			 move(h+7,9);
			 printw("Do you really want to quit? Y/N ");

			 if (getch() == 'y') {
			  endwin();
			  printf("\033[?1003l\n");
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

			case 'c':
			 for (i=0; i<h; i++)
			 {
				 for (j=0; j<w; j++)
				 {
					 clipboard[i][j] = drawing_space[i][j];
				 }
			 }
			 clear_line();
			 move(h+7,9);
			 attron(COLOR_PAIR(4));
			 printw("Copied %d tile(s)",(h/8)*(w/8));
			break;

			case 'p':
			 for (i=0; i<h; i++)
			 {
				 for (j=0; j<w; j++)
				 {
					 drawing_space[i][j] = clipboard[i][j];
				 }
			 }
			 clear_line();
			 move(h+7,9);
			 attron(COLOR_PAIR(4));
			 printw("Pasted %d tile(s)",(h/8)*(w/8));
			break;

			case 's':

			 move(h+7,9);
			 attron(COLOR_PAIR(4));
			 printw("> ");
			 get_input_line(filename,STRING_LENGTH);
			 if (initFile(filename)==SUCCESS) saveAsset();
			break;

			case 'l':

			 move(h+7,9);
			 attron(COLOR_PAIR(4));
			 printw("> ");
			 get_input_line(filename,STRING_LENGTH);
			 if (initFile(filename)==SUCCESS) loadAsset();
			 updateCanvas(current_tile);
			break;

			case 'S':
			 move(h+7,9);
			 attron(COLOR_PAIR(1));
			 move(h+8,9);
			 printw("Press Shift-S again to end");
			 curs_set(1);

       k = getch();
			 while (k != 'S' && k != esc) {
				if (k==KEY_DOWN) h+=8;
				if (k==KEY_UP && h>8) h-=8;
				if (k==KEY_RIGHT) w+=8;
				if (k==KEY_LEFT && w>8) w-=8;
				clear();
				draw_boxes();
				updateCanvas(current_tile);
				displayCanvas();
			 	move(h+7,9);
			 	attron(COLOR_PAIR(1));
			 	printw("%d, %d - use arrows to change",h,w);
			 	attron(COLOR_PAIR(4));
			 	move(h+8,9);
			 	attron(COLOR_PAIR(2));
			 	printw("Press Shift+S again when finished");
				refresh();
        k = getch();
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
	printf("\033[?1003l\n");
	return 0;
}

void initColors()
{
	if (init_pair(1, COLOR_BLACK, COLOR_WHITE)==ERR) {
		printw("Woops...");
		getch();
	}
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);
	init_pair(3, COLOR_WHITE, COLOR_GREEN);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);

	init_color(COLOR_YELLOW, 150,650,400);
	init_color(COLOR_GREEN, 200,400,250);
	init_color(COLOR_BLACK, 100,200,150);
}

void updateCanvas(int tile)
{
	extern unsigned char drawing_space[MAXH][MAXW];
	extern unsigned char asset[256][16];
	extern int h,w;

	int c;
	int i,j,l;
	int b1,b2;

	tile = tile % 256;

	for (i=0; i<h; i++)
	{
		for (l=0; l<w/8; l++)
		{
			for (j=0; j<8; j++)
			{
				c = 0;

				b1=asset[tile+(h/8*(l))][i*2];
				b2=asset[tile+(h/8*(l))][i*2+1];

				c = c | (((b1<<j) & 0x80) >> 7);
				c = c | (((b2<<j) & 0x80) >> 6);

				drawing_space[i][j+l*8] = c;
			}
		}
	}
}
