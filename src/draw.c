/* Some draw functions included have I. */

#include "include/draw.h"
#include "include/def.h"

int display_guide;

void print_help()
{
	int c=0;
	attron(COLOR_PAIR(4));
	printw("\n\n\n");
	printw("         +--1/2--- Help Message -----+\n");
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

	while (getch())!=' ');
	clear();

	printw("\n\n\n");
	printw("         +--2/2--- Help Message -----+\n");
	printw("         | Here's the list of cmds:  |\n");
	printw("         |                           |\n");
	printw("         | u        = update display |\n");
	printw("         | , .      = choose tile    |\n");
	printw("         | l        = load from file |\n");
	printw("         | s        = save to file   |\n");
	printw("         | S        = resize tiles   |\n");
	printw("         |                           |\n");
	printw("         |                           |\n");
	printw("         +--press space to continue--+\n");

	while(getch()!=' ');
	clear();
	draw_boxes();
}



void textbox(int startx, int starty, int w, int h)
{
	int i;

	move(starty,startx);
	addch(/*ACS_ULCORNER*/'+');
	move(starty+h+1, startx);
	addch(/*ACS_LLCORNER*/'+');
	for (i = 1; i < w+1; i++)
	{
		move(starty,startx+i);
		addch(ACS_HLINE);
		move(starty+h+1,startx+i);
		addch(ACS_HLINE);

	}
	mvaddch(starty,startx+w+1, /*ACS_URCORNER*/'+');
	mvaddch(starty+h+1,startx+w+1, /*ACS_LRCORNER*/'+');
	
	for (i = 1; i < h+1; i++)
	{
		move(starty+i, startx);
		addch(ACS_VLINE);
		move(starty+i, startx+w+1);
		addch(ACS_VLINE);
	}
}

void draw_boxes()
{
        attron(COLOR_PAIR(4));
        clear();

        printw("\n         Very Tiny GameBoy Tile Editor\n\n");
        printw("         [ ] [ ] [ ] [ ]   ");
	move(3,13+w*2);
	printw("Hex Data:");
	textbox(9,4,w*2,h);
	textbox(13+(w*2),4,w,h);
}

void displayCanvas()
{
	int i,j;
	extern int h,w;
	extern unsigned char drawing_space[MAXH][MAXW];
	for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)	/* Display canvas */
			{
				attron(COLOR_PAIR(drawing_space[i][j]+1));
				move(5+i,10+j*2);

				if (display_guide) {
				printw(". ");
				} else printw("  ");
			}
		}


}
