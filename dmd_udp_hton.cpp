#include <cstring>

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include <QMessageBox>

#include "dmd_udp.h"

/*-----------------------------------------------------------------------------------------------*/

long int char_to_int32_be(char const* buf)
{
  //  int num = (chars[0] << 24) + (chars[1] << 16) + (chars[2] << 8) + chars[3];
  const long int* p_int = reinterpret_cast<const long int*>(buf);
  //  int* p_int = (int*)buf;
  long int num = *p_int;
  
  return num;
}

unsigned long int char_to_uint32_be(char const* buf)
{
  //  unsigned int num = (chars[0] << 24) + (chars[1] << 16) + (chars[2] << 8) + chars[3];
  const unsigned long int* p_int = reinterpret_cast<const unsigned long int*>(buf);
  //  unsigned int* p_int = (unsigned int*)buf;
  unsigned long int num = ntohl(*p_int);
  
  return num;
}

short int char_to_int16_be(char const* buf)
{
  const short int* p_int = reinterpret_cast<const short int*>(buf);
  short int num = *p_int;
  
  return num;
}

unsigned short int char_to_uint16_be(char const* buf)
{
  const unsigned short int* p_int = reinterpret_cast<const unsigned short int*>(buf);
  unsigned short int num = ntohs(*p_int);
  
  return num;
}

/*-----------------------------------------------------------------------------------------------*/

UDPPayload::UDPPayload(const char* buf)
{
  tot_size = char_to_uint16_be(buf);
  rec_id   = char_to_uint16_be(&buf[2]);

  switch (rec_id)
    {
    case REPLY_ACK:
      payload = new Ack(&buf[4]);
      break;
    case REPLY_INUM_SIZE:
      payload = new InumSize(&buf[4]);
      break;
    case REPLY_IMAGE_TYPE:
      payload = new ImageType(&buf[4]);
      break;
    case REPLY_SEQ_NO_ERROR:
      payload = new SeqNoError(&buf[4]);
      break;
    case REPLY_FLAT_MASK_ON:
      payload = new EnableFCM(&buf[4]);
      break;
    case REPLY_LOAD_INT_IMG:
      payload = new InternalImage(&buf[4]);
      break;
    case REPLY_AUTO_INT_IMG:
      payload = new AutoloadInternalImage(&buf[4]);
      break;
    case REPLY_SEQ_STATE:
      payload = new SequencerState(&buf[4]);
      break;
    case REPLY_SEQ_ERR_LOG:
      payload = new ErrorLog(&buf[4]);
      break;
    case REPLY_SOFT_SYNC:
      payload = new Sync(&buf[4]);
      break;
    case REPLY_TRIGGER_CNT:
      payload = new TriggerCount(&buf[4]);
      break;
    case REPLY_INT_SYNC_FRQ:
      payload = new InternalSyncFreq(&buf[4]);
      break;
    case REPLY_SEQ_REG:
      payload = new SequenceReg(&buf[4]);
      break;
    case REPLY_LIGHT_PLS_DR:
      payload = new LightPulseDuration(&buf[4]);
      break;
    case REPLY_TRIG_INFO:
      payload = new TrigInfo(&buf[4]);
      break;
    case REPLY_TRIG_DELAY:
      payload = new TrigDelay(&buf[4]);
      break;
    case REPLY_TRIG_DIVIDE:
      payload = new TrigDivide(&buf[4]);
      break;
    case REPLY_ACTIVE_SEQ:
      payload = new ActiveSequence(&buf[4]);
      break;
    case REPLY_AUTO_START:
      payload = new AutostartSequencer(&buf[4]);
      break;
    case REPLY_DMD_MIR_SHK:
      payload = new DmdMirrorShake(&buf[4]);
      break;
    case REPLY_TRIG_SOURCE:
      payload = new TrigSource(&buf[4]);
      break;
    case REPLY_ILLUM_LED:
      payload = new IllumLed(&buf[4]);
      break;
    case REPLY_NETWORK_SET:
      payload = new NetworkSettings(&buf[4]);
      break;
    case REPLY_FW_VERSION:
      payload = new FwVersion(&buf[4]);
      break;
    case REPLY_DMD_INFO:
      payload = new DmdInfo(&buf[4]);
      break;
    case REPLY_ERROR_STATUS:
      payload = new ErrorStatus(&buf[4]);
      break;
    //----------------------------------------------------------------------------------
    case LOAD_IMAGE_DATA:
      payload = new ImageData(tot_size-14,&buf[4]);
      break;
    default:
      payload = 0;
      fprintf(stderr,"UDPPayload::UDPPayload(const char*): did not recognise record id\n");
    }
}

