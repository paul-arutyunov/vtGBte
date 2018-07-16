#include <stdio.h>
#include <ncurses.h>

unsigned char drawing_space[8][8];

void draw_boxes();
void print_help();

int main()
{
	extern unsigned char drawing_space[8][8];
	int k;
	int color;
	int i;
	int j;
	MEVENT event;
	unsigned char b1,b2;
	unsigned char a = 0;
	unsigned int x = 0;
	unsigned int y = 0;

	initscr();
	if (has_colors() == FALSE) {
		endwin();
		printf("\nYour terminal does not support color. Life is dull without color, how can you live without it?");
		printf("\nTerminating :(\n");
		return 0;
	}
	noecho();
	start_color();
	keypad(stdscr, 1);
	mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);
	curs_set(0);
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);
	init_pair(3, COLOR_WHITE, COLOR_GREEN);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);

	init_color(COLOR_YELLOW, 150,650,400);
	init_color(COLOR_GREEN, 200,400,250);
	init_color(COLOR_BLACK, 100,200,150);

	color = 3;

	draw_boxes();
	
	while(1)
	{
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				attron(COLOR_PAIR(drawing_space[i][j]+1));
				move(5+i,10+j*2);
				printw("  ");
			}
		}
		
		for (i = 0; i < 4; i++) 
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
		for (i = 0; i < 8; i++)
		{
			b1 = 0;
			b2 = 0;
			for (j = 0; j < 8; j++)
			{
				a = drawing_space[i][j];
				b1 = b1 | ( (a>>1) << (7-j) );
				b2 = b2 | ( (a&1) << (7-j) );
			}
			move(5+i,30);
			printw("%.2X, %.2X",b1,b2);
		}
		
		x = x%8;
		y = y%8;
		move(5+y,10+x*2);
		attron(COLOR_PAIR(drawing_space[y][x]+1));	/* Change color to prevent cursor hiding the pixel */
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

			case ' ':
			 drawing_space[y][x] = color;
			 break;

			case 'r':
			 clear();
			 draw_boxes();
			 break;

			case 'h':
			 clear();
			 print_help();
			 break;

			case 'f':
			 for (i = 0; i<8; i++) for (j = 0; j<8; j++) 
						drawing_space[i][j] = color;
			 break;
			 
			case 'q':
			 clear();
			 curs_set(1);
			 attron(COLOR_PAIR(1));
			 move(5,10);
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

			case KEY_MOUSE:		/* Mess up mith mouse (DOESN'T WORK!) */ 
			 if (getmouse(&event) == OK)
			 {
			 	x = (event.x%8)-9;
			 	y = (event.y%8)-4;
				if (event.bstate & BUTTON1_PRESSED) drawing_space[y][x] = color;
			 }
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

void draw_boxes()
{
        attron(COLOR_PAIR(4));
        clear();
        printw("\n         Very Tiny GameBoy Tile Editor\n\n");
        printw("         [ ] [ ] [ ] [ ]     Hex Data:\n");
        printw("         +----------------+  +-------+\n");
        printw("         |                |  |       |\n");
        printw("         |                |  |       |\n");
        printw("         |                |  |       |\n");
        printw("         |                |  |       |\n");
        printw("         |                |  |       |\n");
        printw("         |                |  |       |\n");
        printw("         |                |  |       |\n");
        printw("         |                |  |       |\n");
        printw("         +----------------+  +-------+\n");
}

void print_help()
{
	attron(COLOR_PAIR(4));
	printw("\n\n\n");
	printw("         +-------- Help Message -----+\n");
	printw("         | Here's the list of cmds:  |\n");
        printw("         |                           |\n");
        printw("         | 1...4    = pick color     |\n");
        printw("         | ARROWS   = move cursor    |\n");
        printw("         | SPACE    = draw           |\n");
        printw("         | r        = redraw screen  |\n");
	printw("         | f        = fill with color|\n");
        printw("         | h        = get some help  |\n");
        printw("         | q        = quit           |\n");
        printw("         +--press space to continue--+\n");


	while (getch() != ' ') ;
	clear();
	draw_boxes();
}
