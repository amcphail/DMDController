#include <stdio.h>
#include <stdlib.h>

#include <cstring>

#include "dmd_file.h"

#define BMP_HEADER_SIZE 14

/*-----------------------------------------------------------------------------------------------*/

int loadFile(const char* fn,int& ln,char*& data)
{
  char* buffer = 0;
  long length = 0;
  FILE * f = fopen (fn, "rb");

  if (f)
    {
      fseek(f, 0, SEEK_END);
      length = ftell(f);
      fseek(f, 0, SEEK_SET);
      buffer = (char*)malloc(length+1);
      if (buffer)
	{
	  fread(buffer, 1, length, f);
	}
      fclose (f);
      buffer[length] = '\0';
    }
  else
    {
        return -1;
    }

  ln = length;
  data = buffer;

  return 0;
}

int getRawBmp(int flen, const char* fbuf, int& blen, char*& bbuf)
{
  // position of size information is 10th byte
  const int* p_int = reinterpret_cast<const int*>(&fbuf[10]);
  int num = *p_int;

  blen = flen - num;
  
  bbuf = (char*)malloc(sizeof(char)*blen);
  memcpy(bbuf,&fbuf[num],blen);

  return 0;
}

/*-----------------------------------------------------------------------------------------------*/

// width in bits
// ibuf must be allocated before call to this function
int bitmapToInt(int width,int height,const char* cbuf, uint8_t*& ibuf)
{
    int cols = (width-1)/8 + 1;

    int ij, ij8;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < cols; j++) {

            ij = i*cols+j;
            ij8 = ij*8;

            ibuf[ij8+0] = (cbuf[ij] & 0x80) ? 1 : 0;
            ibuf[ij8+1] = (cbuf[ij] & 0x40) ? 1 : 0;
            ibuf[ij8+2] = (cbuf[ij] & 0x20) ? 1 : 0;
            ibuf[ij8+3] = (cbuf[ij] & 0x10) ? 1 : 0;
            ibuf[ij8+4] = (cbuf[ij] & 0x08) ? 1 : 0;
            ibuf[ij8+5] = (cbuf[ij] & 0x04) ? 1 : 0;
            ibuf[ij8+6] = (cbuf[ij] & 0x02) ? 1 : 0;
            ibuf[ij8+7] = (cbuf[ij] & 0x01) ? 1 : 0;

        }
    }
    return 0;
}

// width in bits
// cbuf must be allocated before call to this function
int intToBitmap(int width, int height, const uint8_t* ibuf, char*& cbuf)
{
    int cols = (width-1)/8 + 1;

    int ij, ij8;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < cols; j++) {

            ij = i*cols+j;
            ij8 = ij*8;

            cbuf[ij] = ((ibuf[ij8+0] ? 1 : 0) << 7) | ((ibuf[ij8+1] ? 1 : 0) << 6)
                     | ((ibuf[ij8+2] ? 1 : 0) << 5) | ((ibuf[ij8+3] ? 1 : 0) << 4)
                     | ((ibuf[ij8+4] ? 1 : 0) << 3) | ((ibuf[ij8+5] ? 1 : 0) << 2)
                     | ((ibuf[ij8+6] ? 1 : 0) << 1) | ((ibuf[ij8+7] ? 1 : 0));
        }
    }
    return 0;
}

/*-----------------------------------------------------------------------------------------------*/

int gcd(int a, int b)
{
    while (b != 0)
    {
        a %= b;
        a ^= b;
        b ^= a;
        a ^= b;
    }

    return a;
}

int scrollHor(int width, int height, int pixels, char*& buf)
{
    while (pixels < 0) pixels += width;
    if (pixels >= width) pixels %= width;

    uint8_t* temp = (uint8_t*)malloc(sizeof(uint8_t)*height);

    uint8_t* ibuf = (uint8_t*)malloc(sizeof(uint8_t)*width*height);

    bitmapToInt(width,height,buf,ibuf);

    int l, k;

    for (int i = 0; i < gcd(pixels,width); i++) {
        for (int j = 0; j < height; j++) {
            temp[j] = ibuf[j*width+i];
        }
        l = i;
        while (1) {
            k = l + pixels;
            if (k >= width) k = k - width;
            if (k == i) break;
            for (int j = 0; j < height; j++) {
                ibuf[j*width+l] = ibuf[j*width+k];
            }
            l = k;
        }
        for (int j = 0; j < height; j++) {
            ibuf[j*width+l] = temp[j];
        }
    }

    intToBitmap(width,height,ibuf,buf);

    free(ibuf);

    return 0;
}