UDPPayload::UDPPayload(unsigned short int ri, Payload* pl)
{
  tot_size = 4;
  if (pl) tot_size += pl->size();
  
  rec_id   = ri;

  if (pl) payload = pl->clone();
  else payload = 0;
}

UDPPayload::UDPPayload(const UDPPayload& pl)
{
  tot_size = pl.tot_size;
  rec_id   = pl.rec_id;

  if (pl.payload) payload = pl.payload->clone();
}

UDPPayload::~UDPPayload()
{
  if (payload) delete payload;
}

void UDPPayload::toBytes(char* buf)
{
  unsigned short int val;

  val = htons(tot_size);
  memcpy(buf,&val,2);

  val = htons(rec_id);
  memcpy(&buf[2],&val,2);

  QMessageBox msg;

  msg.setText(QString::number(val));
  msg.exec();

  msg.setText(buf);
  msg.exec();



  if (payload) payload->toBytes(&buf[4]);
}

/*-----------------------------------------------------------------------------------------------*/

Char::Char(const char* buf)
{
  char_value = buf[0];
}

Char::Char(const Char& ra)
{
  char_value = ra.char_value;
}

void Char::toBytes(char* buf)
{
  buf[0] = char_value;
}

/*-----------------------------------------------------------------------------------------------*/

UnsignedShortInt::UnsignedShortInt(const char* buf)
{
  unsigned_short_int = char_to_uint16_be(buf);
}

UnsignedShortInt::UnsignedShortInt(const UnsignedShortInt& ra)
{
  unsigned_short_int = ra.unsigned_short_int;
}

void UnsignedShortInt::toBytes(char* buf)
{
  unsigned short int val;

  val = htons(unsigned_short_int);
  memcpy(buf,&val,2);
}

/*-----------------------------------------------------------------------------------------------*/

UnsignedLongInt::UnsignedLongInt(const char* buf)
{
  unsigned_long_int = char_to_uint32_be(buf);
}

UnsignedLongInt::UnsignedLongInt(const UnsignedLongInt& ra)
{
  unsigned_long_int = ra.unsigned_long_int;
}

void UnsignedLongInt::toBytes(char* buf)
{
  unsigned long int val;

  val = htons(unsigned_long_int);
  memcpy(buf,&val,4);
}

/*-----------------------------------------------------------------------------------------------*/

ImageData::ImageData(const int len, const char* buf)
{
  seq_no = char_to_uint16_be(buf);
  inum = char_to_uint32_be(&buf[2]);
  offset = char_to_uint32_be(&buf[6]);
  length = len;

  data = new char[len];
  memcpy(data,&buf[10],len);
}

ImageData::ImageData(const unsigned short int sn
		     ,const unsigned long int in
		     ,const unsigned long int os
		     ,const size_t ln
		     ,const char* dt)
{
  seq_no = sn;
  inum = in;
  offset = os;
  length = ln;

  data = new char[length];
  memcpy(data,dt,length);
}

ImageData::ImageData(const ImageData& id)
{
  seq_no = id.seq_no;
  inum = id.inum;
  offset = id.offset;
  length = id.length;

  data = new char[length];
  memcpy(data,id.data,length);
}

ImageData::~ImageData()
{
  delete[] data;
}

int ImageData::getData(size_t& len, char*& buf)
{
  len = length;
  buf = (char*)malloc(length);

  memcpy(buf,data,length);
  
  return 0;
}

void ImageData::toBytes(char* buf)
{
  unsigned short int sval;
  unsigned long int lval;

  sval = htons(seq_no);
  memcpy(buf,&sval,2);

  lval = htonl(inum);
  memcpy(&buf[2],&lval,4);
  
  lval = htonl(offset);
  memcpy(&buf[6],&lval,4);

  memcpy(&buf[10],data,length);
}

/*-----------------------------------------------------------------------------------------------*/

SeqNoError::SeqNoError(const char* buf)
{
  is_error = buf[0];
  
  last_seq_no = char_to_uint16_be(&buf[1]);
}

SeqNoError::SeqNoError(const SeqNoError& sne)
{
  is_error = sne.is_error;
  last_seq_no = sne.is_error;
}

