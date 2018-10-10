#include "file.h"

FILE *fp;

int initFile(char *filename)
{
	extern FILE *fp;
	extern int h,w;
	fp = fopen(filename,"rb+");
	move(h+7,9);
	if (fp!=NULL) {
		printw("Opened file '%s'",filename);
		return SUCCESS;	
	} else {
	/*	fp = fopen(filename,"wb
		printw("Created file '%s'",filename);*/
		printw("Can't open file '%s'",filename);
		return FAILURE;
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