int scrollVer(int width, int height, int pixels, char*& buf)
{
    while (pixels < 0) pixels += height;
    if (pixels >= height) pixels %= height;

    uint8_t* temp = (uint8_t*)malloc(sizeof(uint8_t)*width);

    uint8_t* ibuf = (uint8_t*)malloc(sizeof(uint8_t)*width*height);

    bitmapToInt(width,height,buf,ibuf);

    int l, k;

    for (int i = 0; i < gcd(pixels,height); i++) {
        for (int j = 0; j < width; j++) {
            temp[j] = ibuf[i*width+j];
        }
        l = i;
        while (1) {
            k = l + pixels;
            if (k >= height) k = k - height;
            if (k == i) break;
            for (int j = 0; j < width; j++) {
                ibuf[l*width+j] = ibuf[k*width+j];
            }
            l = k;
        }
        for (int j = 0; j < width; j++) {
            ibuf[l*width+j] = temp[j];
        }
    }

    intToBitmap(width,height,ibuf,buf);

    free(ibuf);

    return 0;
}


/*-----------------------------------------------------------------------------------------------*/

int getLine(int& length, char*& line, const int len, const char* buf)
{
  length = 0;

  for (int i = 0; i < len; i++)
    {
      if (buf[i] == '\n')
	{
	  length = i;
	  break;
	}
    }
  
  line = (char*)malloc(length);
  
  memcpy(line,buf,length);

  line[length] = '\0';

  return 0;
}

int getWord(int& length, char*& line, const int len, const char* buf)
{
  length = 0;

  char ch;
  
  for (int i = 0; i < len; i++)
    {
      ch = buf[i];
      if (ch == '\n' || ch == '\t' || ch == ' ' || ch == '\0')
	{
	  length = i;
	  break;
	}
      else if ((i+1) == len) length = len;
    }
  
  line = (char*)malloc(length+1);
  
  memcpy(line,buf,length);

  line[length] = '\0';

  return 0;
}

int skipWhiteSpace(int& length, const int len, const char* buf)
{
  length = 0;

  char ch;

  int i;
  for (i = 0; i < len; i++)
    {
      ch = buf[i];
      if (ch == '\t' || ch == ' ') length = i+1;
      else break;
    }

  return 0;
}

int parseSingleImageMap(ImageMap*& im, int& length, const int len, const char* buf)
{
  int line_length = 0;
  int total = 0;
  int current = 0;
  char* line;
  char* number;
  char* word;

  getLine(line_length,line,len,buf);
  skipWhiteSpace(current,line_length,line);

  total += current;

  getWord(current,number,line_length-total,&line[total]);

  total += current;

  skipWhiteSpace(current,line_length-total,&line[total]);

  total += current;

  getWord(current,word,line_length-total,&line[total]);

  total += current;
 
  unsigned long int num = atoi(number);

  ImageMap* image_map = new ImageMap;
  image_map->inum = num;
  image_map->filename = word;

  im = image_map;

  length = line_length;

  return 0;
}


int parseImageMap(int& num, ImageMap**& imps, const int len, const char* buf)
{
  int length = 0;
  int total = 0;
  num = 0;

  ImageMap** ims = 0;
  ImageMap** tmp;

  ImageMap* im;

  while (total < len)
    {
      tmp = ims;
      ims = new ImageMap * [num+1];

      for (int i = 0; i < num; i++) ims[i] = tmp[i];

      parseSingleImageMap(im,length,len-total,&buf[total]);
      ims[num] = im;

      total += length + 1;
      num += 1;

      if (tmp) delete tmp;
    }
  imps = ims;

  return 0;
}

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