SeqNoError::~SeqNoError() {}

void SeqNoError::toBytes(char* buf)
{
  unsigned short int sval;
  
  buf[0] = is_error;
  
  sval = htons(last_seq_no);
  memcpy(&buf[1],&sval,2);
}

/*-----------------------------------------------------------------------------------------------*/

FlatnessCorrectionMask::FlatnessCorrectionMask(const unsigned short int pn
					       ,const unsigned short int tp
					       ,const unsigned short int ds
					       ,const char* dt)
{
  pkg_no = pn;
  tot_pkg = tp;
  data_size = ds;

  data = new char[data_size];
  memcpy(data,dt,data_size);
}

FlatnessCorrectionMask::FlatnessCorrectionMask(const FlatnessCorrectionMask& fcm)
{
  pkg_no = fcm.pkg_no;
  tot_pkg = fcm.tot_pkg;
  data_size = fcm.data_size;

  data = new char[data_size];
  memcpy(data,fcm.data,data_size);
}

FlatnessCorrectionMask::~FlatnessCorrectionMask()
{
  delete[] data;
}

void FlatnessCorrectionMask::toBytes(char* buf)
{
  unsigned short int sval;

  sval = htons(pkg_no);
  memcpy(buf,&sval,2);

  sval = htons(tot_pkg);
  memcpy(&buf[2],&sval,2);
  
  sval = htonl(data_size);
  memcpy(&buf[4],&sval,2);

  memcpy(&buf[6],data,data_size);
}

/*-----------------------------------------------------------------------------------------------*/

AutoloadInternalImage::AutoloadInternalImage(const char* buf)
{
  autoload = buf[0];
  img_no = buf[1];
}

AutoloadInternalImage::AutoloadInternalImage(const AutoloadInternalImage& ail)
{
  autoload = ail.autoload;
  img_no = ail.img_no;
}

AutoloadInternalImage::~AutoloadInternalImage() {}

void AutoloadInternalImage::toBytes(char* buf)
{
  autoload = buf[0];
  img_no = buf[1];
}

/*-----------------------------------------------------------------------------------------------*/

SequenceFile::SequenceFile(const unsigned short int pn
				   ,const unsigned short int tp
				   ,const unsigned short int ds
		     ,const char* dt)
{
  pkg_no = pn;
  tot_pkg = tp;
  data_size = ds;

  data = new char[data_size];
  memcpy(data,dt,data_size);
}

SequenceFile::SequenceFile(const SequenceFile& lsf)
{
  pkg_no = lsf.pkg_no;
  tot_pkg = lsf.tot_pkg;
  data_size = lsf.data_size;

  data = new char[data_size];
  memcpy(data,lsf.data,data_size);
}

SequenceFile::~SequenceFile()
{
  delete[] data;
}

void SequenceFile::toBytes(char* buf)
{
  unsigned short int sval;

  sval = htons(pkg_no);
  memcpy(buf,&sval,2);

  sval = htons(tot_pkg);
  memcpy(&buf[2],&sval,2);
  
  sval = htons(data_size);
  memcpy(&buf[4],&sval,2);

  memcpy(&buf[6],data,data_size);
}

/*-----------------------------------------------------------------------------------------------*/

SequencerState::SequencerState(const char* buf)
{
  seq_cmd = buf[0];
  enable = buf[1];
  valid_cmd = buf[2];
}

SequencerState::SequencerState(const SequencerState& ss)
{
  seq_cmd = ss.seq_cmd;
  enable = ss.enable;
  valid_cmd = ss.valid_cmd;
}

SequencerState::~SequencerState()
{
}

void SequencerState::toBytes(char* buf)
{
  buf[0] = seq_cmd;
  if (enable < 2)
    {
      buf[1] = enable;
      if (valid_cmd < 2) buf[2] = valid_cmd;
    }
}

/*-----------------------------------------------------------------------------------------------*/

ErrorLog::ErrorLog(const char* buf)
{
  data_size = char_to_uint16_be(buf);

  data = new char[data_size];
  memcpy(data,&buf[2],data_size);
}

ErrorLog::ErrorLog(const unsigned short int ds
		   ,const char* dt)
{
  data_size = ds;

  data = new char[data_size];
  memcpy(data,dt,data_size);
}

