#ifndef __AVHM__DMD_H__
#define __AVHM__DMD_H__

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <QUdpSocket>

#include "dmd_udp.h"

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

class DMD
{
 public:
  
  DMD(const char* ip = SERVER);

  ~DMD();

  inline unsigned long int getError() { return error; }
  
  int SetInumSize(unsigned short int);
  unsigned short int RequestInumSize();

  int SetImageType(unsigned short int);
  unsigned short int RequestImageType();

  int SetFlatnessMaskOn(bool);
  bool RequestFlatnessMaskOn();

  int SetLoadInternalImage(char);
  char RequestLoadInternalImage();
  
  int SetAutoloadInternalImage(bool,char);
  AutoloadInternalImage* RequestAutoloadInternalImage();
  
  int SetSaveInternalImage(char);

  int SetSequencerState(char,bool);
  SequencerState* RequestSequencerState(char);
  
  ErrorLog* RequestSeqFileErrorLog();

  int SetSoftwareSync(bool);
  bool RequestSoftwareSync();

  unsigned short int RequestTriggerCount();
  
  int SetInternalSyncFreq(unsigned short int, unsigned short int, unsigned short int);
  InternalSyncFreq* RequestInternalSyncFreq();
  
  int SetSequenceReg(unsigned short int,unsigned short int);
  SequenceReg* RequestSequenceReg(unsigned short int);

  int SetLightPulseDuration(unsigned short int);
  unsigned short int RequestLightPulseDuration();

  TrigInfo* RequestTrigInfo();

  int SetTrigDelay(unsigned short int);
  unsigned short int RequestTrigDelay();
  
  int SetTrigDivide(unsigned short int);
  unsigned short int RequestTrigDivide();
  
  int SetActiveSequence(char);
  char RequestActiveSequence();
  
  int SetAutostartSequencer(bool);
  bool RequestAutostartSequencer();
  
  int SetDmdMirrorShake(bool);
  bool RequestDmdMirrorShake();
 
  int SetTrigSource(bool,bool);
  TrigSource* RequestTrigSource();
  
  int SetIllumLed(char);
  char RequestIllumLed();

  int SetNetworkSettings(unsigned long int,unsigned long int,unsigned long int,bool);
  NetworkSettings* RequestNetworkSettings();
  
  int SaveSettings();
  int SetFactoryDefaults();
  int SetSafeShutdown();
  
  FwVersion* RequestFwVersion();
  DmdInfo* RequestDmdInfo();

  unsigned long int RequestErrorStatus();
  int ClearErrorStatus();

  int LoadImageData(unsigned long int,size_t,const char*);

  int LoadFlatnessCorrectionMask(unsigned short int,const char*);

  int LoadSequenceFile(unsigned short int,const char*);
  
 protected:

  Payload* sendMessage(UDPPayload*);
  Payload* sendCompoundMessage(int, UDPPayload**);

  int sendAckedMessage(UDPPayload*);

  QString address;
  int port;
  int image_port;

  QUdpSocket* socket;
  QUdpSocket* image_socket;

  unsigned long int error;
};

  
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/



#endif
