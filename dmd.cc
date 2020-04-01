#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <time.h>

#include <QEventLoop>
#include <QByteArray>
#include <QUdpSocket>

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
    socket = new QUdpSocket(this);
    image_socket = new QUdpSocket(this);

    address = QString(ip);

    port = PORT;
    image_port = IMG_PORT;

    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

//    connect(image_socket,SIGNAL(connected()),this,SLOT(image_connected()));

    socket->connectToHost(QHostAddress(address),port);

    connect(this,SIGNAL(messageReceived(QByteArray)),this,SLOT(processMessage(QByteArray)));

    error = 0;
}

DMD::~DMD()
{
    socket->close();

    delete image_socket;
    delete socket;
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

int DMD::LoadImageData(unsigned long int inum, size_t ln, const char* buf)
{
  int pln = 0;

  struct timespec tim, tim2;
  tim.tv_sec = 0;
  tim.tv_nsec = 5000;

  UDPPayload* uack = UDP_Helper::ResetSeqNo();
  int error = sendAckedMessage(uack);

  delete uack;

  if (error) return error;

  QByteArray data(buf,ln);

  UDPPayload** payloads = UDP_Helper::LoadImageData(inum,pln,ln,data);

  fprintf(stderr,"Image slot: %d, total payloads: %d, length %d\n",inum,pln,ln);

  int bytesSent = -1;

  int i;
  for (i = 0; i < pln; i++) {

    data.clear();

    payloads[i]->toBytes(data);
    
    //send the message
    bytesSent = image_socket->writeDatagram(data,QHostAddress(address),image_port);

    nanosleep(&tim,&tim2);

    if (bytesSent == -1) {
        for (int j = 0; j < pln; j++) delete payloads[j];
        delete payloads;
        return -1;
    }
    fprintf(stderr,"Sent packet: %d, of %d bytes\n",i,bytesSent);
  }

  fprintf(stderr,"About to request sequence number.\n");
  SeqNoError* snerror = (SeqNoError*)this->sendMessage(UDP_Helper::RequestSeqNoError());
  unsigned short int last_seq_no;
  
  while (snerror->getIsError()) {
      last_seq_no = snerror->getLastSeqNo();

      fprintf(stderr,"Last sequence number: %d\n",last_seq_no);

      for (i = last_seq_no; i < pln; i++) {
        data.clear();

        payloads[i]->toBytes(data);

        //send the message
        bytesSent = image_socket->writeDatagram(data,QHostAddress(address),image_port);

        if (bytesSent == -1){
            for (int j = 0; j < pln; j++) delete payloads[j];
            delete payloads;
            return -1;
        }
      }

      fprintf(stderr,"Resent packet: %d\n",i);

      delete snerror;
      snerror = (SeqNoError*)this->sendMessage(UDP_Helper::RequestSeqNoError());
  }
      
  for (int j = 0; j < pln; j++) delete payloads[j];
  delete payloads;
 
  fprintf(stderr,"About to get error.\n");
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

void DMD::connected()
{
    fprintf(stderr,"DMD::connected() fired\n");
}

void DMD::readyRead()
{
    QByteArray received;

    fprintf(stderr,"Entered DMD::readyRead()\n");

    received.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    fprintf(stderr,"Receive Size: %d\n",received.size());

    socket->readDatagram(received.data(),received.size(),
                          &sender, &senderPort);

    emit messageReceived(received);
}

void DMD::image_connected()
{
    fprintf(stderr,"DMD::image_connected() fired\n");
}

void DMD::processMessage(QByteArray data)
{
    fprintf(stderr,"Message received.\n");

    recv_data = data;
}

/*-----------------------------------------------------------------------------------------------*/

Payload* DMD::sendMessage(UDPPayload* message)
{
  UDPPayload* response;
  
  QByteArray send_data;

  send_data.clear();

  message->toBytes(send_data);

  fprintf(stderr,"send_data: %hu:%hu:%hu:%hu\n",send_data[0],send_data[1],send_data[2],send_data[3]);

  qint64 bytes_written;

  bytes_written = socket->write(send_data);

  fprintf(stderr,"Wrote %d bytes\n",bytes_written);

  QEventLoop loop;
  loop.connect(this,SIGNAL(messageReceived(QByteArray)),SLOT(quit()));
  fprintf(stderr,"About to wait for response...\n");
  loop.exec();

  response = new UDPPayload(recv_data);

  Payload* pl = response->getPayload()->clone();

  delete response;

  return pl;
}

Payload* DMD::sendCompoundMessage(int num, UDPPayload** messages)
{
  UDPPayload* response = 0;
  Ack* ack = 0;

  QByteArray send_data;

  qint64 bytes;

  QEventLoop loop;

  int i;
  for (i = 0; i < num; i++) {

    send_data.clear();
    messages[i]->toBytes(send_data);
         
    socket->write(send_data);

    recv_data.clear();

    loop.connect(this,SIGNAL(messageReceived(QByteArray)),SLOT(quit()));
    loop.exec();

    response = new UDPPayload(recv_data);

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