ErrorLog::ErrorLog(const ErrorLog& el)
{
  data_size = el.data_size;

  data = new char[data_size];
  memcpy(data,el.data,data_size);
}

ErrorLog::~ErrorLog()
{
  delete[] data;
}

char* ErrorLog::getErrorLog()
{
  char* error_log = (char*)malloc(sizeof(char)*data_size);
  memcpy(error_log,data,data_size);

  return error_log;
}

void ErrorLog::toBytes(char* buf)
{
  unsigned short int sval;

  sval = htons(data_size);
  memcpy(buf,&sval,2);

  memcpy(&buf[2],data,data_size);
}

/*-----------------------------------------------------------------------------------------------*/

InternalSyncFreq::InternalSyncFreq(const char* buf)
{
  sec = char_to_uint16_be(buf);
  ms  = char_to_uint16_be(&buf[2]);
  us  = char_to_uint16_be(&buf[4]);
}

void InternalSyncFreq::toBytes(char* buf)
{
  unsigned short int sval;

  sval = htons(sec);
  memcpy(buf,&sval,2);

  sval = htons(ms);
  memcpy(&buf[2],&sval,2);
  
  sval = htons(us);
  memcpy(&buf[4],&sval,2);
}

/*-----------------------------------------------------------------------------------------------*/

SequenceReg::SequenceReg(const char* buf)
{
  reg_no = char_to_uint16_be(buf);
  reg_val = char_to_uint16_be(&buf[2]);
  valid_reg  = buf[5];
}

void SequenceReg::toBytes(char* buf)
{
  unsigned short int sval;

  sval = htons(reg_no);
  memcpy(buf,&sval,2);

  if (reg_val != 65535)
    {
      sval = htons(reg_val);
      memcpy(&buf[2],&sval,2);

      if (valid_reg < 2) buf[4] = valid_reg;
    }
}

/*-----------------------------------------------------------------------------------------------*/

TrigInfo::TrigInfo(const char* buf)
{
  in_puls_e = char_to_uint32_be(buf);
  in_puls_o = char_to_uint32_be(&buf[4]);
  min_per_e = char_to_uint16_be(&buf[8]);
  min_per_o = char_to_uint16_be(&buf[10]);
  seq_puls = char_to_uint16_be(&buf[12]);
}


TrigInfo::TrigInfo(const TrigInfo& ti)
{
  in_puls_e = ti.in_puls_e;
  in_puls_o = ti.in_puls_o;
  min_per_e = ti.min_per_e;
  min_per_o = ti.min_per_o;
  seq_puls = ti.seq_puls;
  
}

void TrigInfo::toBytes(char* buf)
{
  unsigned long int lval;
  unsigned short int sval;

  lval = htonl(in_puls_e);
  memcpy(buf,&lval,4);
  
  lval = htonl(in_puls_o);
  memcpy(&buf[4],&lval,4);
  
  sval = htons(min_per_e);
  memcpy(&buf[8],&sval,2);

  sval = htons(min_per_o);
  memcpy(&buf[10],&sval,2);

  sval = htons(seq_puls);
  memcpy(&buf[12],&sval,2);
}

/*-----------------------------------------------------------------------------------------------*/

TrigSource::TrigSource(const char* buf)
{
  source = char_to_uint16_be(buf);
  edge = char_to_uint16_be(&buf[2]);
}

TrigSource::TrigSource(const bool sr, const bool ed)
{
  source = sr ? 1 : 0;
  edge = ed ? 1 : 0;
}

TrigSource::TrigSource(const TrigSource& ts)
{
  source = ts.source;
  edge = ts.edge;
}

void TrigSource::toBytes(char* buf)
{
  unsigned short int sval;
  
  sval = htons(source);
  memcpy(buf,&sval,2);
  
  sval = htons(edge);
  memcpy(&buf[2],&sval,2);
}

/*-----------------------------------------------------------------------------------------------*/

NetworkSettings::NetworkSettings(const char* buf)
{
  ip_addr = char_to_uint32_be(buf);
  subnet = char_to_uint32_be(&buf[4]);
  gateway = char_to_uint32_be(&buf[8]);

  dhcp = buf[12];

}

NetworkSettings::NetworkSettings(const unsigned long int ip,const unsigned long int sn
				 ,const unsigned long int gw,const bool d)
{
  ip_addr = ip;
  subnet = sn;
  gateway = gw;

  dhcp = d ? 1 : 0;
}

