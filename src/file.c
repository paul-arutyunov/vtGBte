#include "include/file.h"

FILE *fp;

int initFile(char *filename)
{
	extern FILE *fp;
	extern int h,w;
	int c;
	fp = fopen(filename,"rb+");
	move(h+7,9);
	if (fp!=NULL) {
		printw("Opened file '%s'",filename);
		return SUCCESS;	
	} else {
		printw("There's no file '%s'. Create?",filename);
		c=getch();

		if (c=='y')
		{
			fp = fopen(filename,"wb+");

			if (fp==NULL) {
				printw("\r         Can't create file '%s'          ",filename);
				return FAILURE;
			} else {
				printw("\r         Created file '%s'               ",filename);
				return SUCCESS;
			}
		}
		if (c=='n')
		{
			printw("\r        Oki.                 ");
			return FAILURE;
		}
	}
}

void saveAsset()
{
	extern unsigned char asset[256][16];
	extern FILE *fp;
	int i,j;

	for (i=0; i<256; i++)
	{
		fwrite(asset[i],sizeof(unsigned char),16,fp);
	}
	fclose(fp);
}

void loadAsset()
{
	extern unsigned char asset[256][16];
	extern FILE *fp;
	int i,j;

	for (i=0; i<256; i++)
	{
		fread(asset[i],sizeof(unsigned char),16,fp);
	}
	fclose(fp);
}
