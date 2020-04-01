#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
  printf("Size of int: %d\n",sizeof(int));

  char* buffer = 0;
  long length = 0;
  FILE * f = fopen ("DMDImages/test.bmp", "rb");

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

  const int* p_int = reinterpret_cast<const int*>(&buffer[10]);
  int num = *p_int;

  uint8_t b;
  
  for (int i = num; i < length; i++) {
    b = (uint8_t)buffer[i];
    printf("%d%d%d%d%d%d%d%d"
	   ,b & 128 ? 1 : 0,b & 64 ? 1 : 0,b & 32 ? 1 : 0,b & 16 ? 1 : 0
	   ,b & 8   ? 1 : 0,b & 4  ? 1 : 0,b & 2  ? 1 : 0,b & 1  ? 1 : 0);
    printf("\n");
  }
  
  return 0;
}
  