NetworkSettings::NetworkSettings(const NetworkSettings& ns)
{
  ip_addr = ns.ip_addr;
  subnet = ns.subnet;
  gateway = ns.gateway;

  dhcp = ns.dhcp;
}

NetworkSettings::~NetworkSettings()
{
}

void NetworkSettings::toBytes(char* buf)
{
  unsigned long int lval;

  lval = htonl(ip_addr);
  memcpy(buf,&lval,4);

  lval = htonl(subnet);
  memcpy(&buf[4],&lval,4);

  lval = htonl(gateway);
  memcpy(&buf[8],&lval,4);

  buf[13] = dhcp;
}

/*-----------------------------------------------------------------------------------------------*/

FwVersion::FwVersion(const char* buf)
{
 sys_fw_major = char_to_uint16_be(buf);
 sys_fw_minor = char_to_uint16_be(&buf[2]);
 app_sw_ver = char_to_uint16_be(&buf[4]);
 app_sw_year = char_to_uint16_be(&buf[6]);
 app_sw_month = buf[8];
 app_sw_day = buf[9];
 app_sw_hour = buf[10];
 app_sw_min = buf[11];
 app_sw_sec = buf[12];
 FPGA_year = char_to_uint16_be(&buf[13]);
 FPGA_month = buf[15];
 FPGA_day = buf[16];
 FPGA_hour = buf[17];
 FPGA_min = buf[18];
 FPGA_sec = buf[19];
}

FwVersion::FwVersion(const FwVersion& fv)
{
  sys_fw_major = fv.sys_fw_major;
  sys_fw_minor = fv.sys_fw_minor;
  app_sw_ver = fv.app_sw_ver;
  app_sw_year = fv.app_sw_year;
  app_sw_month = fv.app_sw_month;
  app_sw_day = fv.app_sw_day;
  app_sw_hour = fv.app_sw_hour;
  app_sw_min = fv.app_sw_min;
  app_sw_sec = fv.app_sw_sec;
  FPGA_year = fv.FPGA_year;
  FPGA_month = fv.FPGA_month;
  FPGA_day = fv.FPGA_day;
  FPGA_hour = fv.FPGA_hour;
  FPGA_min = fv.FPGA_min;
  FPGA_sec = fv.FPGA_sec;
} 

void FwVersion::toBytes(char* buf)
{
  unsigned short int sval;
  
  sval = htons(sys_fw_major);
  memcpy(buf,&sval,2);

  sval = htons(sys_fw_minor);
  memcpy(&buf[2],&sval,2);

  sval = htons(app_sw_ver);
  memcpy(&buf[4],&sval,2);

  sval = htons(app_sw_year);
  memcpy(&buf[6],&sval,2);

 buf[8] = app_sw_month;
 buf[9] = app_sw_day;
 buf[10] = app_sw_hour;
 buf[11] = app_sw_min;
 buf[12] = app_sw_sec;
 
 sval = htons(FPGA_year);
 memcpy(&buf[13],&sval,2);

 buf[15] = FPGA_month;
 buf[16] = FPGA_day;
 buf[17] = FPGA_hour;
 buf[18] = FPGA_min;
 buf[19] = FPGA_sec;
}

/*-----------------------------------------------------------------------------------------------*/

DmdInfo::DmdInfo(const char* buf)
{
 dmd_type = char_to_uint16_be(buf);
 blocks = char_to_uint16_be(&buf[2]);
 cols = char_to_uint16_be(&buf[4]);
 rows = char_to_uint16_be(&buf[6]);
 rows_pr_block = char_to_uint16_be(&buf[8]);
}

DmdInfo::DmdInfo(const DmdInfo& di)
{
  dmd_type = di.dmd_type;
  blocks = di.blocks;
  cols = di.cols;
  rows = di.rows;
  rows_pr_block = di.rows_pr_block;
} 

void DmdInfo::toBytes(char* buf)
{
  unsigned short int sval;
  
  sval = htons(dmd_type);
  memcpy(buf,&sval,2);

  sval = htons(blocks);
  memcpy(&buf[2],&sval,2);

  sval = htons(cols);
  memcpy(&buf[4],&sval,2);

  sval = htons(rows);
  memcpy(&buf[6],&sval,2);

 sval = htons(rows_pr_block);
 memcpy(&buf[8],&sval,2);
}

/*-----------------------------------------------------------------------------------------------*/

