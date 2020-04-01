#ifndef __AVHM__DMD_INTERFACE_H__
#define __AVHM__DMD_INTERFACE_H__

#include "dmd.h"

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

#define IMAGE_COLS 1920
//#define IMAGE_ROWS 1080
#define IMAGE_ROWS 1200

//#define IMAGE_COLS 32
//#define IMAGE_ROWS 32

/*-----------------------------------------------------------------------------------------------*/

/* ASSUMES IMAGES ARE 1920x1080 */
/*  12/6/2018 LOOKS LIKE THEY'RE 1920x1200 */

/*-----------------------------------------------------------------------------------------------*/

class DMDInterface
{
 public:
  
  DMDInterface(const char* ip = SERVER);

  ~DMDInterface();

  int oneTimeInitialiseDMD();

  inline unsigned long int getError() { return dmd->getError(); }
  
  int setupSingleImage(const char*);
  int setupSwapImage(const char*, const char*);
  int setupMultipleImages(const char*, const char*);

  int runSequencer();
  int pauseSequencer();

  int quit();

  int test();

 protected:

  int loadSequence(const char*);

 private:

  DMD* dmd;

};

  
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/



#endif
