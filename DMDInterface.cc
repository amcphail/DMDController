#include <stdio.h>

#include <cstdlib>

#include "dmd_file.h"

#include "DMDInterface.h"



/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

DMDInterface::DMDInterface(const char* ip)
{
  dmd = new DMD(ip);
}

DMDInterface::~DMDInterface()
{
  delete dmd;
}

/*-----------------------------------------------------------------------------------------------*/

int DMDInterface::oneTimeInitialiseDMD()
{
  int error;

  // set image type
  error = dmd->SetImageType(IMAGE_TYPE_BINARY);
  if (error) return error;
  // set inum size (1920x1200)
  error = dmd->SetInumSize(IMAGE_ROWS);
  if (error) return error;

  return 0;
}

  
int DMDInterface::setupSingleImage(const char* buf)
{
  int error;

  // set run to false
  if ((error = dmd->SetSequencerState(SEQUENCER_RUN,false))) return error;
  // reset sequencer
  if ((error = dmd->SetSequencerState(SEQUENCER_RESET,true))) return error;

  if ((error = dmd->LoadImageData(0,IMAGE_ROWS*IMAGE_COLS/8,buf))) return error;
  fprintf(stderr,"About to send sequence file.\n");
  if ((error = loadSequence("/home/lab/data/Programming/DMDController/DMDSequences/single_image.txt"))) return error;
  fprintf(stderr,"About to set RESET to false.");
  if ((error = dmd->SetSequencerState(SEQUENCER_RESET,false))) return error;

  return 0;
}

int DMDInterface::setupSwapImage(const char* buf1, const char* buf2)
{
  int error;

  // set run to false
  if ((error = dmd->SetSequencerState(SEQUENCER_RUN,false))) return error;
  // reset sequencer
  if ((error = dmd->SetSequencerState(SEQUENCER_RESET,true))) return error;

  if ((error = dmd->LoadImageData(0,IMAGE_ROWS*IMAGE_COLS/8,buf1))) return error;
  if ((error = dmd->LoadImageData(1,IMAGE_ROWS*IMAGE_COLS/8,buf2))) return error;

  if ((error = loadSequence("/home/lab/data/Programming/DMDController/DMDSequences/swap_images.txt"))) return error;

  if ((error = dmd->SetSequencerState(SEQUENCER_RESET,false))) return error;

  return 0;
}

int DMDInterface::setupMultipleImages(const char* image_map_filename, const char* sequence_filename)
{
  int error;

  // set run to false
  if ((error = dmd->SetSequencerState(SEQUENCER_RUN,false))) return error;
  // reset sequencer
  if ((error = dmd->SetSequencerState(SEQUENCER_RESET,true))) return error;

  int length = 0;
  char* data;

  if ((error = loadFile(image_map_filename,length,data))) 
    {
      if (data) free(data);
      return error;
    }
  
  int num_images = 0;
  ImageMap** image_maps;
  
  if ((error = parseImageMap(num_images,image_maps,length,data))) 
    {
      for (int j = 0; j < num_images; j++) delete image_maps[j];
      delete image_maps;
      return error;
    }

  free(data);
      
  for (int i = 0; i < num_images; i++) {
      if ((error = loadFile(image_maps[i]->filename,length,data))) {
          if (data) free(data);
          return error;
      }
      delete image_maps[i];
      
      if ((error = dmd->LoadImageData(i,IMAGE_ROWS*IMAGE_COLS/8,data))) {
          if (data) free(data);
          return error;
      }
      free(data);
  }
  delete image_maps;
  
  if ((error = loadSequence(sequence_filename))) return error;
  
  if ((error = dmd->SetSequencerState(SEQUENCER_RESET,false))) return error;
  
  return 0;
}

int DMDInterface::runSequencer()
{
  return dmd->SetSequencerState(SEQUENCER_RUN,true);
}

int DMDInterface::pauseSequencer()
{
  return dmd->SetSequencerState(SEQUENCER_RUN,false);
}

int DMDInterface::quit()
{
    return dmd->SetSafeShutdown();
}

int DMDInterface::test()
{
    unsigned short int img_type;

    img_type = dmd->RequestImageType();

    if (img_type == 1 || img_type == 2) return 0;
    else return img_type;
}

/*-----------------------------------------------------------------------------------------------*/

int DMDInterface::loadSequence(const char* fname)
{
  FILE* seq_file;
  unsigned long file_size;
  char* buffer;
  size_t result;

  seq_file = fopen(fname,"rb");

  if (seq_file == NULL) return -1;

  fseek(seq_file,0,SEEK_END);
  file_size = ftell(seq_file);
  rewind(seq_file);

  buffer  = (char*)malloc(sizeof(char)*file_size);

  if (buffer == NULL) return -1;

  result = fread(buffer,1,file_size,seq_file);

  if (result != file_size) return -1;

  fclose(seq_file);

  int error;
  
  // upload the sequence file
  error = dmd->LoadSequenceFile((unsigned short int)file_size,buffer);
  free(buffer);

  ErrorLog* error_log;
  char* log;

  if (error == ERROR_PARSE_SEQ_FILE) 
    {
      // assume since we're here that log is not NULL
      error_log = dmd->RequestSeqFileErrorLog(); 
      log = error_log->getErrorLog();

      fprintf(stderr,log);

      free(log);

      delete error_log;
    }

  return error;
  
}

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