namespace UDP_Helper
{
  
  UDPPayload* ReplyAck(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetInumSize(unsigned short int rows) { return new UDPPayload(SET_INUM_SIZE,new InumSize(rows)); }
  UDPPayload* RequestInumSize() { return new UDPPayload(REQUEST_INUM_SIZE); }
  UDPPayload* ReplyInumSize(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetImageType(unsigned short int image_type) { return new UDPPayload(SET_IMAGE_TYPE,new ImageType(image_type)); }
  UDPPayload* RequestImageType() { return new UDPPayload(REQUEST_IMAGE_TYPE); }
  UDPPayload* ReplyImageType(char* buf) { return new UDPPayload(buf); }
  
  /*-----------------------------------------------------------------------------------------------*/
  
  /*
    get image
  image size is N rows
  calculate number of packets where each data packet is an integer number of lines
  an image is image_type 1 (binary 1 bit bmp) HD 10180 (1920x1080) 1080 lines
  a row is 1920/8 = 240 bytes and must be divisible by 4
  maximum rows per payload is MAX_IMAGE_LEN `div` 240 bytes * 240 bytes
  
  */
  
  
  UDPPayload** LoadImageData(unsigned long int inum
			     ,int& pln, size_t ln, const char* data)
  {
    int max_rows = MAX_IMAGE_LEN/240;
    int max_rows_bytes = max_rows*240;
    int num_full = ln/max_rows_bytes; 
    
    int remaining = ln%max_rows_bytes;
    
    int num_images = num_full;
    if (remaining > 0) num_images += 1;
    
    UDPPayload** pls = new UDPPayload* [num_images];
    
    char* buf = new char[max_rows_bytes];
    
    unsigned short int i;
    for (i = 0; i < num_full; i++)
      {
	memcpy(buf,&data[i*max_rows_bytes],max_rows_bytes);
	
	pls[i] = new UDPPayload(LOAD_IMAGE_DATA,new ImageData(i+1,inum,i*max_rows,max_rows_bytes,buf));
      }
    
    if (remaining % 4 == 0)
      {
	if (remaining > 0)
	  {
	    memset(buf,0,max_rows_bytes);
	    memcpy(buf,&data[i*max_rows_bytes],remaining);
	    
	    pls[i] = new UDPPayload(LOAD_IMAGE_DATA,new ImageData(i+1,inum,i*max_rows,remaining,buf));
	  }
	
	pln = i+1;
      }
    else // not aligned correctly
      {
	return 0;
      }
    return pls;
  }
  
  /*-----------------------------------------------------------------------------------------------*/
  
  UDPPayload* RequestSeqNoError() { return new UDPPayload(REQUEST_SEQ_NO_ERROR); }
  UDPPayload* ReplySeqNoError(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* ResetSeqNo() { return new UDPPayload(RESET_SEQ_NO); }
  
  /*-----------------------------------------------------------------------------------------------*/
  
  UDPPayload** LoadFlatnessCorrectionMask(unsigned short int ln
					  ,int& pln, const char* data)
  {
    int max_bytes = 8962;
    int num_full = ln/max_bytes; 
    
    int remaining = ln%max_bytes;
    
    int num_images = num_full;
    if (remaining > 0) num_images += 1;

    UDPPayload** pls = new UDPPayload* [num_images];

    char* buf = new char[max_bytes];
    
    unsigned short int i;
    for (i = 0; i < num_full; i++)
      {
	memcpy(buf,&data[i*max_bytes],max_bytes);
	
	pls[i] = new UDPPayload(LOAD_FLAT_CORR_MASK,new FlatnessCorrectionMask(i+1,num_images,max_bytes,buf));
      }
    
    pln = i;
    
    if (remaining > 0)
      {
	i += 1;
	memset(buf,0,max_bytes);
	memcpy(buf,&data[i*max_bytes],remaining);
	
	pls[i] = new UDPPayload(LOAD_FLAT_CORR_MASK,new FlatnessCorrectionMask(i+1,num_images,remaining,buf));
      }
    
    pln = i+1;
    
    return pls;
  }
  
  /*-----------------------------------------------------------------------------------------------*/
  
  UDPPayload* SetFlatnessMaskOn(bool enable) { return new UDPPayload(SET_FLAT_MASK_ON,new EnableFCM(enable)); }
  UDPPayload* RequestFlatnessMaskOn() { return new UDPPayload(REQUEST_FLAT_MASK_ON); }
  UDPPayload* ReplyFlatnessMaskOn(char* buf) { return new UDPPayload(buf); }

  UDPPayload* SetLoadInternalImage(char img_no) { return new UDPPayload(LOAD_INTERNAL_IMAGE,new InternalImage(img_no)); }
  UDPPayload* RequestLoadInternalImage() { return new UDPPayload(REQUEST_LOAD_INT_IMG); }
  UDPPayload* ReplyLoadInternalImage(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetAutoloadInternalImage(bool autoload, char img_no) { return new UDPPayload(SET_AUTO_INT_IMAGE,new AutoloadInternalImage(autoload,img_no)); }
  UDPPayload* RequestAutoloadInternalImage() { return new UDPPayload(REQUEST_AUTO_INT_IMG); }
  UDPPayload* ReplyAutoloadInternalImage(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetSaveInternalImage(char img_no) { return new UDPPayload(SAVE_INTERNAL_IMAGE,new InternalImage(img_no)); }

  /*-----------------------------------------------------------------------------------------------*/
  
  UDPPayload** LoadSequenceFile(unsigned short int ln
				,int& pln, const char* data)
  {
    int max_bytes = 8962;
    int num_full = ln/max_bytes; 
    
    int remaining = ln%max_bytes;
    
    int num_images = num_full;
    if (remaining > 0) num_images += 1;
    
    UDPPayload** pls = new UDPPayload* [num_images];
    
    char* buf = new char[max_bytes];
    
    unsigned short int i;
    for (i = 0; i < num_full; i++)
      {
	memcpy(buf,&data[i*max_bytes],max_bytes);
	
	pls[i] = new UDPPayload(LOAD_SEQUENCE_FILE,new SequenceFile(i+1,num_images,max_bytes,buf));
      }
    
    pln = i;
    
    if (remaining > 0)
      {
	i += 1;
	memset(buf,0,max_bytes);
	memcpy(buf,&data[i*max_bytes],remaining);
	
	pls[i] = new UDPPayload(LOAD_SEQUENCE_FILE,new SequenceFile(i+1,num_images,remaining,buf));
      }
    
    pln = i+1;
    
    return pls;
  }
  
  /*-----------------------------------------------------------------------------------------------*/
  
  UDPPayload* SetSequencerState(char seq_cmd, bool enable) { return new UDPPayload(SET_SEQUENCER_STATE,new SequencerState(seq_cmd,enable)); }
  UDPPayload* RequestSequencerState(char seq_cmd) { return new UDPPayload(REQUEST_SEQ_STATE,new SequencerState(seq_cmd)); }
  UDPPayload* ReplySequencerState(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* RequestSeqFileErrorLog() { return new UDPPayload(REQUEST_SEQ_ERR_LOG); }
  UDPPayload* ReplySeqFileErrorLog(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetSoftwareSync(bool enable) { return new UDPPayload(SET_SOFTWARE_SYNC,new Sync(enable)); }
  UDPPayload* RequestSoftwareSync() { return new UDPPayload(REQUEST_SOFT_SYNC); }
  UDPPayload* ReplySoftwareSync(char* buf) { return new UDPPayload(buf); }

  UDPPayload* RequestTriggerCount() { return new UDPPayload(REQUEST_TRIGGER_CNT); }
  UDPPayload* ReplyTriggerCount(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetInternalSyncFreq(unsigned short int s
				  , unsigned short int m
				  , unsigned short int u)
  { return new UDPPayload(SET_INT_SYNC_FRQ,new InternalSyncFreq(s,m,u)); }
  UDPPayload* RequestInternalSyncFreq() { return new UDPPayload(REQUEST_INT_SYNC_FRQ); }
  UDPPayload* ReplyInternalSyncFreq(char* buf) { return new UDPPayload(buf); }
  
UDPPayload* SetSequenceReg(unsigned short int rn
			   ,unsigned short int rv)
{ return new UDPPayload(SET_SEQUENCE_REG,new SequenceReg(rn,rv)); }
  UDPPayload* RequestSequenceReg(unsigned short int rn) { return new UDPPayload(REQUEST_SEQ_REG,new SequenceReg(rn)); }
  UDPPayload* ReplySequenceReg(char* buf) { return new UDPPayload(buf); }

  UDPPayload* SetLightPulseDuration(unsigned short int duration) { return new UDPPayload(SET_LIGHT_PLS_DR,new LightPulseDuration(duration)); }
  UDPPayload* RequestLightPulseDuration() { return new UDPPayload(REQUEST_LIGHT_PLS_DR); }
  UDPPayload* ReplyLightPulseDuration(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* RequestTrigInfo() { return new UDPPayload(REQUEST_TRIG_INFO); }
  UDPPayload* ReplyTrigInfo(char* buf) { return new UDPPayload(buf); }

  UDPPayload* SetTrigDelay(unsigned short int delay) { return new UDPPayload(SET_TRIG_DELAY,new TrigDelay(delay)); }
  UDPPayload* RequestTrigDelay() { return new UDPPayload(REQUEST_TRIG_DELAY); }
  UDPPayload* ReplyTrigDelay(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetTrigDivide(unsigned short int factor) { return new UDPPayload(SET_TRIG_DIVIDE,new TrigDivide(factor)); }
  UDPPayload* RequestTrigDivide() { return new UDPPayload(REQUEST_TRIG_DIVIDE); }
  UDPPayload* ReplyTrigDivide(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetActiveSequence(char type) { return new UDPPayload(SET_ACTIVE_SEQ,new ActiveSequence(type)); }
  UDPPayload* RequestActiveSequence() { return new UDPPayload(REQUEST_ACTIVE_SEQ); }
  UDPPayload* ReplyActiveSequence(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetAutostartSequencer(bool mode) { return new UDPPayload(SET_AUTO_START_SEQ,new AutostartSequencer(mode ? (char)1 : (char)0)); }
  UDPPayload* RequestAutostartSequencer() { return new UDPPayload(REQUEST_AUTO_START); }
  UDPPayload* ReplyAutostartSequencer(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetDmdMirrorShake(bool enable) { return new UDPPayload(SET_DMD_MIRROR_SHAKE,new DmdMirrorShake(enable ? (char)1 : (char)0)); }
  UDPPayload* RequestDmdMirrorShake() { return new UDPPayload(REQUEST_DMD_MIR_SHK); }
  UDPPayload* ReplyDmdMirrorShake(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetTrigSource(bool optical,bool negative) { return new UDPPayload(SET_TRIG_SOURCE,new TrigSource(optical ? (char)1 : (char)0,negative ? (char)1 : (char)0)); }
  UDPPayload* RequestTrigSource() { return new UDPPayload(REQUEST_TRIG_SOURCE); }
  UDPPayload* ReplyTrigSource(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetIllumLed(char type) { return new UDPPayload(SET_ILLUM_LED,new IllumLed(type)); }
  UDPPayload* RequestIllumLed() { return new UDPPayload(REQUEST_ILLUM_LED); }
  UDPPayload* ReplyIllumLed(char* buf) { return new UDPPayload(buf); }

  UDPPayload* SetNetworkSettings(unsigned long int ip,unsigned long int sn
				 ,unsigned long int gw,bool d) { return new UDPPayload(SET_NETWORK_SETTINGS,new NetworkSettings(ip,sn,gw,d)); }
  UDPPayload* RequestNetworkSettings() { return new UDPPayload(REQUEST_NETWORK_SET); }
  UDPPayload* ReplyNetworkSettings(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* SaveSettings() { return new UDPPayload(SAVE_SETTINGS); }
  
  UDPPayload* SetFactoryDefaults() { return new UDPPayload(SET_FACTORY_DEFAULTS); }

  UDPPayload* SetSafeShutdown() { return new UDPPayload(SET_SAFE_SHUTDOWN); }
  
  UDPPayload* RequestFwVersion() { return new UDPPayload(REQUEST_FW_VERSION); }
  UDPPayload* ReplyFwVersion(char* buf) { return new UDPPayload(buf); }
  
  UDPPayload* RequestDmdInfo() { return new UDPPayload(REQUEST_FW_VERSION); }
  UDPPayload* ReplyDmdInfo(char* buf) { return new UDPPayload(buf); }

  UDPPayload* RequestErrorStatus() { return new UDPPayload(REQUEST_ERROR_STATUS); }
  UDPPayload* ReplyErrorStatus(char* buf) { return new UDPPayload(buf); }

  UDPPayload* SetClearErrorStatus() { return new UDPPayload(SET_CLEAR_ERROR_STAT); }
  

}

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
