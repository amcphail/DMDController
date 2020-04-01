#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "dmd.h"

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

void terminate(const char* msg)
{
  perror(msg);
  exit(1);
}

/*-----------------------------------------------------------------------------------------------*/

DMD::DMD(const char* ip)
{
  slen = sizeof(si_other);

  if ( (sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) terminate("DMD::DMD: could not open socket");

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
     
  if ( (img_sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) terminate("DMD::DMD: could not open socket");

  memset((char *) &si_image, 0, sizeof(si_other));
  si_image.sin_family = AF_INET;
  si_image.sin_port = htons(IMG_PORT);
     
  if (inet_aton(ip , &si_other.sin_addr) == 0) terminate("DMD::DMD: inet_aton() failed");

  if (inet_aton(ip , &si_image.sin_addr) == 0) terminate("DMD::DMD: inet_aton() failed");

  error = 0;
}

DMD::~DMD()
{
  close(sock);
  close(img_sock);
}

/*-----------------------------------------------------------------------------------------------*/

int DMD::SetInumSize(unsigned short int rows)
{
  UDPPayload* pl = UDP_Helper::SetInumSize(rows);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

unsigned short int DMD::RequestInumSize()
{
  UDPPayload* pl = UDP_Helper::RequestInumSize();
  InumSize* p = (InumSize*)sendMessage(pl);
  unsigned short int r = p->getRows();
  delete pl;
  delete p;
  return r;
}

int DMD::SetImageType(unsigned short int image_type)
{
  UDPPayload* pl = UDP_Helper::SetImageType(image_type);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

unsigned short int DMD::RequestImageType()
{
  UDPPayload* pl = UDP_Helper::RequestImageType();
  ImageType* p = (ImageType*)sendMessage(pl);
  unsigned short int r = p->getImageType();
  delete pl;
  delete p;
  return r;
}

int DMD::SetFlatnessMaskOn(bool enable)
{ 
  UDPPayload* pl = UDP_Helper::SetFlatnessMaskOn(enable);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

bool DMD::RequestFlatnessMaskOn()
{
  UDPPayload* pl = UDP_Helper::RequestFlatnessMaskOn();
  EnableFCM* p = (EnableFCM*)sendMessage(pl);
  bool r = p->getEnable();
  delete pl;
  delete p;
  return r;
}

int DMD::SetLoadInternalImage(char image_no)
{
  UDPPayload* pl = UDP_Helper::SetLoadInternalImage(image_no);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

char DMD::RequestLoadInternalImage()
{
  UDPPayload* pl = UDP_Helper::RequestLoadInternalImage();
  InternalImage* p = (InternalImage*)sendMessage(pl);
  char r = p->getImageNo();
  delete pl;
  delete p;
  return r;
}

int DMD::SetAutoloadInternalImage(bool autoload,char image_no)
{
  UDPPayload* pl = UDP_Helper::SetAutoloadInternalImage(autoload,image_no);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

AutoloadInternalImage* DMD::RequestAutoloadInternalImage()
{
  UDPPayload* pl = UDP_Helper::RequestAutoloadInternalImage();
  AutoloadInternalImage* p = (AutoloadInternalImage*)sendMessage(pl);
  delete pl;
  return p;
}

int DMD::SetSaveInternalImage(char img_no)
{
    UDPPayload* pl = UDP_Helper::SetSaveInternalImage(img_no);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

int DMD::SetSequencerState(char seq_cmd, bool enable)
{ 
  UDPPayload* pl = UDP_Helper::SetSequencerState(seq_cmd,enable);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

SequencerState* DMD::RequestSequencerState(char seq_cmd)
{
  UDPPayload* pl = UDP_Helper::RequestSequencerState(seq_cmd);
  SequencerState* p = (SequencerState*)sendMessage(pl);
  delete pl;
  return p;
}

ErrorLog* DMD::RequestSeqFileErrorLog()
{
  UDPPayload* pl = UDP_Helper::RequestSeqFileErrorLog();
  ErrorLog* p = (ErrorLog*)sendMessage(pl);
  delete pl;
  return p;
}

int DMD::SetSoftwareSync(bool level)
{ 
  UDPPayload* pl = UDP_Helper::SetSoftwareSync(level);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

bool DMD::RequestSoftwareSync()
{
  UDPPayload* pl = UDP_Helper::RequestSoftwareSync();
  Sync* p = (Sync*)sendMessage(pl);
  bool r = p->getSync();
  delete pl;
  delete p;
  return r;
}

unsigned short int DMD::RequestTriggerCount()
{
  UDPPayload* pl = UDP_Helper::RequestTriggerCount();
  TriggerCount* p = (TriggerCount*)sendMessage(pl);
  unsigned short int r = p->getTriggerCount();
  delete pl;
  delete p;
  return r;
}

int DMD::SetInternalSyncFreq(unsigned short int sec, unsigned short int ms, unsigned short int us)
{
  UDPPayload* pl = UDP_Helper::SetInternalSyncFreq(sec,ms,us);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

InternalSyncFreq* DMD::RequestInternalSyncFreq()
{
  UDPPayload* pl = UDP_Helper::RequestInternalSyncFreq();
  InternalSyncFreq* p = (InternalSyncFreq*)sendMessage(pl);
  delete pl;
  return p;
}

int DMD::SetSequenceReg(unsigned short int reg_no, unsigned short int reg_val)
{
  UDPPayload* pl = UDP_Helper::SetSequenceReg(reg_no,reg_val);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

SequenceReg* DMD::RequestSequenceReg(unsigned short int reg_no)
{
  UDPPayload* pl = UDP_Helper::RequestSequenceReg(reg_no);
  SequenceReg* p = (SequenceReg*)sendMessage(pl);
  delete pl;
  return p;
}

int DMD::SetLightPulseDuration(unsigned short int duration)
{ 
  UDPPayload* pl = UDP_Helper::SetLightPulseDuration(duration);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

unsigned short int DMD::RequestLightPulseDuration()
{
  UDPPayload* pl = UDP_Helper::RequestLightPulseDuration();
  LightPulseDuration* p = (LightPulseDuration*)sendMessage(pl);
  unsigned short int r = p->getDuration();
  delete pl;
  delete p;
  return r;
}

TrigInfo* DMD::RequestTrigInfo()
{
  UDPPayload* pl = UDP_Helper::RequestTrigInfo();
  TrigInfo* p = (TrigInfo*)sendMessage(pl);
  delete pl;
  return p;
}

int DMD::SetTrigDelay(unsigned short int duration)
{ 
  UDPPayload* pl = UDP_Helper::SetTrigDelay(duration);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

unsigned short int DMD::RequestTrigDelay()
{
  UDPPayload* pl = UDP_Helper::RequestTrigDelay();
  TrigDelay* p = (TrigDelay*)sendMessage(pl);
  bool r = p->getTrigDelay();
  delete pl;
  delete p;
  return r;
}

int DMD::SetTrigDivide(unsigned short int factor)
{ 
  UDPPayload* pl = UDP_Helper::SetTrigDivide(factor);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

unsigned short int DMD::RequestTrigDivide()
{
  UDPPayload* pl = UDP_Helper::RequestTrigDivide();
  TrigDivide* p = (TrigDivide*)sendMessage(pl);
  bool r = p->getTrigDivide();
  delete pl;
  delete p;
  return r;
}

int DMD::SetActiveSequence(char type)
{ 
  UDPPayload* pl = UDP_Helper::SetActiveSequence(type);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

char DMD::RequestActiveSequence()
{
  UDPPayload* pl = UDP_Helper::RequestActiveSequence();
  ActiveSequence* p = (ActiveSequence*)sendMessage(pl);
  bool r = p->getActiveSequence();
  delete pl;
  delete p;
  return r;
}

int DMD::SetAutostartSequencer(bool mode)
{ 
  UDPPayload* pl = UDP_Helper::SetAutostartSequencer(mode);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

bool DMD::RequestAutostartSequencer()
{
  UDPPayload* pl = UDP_Helper::RequestAutostartSequencer();
  AutostartSequencer* p = (AutostartSequencer*)sendMessage(pl);
  bool r = p->getAutostartSequencer();
  delete pl;
  delete p;
  return r;
}

int DMD::SetDmdMirrorShake(bool enable)
{ 
  UDPPayload* pl = UDP_Helper::SetDmdMirrorShake(enable);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

bool DMD::RequestDmdMirrorShake()
{
  UDPPayload* pl = UDP_Helper::RequestDmdMirrorShake();
  DmdMirrorShake* p = (DmdMirrorShake*)sendMessage(pl);
  bool r = p->getDmdMirrorShake();
  delete pl;
  delete p;
  return r;
}

int DMD::SetTrigSource(bool optical, bool negative)
{
  UDPPayload* pl = UDP_Helper::SetTrigSource(optical,negative);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

TrigSource* DMD::RequestTrigSource()
{
  UDPPayload* pl = UDP_Helper::RequestTrigSource();
  TrigSource* p = (TrigSource*)sendMessage(pl);
  delete pl;
  return p;
}

int DMD::SetIllumLed(char illum)
{ 
  UDPPayload* pl = UDP_Helper::SetIllumLed(illum);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

char DMD::RequestIllumLed()
{
  UDPPayload* pl = UDP_Helper::RequestIllumLed();
  IllumLed* p = (IllumLed*)sendMessage(pl);
  char r = p->getIllumVal();
  delete pl;
  delete p;
  return r;
}

int DMD::SetNetworkSettings(unsigned long int ip_addr, unsigned long int subnet, unsigned long int gateway, bool dhcp)
{
  UDPPayload* pl = UDP_Helper::SetNetworkSettings(ip_addr,subnet,gateway,dhcp);
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

NetworkSettings* DMD::RequestNetworkSettings()
{
  UDPPayload* pl = UDP_Helper::RequestNetworkSettings();
  NetworkSettings* p = (NetworkSettings*)sendMessage(pl);
  delete pl;
  return p;
}

int DMD::SaveSettings()
{ 
  UDPPayload* pl = UDP_Helper::SaveSettings();
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

int DMD::SetFactoryDefaults()
{ 
  UDPPayload* pl = UDP_Helper::SetFactoryDefaults();
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

int DMD::SetSafeShutdown()
{ 
  UDPPayload* pl = UDP_Helper::SetSafeShutdown();
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}

FwVersion* DMD::RequestFwVersion()
{
  UDPPayload* pl = UDP_Helper::RequestFwVersion();
  FwVersion* p = (FwVersion*)sendMessage(pl);
  delete pl;
  return p;
}

DmdInfo* DMD::RequestDmdInfo()
{
  UDPPayload* pl = UDP_Helper::RequestDmdInfo();
  DmdInfo* p = (DmdInfo*)sendMessage(pl);
  delete pl;
  return p;
}

unsigned long int DMD::RequestErrorStatus()
{
  UDPPayload* pl = UDP_Helper::RequestErrorStatus();
  ErrorStatus* p = (ErrorStatus*)sendMessage(pl);
  unsigned long int r = p->getErrorStatus();
  delete pl;
  delete p;
  return r;
}

int DMD::ClearErrorStatus()
{ 
  UDPPayload* pl = UDP_Helper::SetClearErrorStatus();
  int error = sendAckedMessage(pl);
  delete pl;
  return error;
}
/*-----------------------------------------------------------------------------------------------*/

int DMD::LoadImageData(unsigned long int inum, size_t ln, const char* img)
{
  int pln = 0;
  
  UDPPayload* uack = UDP_Helper::ResetSeqNo();
  int error = sendAckedMessage(uack);

  delete uack;

  if (error) 
    {
      return error;
    }
  
  UDPPayload** payloads = UDP_Helper::LoadImageData(inum,pln,ln,img);

  int i;
  for (i = 0; i < pln; i++) {
    payloads[i]->toBytes(send_buf);
    
    //send the message
    if (sendto(img_sock, send_buf, payloads[i]->getSize() , 0 , (struct sockaddr *) &si_image, slen)==-1)
      {
	//terminate("UDPPayload::sendMessage: sendto() failed");

	for (int j = 0; j < pln; j++) delete payloads[j];
	delete payloads;
	return -1;
      }  
  }

  SeqNoError* snerror = (SeqNoError*)this->sendMessage(UDP_Helper::RequestSeqNoError());
  unsigned short int last_seq_no;
  
  while (snerror->getIsError())
    {
      last_seq_no = snerror->getLastSeqNo();

      for (i = last_seq_no; i < pln; i++) {
	payloads[i]->toBytes(send_buf);
         
	//send the message
	if (sendto(img_sock, send_buf, payloads[i]->getSize() , 0 , (struct sockaddr *) &si_image, slen)==-1)
	  {
	    //terminate("UDPPayload::sendMessage: sendto() failed");
	    
	    for (int j = 0; j < pln; j++) delete payloads[j];
	    delete payloads;
	    return -1;
	  }  
      }

      delete snerror;
      snerror = (SeqNoError*)this->sendMessage(UDP_Helper::RequestSeqNoError());
    }
      
  for (int j = 0; j < pln; j++) delete payloads[j];
  delete payloads;
 
  bool is_error = snerror->getIsError();

  delete snerror;

  return (is_error ? (int)(-1) : 0);
}  

int DMD::LoadFlatnessCorrectionMask(unsigned short int data_size,const char* data)
{
  int pls = 0;
  
  UDPPayload** payloads = UDP_Helper::LoadFlatnessCorrectionMask(data_size,pls,data);

  Ack* response = (Ack*)sendCompoundMessage(pls,payloads);

  for (int j = 0; j < pls; j++) delete payloads[j];
  delete payloads;
  
  if (response)
    {
      error = response->getErrorCode();
      delete response;
      return error;
    }
  else 
    {
      return -1;
    }
}

int DMD::LoadSequenceFile(unsigned short int data_size,const char* data)
{
  int pls = 0;
  
  UDPPayload** payloads = UDP_Helper::LoadSequenceFile(data_size,pls,data);

  Ack* response = (Ack*)sendCompoundMessage(pls,payloads);

  for (int j = 0; j < pls; j++) delete payloads[j];
  delete payloads;
  
  if (response)
    {
      error = response->getErrorCode();
      delete response;
      return error;
    }
  else 
    {
      return -1;
    }
}

/*-----------------------------------------------------------------------------------------------*/

Payload* DMD::sendMessage(UDPPayload* message)
{
  UDPPayload* response;
  
  message->toBytes(send_buf);
         
  //send the message
  if (sendto(sock, send_buf, message->getSize() , 0 , (struct sockaddr *) &si_other, slen)==-1)
    //terminate("UDPPayload::sendMessage: sendto() failed");
    return 0;

  //receive a reply and print it
  //clear the buffer by filling null, it might have previously received data
  //memset(buf,'\0', BUFLEN);
  //try to receive some data, this is a blocking call
  if (recvfrom(sock, recv_buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
    //terminate("UDPPayload::sendMessage: recvfrom() failed");
    return 0;

  response = new UDPPayload(recv_buf);

  Payload* pl = response->getPayload()->clone();

  delete response;

  return pl;
}

Payload* DMD::sendCompoundMessage(int num, UDPPayload** messages)
{
  UDPPayload* response = 0;
  Ack* ack = 0;
  
  int i;
  for (i = 0; i < num; i++) {
    messages[i]->toBytes(send_buf);
         
    //send the message
    if (sendto(sock, send_buf, messages[i]->getSize() , 0 , (struct sockaddr *) &si_other, slen)==-1)
      //terminate("UDPPayload::sendMessage: sendto() failed");
      return 0;
    
    //receive a reply and print it
    //clear the buffer by filling null, it might have previously received data
    //memset(buf,'\0', BUFLEN);
    //try to receive some data, this is a blocking call
    if (recvfrom(sock, recv_buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
      //terminate("UDPPayload::sendMessage: recvfrom() failed");
      return 0;

    response = new UDPPayload(recv_buf);

    ack = (Ack*)response->getPayload();

    error = ack->getErrorCode();
    if (error) return 0;

  }
  if (ack) return ack;

  return 0;
}

int DMD::sendAckedMessage(UDPPayload* pl)
{
  Ack* response = (Ack*)this->sendMessage(pl);

  if (response)
    {
      error = response->getErrorCode();
      delete response;
      return error;
    }
  else return -1;
}

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
