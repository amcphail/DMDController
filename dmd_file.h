#ifndef __AVHM__DMD_FILE_H__
#define __AVHM__DMD_FILE_H__

#include <stdint.h>

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

int loadFile(const char*,int&,char*&);
int getRawBmp(int,const char*,int&,char*&);

int bitmapToInt(int,int,const char*, uint8_t*&);
int intToBitmap(int,int,const uint8_t*,char*&);

int scrollHor(int,int,int,char*&);
int scrollVer(int,int,int,char*&);

struct ImageMap
{
  unsigned long int inum;
  char* filename;
};

int getLine(int&, char*&, const int, const char*);
int getWord(int&, char*&, const int, const char*);
int skipWhiteSpace(int&, const int, const char*);

int parseSingleImageMap(ImageMap*&, int&, const int, const char*);
int parseImageMap(int&, ImageMap**&, const int, const char*);


/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/



#endif
