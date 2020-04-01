#include <cstring>

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "dmd_udp.h"

/*-----------------------------------------------------------------------------------------------*/

// NOT CORRECT (I THINK)
int char_to_int32(QByteArray buf)
{
  int num = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];

  return num;
}

unsigned int char_to_uint32(QByteArray buf)
{
  unsigned int num = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | (buf[3] & 0xFF);

  return num;
}

// NOT CORRECT (I THINK)
short int char_to_int16(QByteArray buf)
{
  short int num = (buf[1] << 8) | buf[0];

  return num;
}

unsigned short int char_to_uint16(QByteArray buf)
{
  unsigned short int num = (buf[0] << 8) | (buf[1] & 0xFF);
//  unsigned short int num = (buf[0] << 8) | (buf[1]);
  return num;
}

/*-----------------------------------------------------------------------------------------------*/

void int16_to_char(short int val, QByteArray& buf)
{
    buf.append(val >> 8 & 0xFF);
    buf.append(val & 0xFF);
}

void uint16_to_char(unsigned short int val, QByteArray& buf)
{
    buf.append(val >> 8 & 0xFF);
    buf.append(val & 0xFF);
}

void int32_to_char(int val, QByteArray& buf)
{
    buf.append(val >> 24 & 0xFF);
    buf.append(val >> 16 & 0xFF);
    buf.append(val >> 8 & 0xFF);
    buf.append(val & 0xFF);
}

void uint32_to_char(unsigned int val, QByteArray& buf)
{
    buf.append(val >> 24 & 0xFF);
    buf.append(val >> 16 & 0xFF);
    buf.append(val >> 8 & 0xFF);
    buf.append(val & 0xFF);
}

void int64_to_char(long int val, QByteArray& buf)
{
    buf.append(val >> 56 & 0xFF);
    buf.append(val >> 48 & 0xFF);
    buf.append(val >> 40 & 0xFF);
    buf.append(val >> 32 & 0xFF);
    buf.append(val >> 24 & 0xFF);
    buf.append(val >> 16 & 0xFF);
    buf.append(val >> 8 & 0xFF);
    buf.append(val & 0xFF);
}

void uint64_to_char(unsigned long int val, QByteArray& buf)
{
    buf.append(val >> 56 & 0xFF);
    buf.append(val >> 48 & 0xFF);
    buf.append(val >> 40 & 0xFF);
    buf.append(val >> 32 & 0xFF);
    buf.append(val >> 24 & 0xFF);
    buf.append(val >> 16 & 0xFF);
    buf.append(val >> 8 & 0xFF);
    buf.append(val & 0xFF);
}

/*-----------------------------------------------------------------------------------------------*/

UDPPayload::UDPPayload(QByteArray buf)
{
  tot_size = char_to_uint16(buf);
  rec_id   = char_to_uint16(buf.remove(0,2));

  fprintf(stderr,"Size: %x\tRecord ID: %x\n",tot_size,rec_id);

  switch (rec_id)
    {
    case REPLY_ACK:
      payload = new Ack(buf.remove(0,2));
      break;
    case REPLY_INUM_SIZE:
      payload = new InumSize(buf.remove(0,2));
      break;
    case REPLY_IMAGE_TYPE:
      payload = new ImageType(buf.remove(0,2));
      break;
    case REPLY_SEQ_NO_ERROR:
      payload = new SeqNoError(buf.remove(0,2));
      break;
    case REPLY_FLAT_MASK_ON:
      payload = new EnableFCM(buf.remove(0,2));
      break;
    case REPLY_LOAD_INT_IMG:
      payload = new InternalImage(buf.remove(0,2));
      break;
    case REPLY_AUTO_INT_IMG:
      payload = new AutoloadInternalImage(buf.remove(0,2));
      break;
    case REPLY_SEQ_STATE:
      payload = new SequencerState(buf.remove(0,2));
      break;
    case REPLY_SEQ_ERR_LOG:
      payload = new ErrorLog(buf.remove(0,2));
      break;
    case REPLY_SOFT_SYNC:
      payload = new Sync(buf.remove(0,2));
      break;
    case REPLY_TRIGGER_CNT:
      payload = new TriggerCount(buf.remove(0,2));
      break;
    case REPLY_INT_SYNC_FRQ:
      payload = new InternalSyncFreq(buf.remove(0,2));
      break;
    case REPLY_SEQ_REG:
      payload = new SequenceReg(buf.remove(0,2));
      break;
    case REPLY_LIGHT_PLS_DR:
      payload = new LightPulseDuration(buf.remove(0,2));
      break;
    case REPLY_TRIG_INFO:
      payload = new TrigInfo(buf.remove(0,2));
      break;
    case REPLY_TRIG_DELAY:
      payload = new TrigDelay(buf.remove(0,2));
      break;
    case REPLY_TRIG_DIVIDE:
      payload = new TrigDivide(buf.remove(0,2));
      break;
    case REPLY_ACTIVE_SEQ:
      payload = new ActiveSequence(buf.remove(0,2));
      break;
    case REPLY_AUTO_START:
      payload = new AutostartSequencer(buf.remove(0,2));
      break;
    case REPLY_DMD_MIR_SHK:
      payload = new DmdMirrorShake(buf.remove(0,2));
      break;
    case REPLY_TRIG_SOURCE:
      payload = new TrigSource(buf.remove(0,2));
      break;
    case REPLY_ILLUM_LED:
      payload = new IllumLed(buf.remove(0,2));
      break;
    case REPLY_NETWORK_SET:
      payload = new NetworkSettings(buf.remove(0,2));
      break;
    case REPLY_FW_VERSION:
      payload = new FwVersion(buf.remove(0,2));
      break;
    case REPLY_DMD_INFO:
      payload = new DmdInfo(buf.remove(0,2));
      break;
    case REPLY_ERROR_STATUS:
      payload = new ErrorStatus(buf.remove(0,2));
      break;
    //----------------------------------------------------------------------------------
    case LOAD_IMAGE_DATA:
      payload = new ImageData(tot_size-14,buf.remove(0,2));
      break;
    default:
      payload = 0;
      fprintf(stderr,"UDPPayload::UDPPayload(QByteArray): did not recognise record id\n");
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

void UDPPayload::toBytes(QByteArray& buf)
{

  uint16_to_char(tot_size,buf);
  uint16_to_char(rec_id,buf);

  if (payload) payload->toBytes(buf);
}

/*-----------------------------------------------------------------------------------------------*/

Char::Char(QByteArray buf)
{
  char_value = buf[0];
}

Char::Char(const Char& ra)
{
  char_value = ra.char_value;
}

void Char::toBytes(QByteArray& buf)
{
  buf.append(char_value);
}

/*-----------------------------------------------------------------------------------------------*/

UnsignedShortInt::UnsignedShortInt(QByteArray buf)
{
  unsigned_short_int = char_to_uint16(buf);
}

UnsignedShortInt::UnsignedShortInt(const UnsignedShortInt& ra)
{
  unsigned_short_int = ra.unsigned_short_int;
}

void UnsignedShortInt::toBytes(QByteArray& buf)
{
  uint16_to_char(unsigned_short_int,buf);
}

/*-----------------------------------------------------------------------------------------------*/

UnsignedInt::UnsignedInt(QByteArray buf)
{
  unsigned_int = char_to_uint32(buf);
}

UnsignedInt::UnsignedInt(const UnsignedInt& ra)
{
  unsigned_int = ra.unsigned_int;
}

void UnsignedInt::toBytes(QByteArray& buf)
{
  uint32_to_char(unsigned_int,buf);
}

/*-----------------------------------------------------------------------------------------------*/

UnsignedLongInt::UnsignedLongInt(QByteArray buf)
{
  unsigned_long_int = char_to_uint32(buf);
}

UnsignedLongInt::UnsignedLongInt(const UnsignedLongInt& ra)
{
  unsigned_long_int = ra.unsigned_long_int;
}

void UnsignedLongInt::toBytes(QByteArray& buf)
{
  uint64_to_char(unsigned_long_int,buf);
}

/*-----------------------------------------------------------------------------------------------*/

ImageData::ImageData(const int len, QByteArray buf)
{
  seq_no = char_to_uint16(buf);
  inum = char_to_uint32(buf.remove(0,2));
  offset = char_to_uint32(buf.remove(0,4));
  length = len;

  data = new char[len];
  memcpy(data,buf.remove(0,4).data(),len);
}

ImageData::ImageData(const unsigned short int sn
		     ,const unsigned long int in
		     ,const unsigned long int os
		     ,const size_t ln
             ,QByteArray dt)
{
  seq_no = sn;
  inum = in;
  offset = os;
  length = ln;

  data = new char[length];
  memcpy(data,dt.data(),length);
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

int ImageData::getData(size_t& len, QByteArray& buf)
{
  len = length;
  buf.clear();
  buf.append(data,length);
  
  return 0;
}

void ImageData::toBytes(QByteArray& buf)
{
  uint16_to_char(seq_no,buf);
  uint32_to_char(inum,buf);
  uint32_to_char(offset,buf);
  buf.append(data,length);
}

/*-----------------------------------------------------------------------------------------------*/

SeqNoError::SeqNoError(QByteArray buf)
{
  is_error = buf[0];
  
  last_seq_no = char_to_uint16(buf.remove(0,1));
}

SeqNoError::SeqNoError(const SeqNoError& sne)
{
  is_error = sne.is_error;
  last_seq_no = sne.is_error;
}

SeqNoError::~SeqNoError() {}

void SeqNoError::toBytes(QByteArray& buf)
{
  buf.append(is_error);

  buf.append(last_seq_no);
}

/*-----------------------------------------------------------------------------------------------*/

FlatnessCorrectionMask::FlatnessCorrectionMask(const unsigned short int pn
					       ,const unsigned short int tp
					       ,const unsigned short int ds
                           ,QByteArray dt)
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

void FlatnessCorrectionMask::toBytes(QByteArray& buf)
{
  uint16_to_char(pkg_no,buf);
  uint16_to_char(tot_pkg,buf);
  uint16_to_char(data_size,buf);
  buf.append(data,data_size);
}

/*-----------------------------------------------------------------------------------------------*/

AutoloadInternalImage::AutoloadInternalImage(QByteArray buf)
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

void AutoloadInternalImage::toBytes(QByteArray& buf)
{
  buf.append(autoload);
  buf.append(img_no);
}

/*-----------------------------------------------------------------------------------------------*/

SequenceFile::SequenceFile(const unsigned short int pn
				   ,const unsigned short int tp
				   ,const unsigned short int ds
             ,QByteArray dt)
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

void SequenceFile::toBytes(QByteArray& buf)
{
  uint16_to_char(pkg_no,buf);
  uint16_to_char(tot_pkg,buf);
  uint16_to_char(data_size,buf);
  buf.append(data,data_size);
}

/*-----------------------------------------------------------------------------------------------*/

SequencerState::SequencerState(QByteArray buf)
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

void SequencerState::toBytes(QByteArray& buf)
{
  buf.append(seq_cmd);
  if (enable < 2)
    {
      buf.append(enable);
      if (valid_cmd < 2) buf.append(valid_cmd);
    }
}

/*-----------------------------------------------------------------------------------------------*/

ErrorLog::ErrorLog(QByteArray buf)
{
  data_size = char_to_uint16(buf);

  data = new char[data_size];
  memcpy(data,buf.remove(0,2).data(),data_size);
}

ErrorLog::ErrorLog(const unsigned short int ds
           ,QByteArray dt)
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

void ErrorLog::toBytes(QByteArray& buf)
{
  uint16_to_char(data_size,buf);
  buf.append(data,data_size);
}

/*-----------------------------------------------------------------------------------------------*/

InternalSyncFreq::InternalSyncFreq(QByteArray buf)
{
  sec = char_to_uint16(buf);
  ms  = char_to_uint16(buf.remove(0,2));
  us  = char_to_uint16(buf.remove(0,2));
}

void InternalSyncFreq::toBytes(QByteArray& buf)
{
  uint16_to_char(sec,buf);
  uint16_to_char(ms,buf);
  uint16_to_char(us,buf);
}

/*-----------------------------------------------------------------------------------------------*/

SequenceReg::SequenceReg(QByteArray buf)
{
  reg_no = char_to_uint16(buf);
  reg_val = char_to_uint16(buf.remove(0,2));
  valid_reg  = buf[5];
}

void SequenceReg::toBytes(QByteArray& buf)
{
  uint16_to_char(reg_no,buf);

  if (reg_val != 65535)
    {
      uint16_to_char(reg_val,buf);

      if (valid_reg < 2) buf.append(valid_reg);
    }
}

/*-----------------------------------------------------------------------------------------------*/

TrigInfo::TrigInfo(QByteArray buf)
{
  in_puls_e = char_to_uint32(buf);
  in_puls_o = char_to_uint32(buf.remove(0,4));
  min_per_e = char_to_uint16(buf.remove(0,4));
  min_per_o = char_to_uint16(buf.remove(0,2));
  seq_puls = char_to_uint16(buf.remove(0,2));
}


TrigInfo::TrigInfo(const TrigInfo& ti)
{
  in_puls_e = ti.in_puls_e;
  in_puls_o = ti.in_puls_o;
  min_per_e = ti.min_per_e;
  min_per_o = ti.min_per_o;
  seq_puls = ti.seq_puls;
  
}

void TrigInfo::toBytes(QByteArray& buf)
{
  uint32_to_char(in_puls_e,buf);
  uint32_to_char(in_puls_o,buf);
  uint16_to_char(min_per_e,buf);
  uint16_to_char(min_per_o,buf);
  uint16_to_char(seq_puls,buf);
}

/*-----------------------------------------------------------------------------------------------*/

TrigSource::TrigSource(QByteArray buf)
{
  source = char_to_uint16(buf);
  edge = char_to_uint16(buf.remove(0,2));
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

void TrigSource::toBytes(QByteArray& buf)
{
  uint16_to_char(source,buf);
  uint16_to_char(edge,buf);
}

/*-----------------------------------------------------------------------------------------------*/

NetworkSettings::NetworkSettings(QByteArray buf)
{
  ip_addr = char_to_uint32(buf);
  subnet = char_to_uint32(buf.remove(0,4));
  gateway = char_to_uint32(buf.remove(0,4));

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

void NetworkSettings::toBytes(QByteArray& buf)
{
  uint32_to_char(ip_addr,buf);
  uint32_to_char(subnet,buf);
  uint32_to_char(gateway,buf);

  buf.append(dhcp);
}

/*-----------------------------------------------------------------------------------------------*/

FwVersion::FwVersion(QByteArray buf)
{
 sys_fw_major = char_to_uint16(buf);
 sys_fw_minor = char_to_uint16(buf.remove(0,2));
 app_sw_ver = char_to_uint16(buf.remove(0,2));
 app_sw_year = char_to_uint16(buf.remove(0,2));
 app_sw_month = buf[2];
 app_sw_day = buf[3];
 app_sw_hour = buf[4];
 app_sw_min = buf[5];
 app_sw_sec = buf[6];
 FPGA_year = char_to_uint16(buf.remove(0,7));
 FPGA_month = buf[2];
 FPGA_day = buf[3];
 FPGA_hour = buf[4];
 FPGA_min = buf[5];
 FPGA_sec = buf[6];
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

void FwVersion::toBytes(QByteArray& buf)
{
  uint16_to_char(sys_fw_major,buf);
  uint16_to_char(sys_fw_minor,buf);
  uint16_to_char(app_sw_ver,buf);
  uint16_to_char(app_sw_year,buf);

 buf.append(app_sw_month);
 buf.append(app_sw_day);
 buf.append(app_sw_hour);
 buf.append(app_sw_min);
 buf.append(app_sw_sec);

 uint16_to_char(FPGA_year,buf);

 buf.append(FPGA_month);
 buf.append(FPGA_day);
 buf.append(FPGA_hour);
 buf.append(FPGA_min);
 buf.append(FPGA_sec);
}

/*-----------------------------------------------------------------------------------------------*/

DmdInfo::DmdInfo(QByteArray buf)
{
 dmd_type = char_to_uint16(buf);
 blocks = char_to_uint16(buf.remove(0,2));
 cols = char_to_uint16(buf.remove(0,2));
 rows = char_to_uint16(buf.remove(0,2));
 rows_pr_block = char_to_uint16(buf.remove(0,2));
}

DmdInfo::DmdInfo(const DmdInfo& di)
{
  dmd_type = di.dmd_type;
  blocks = di.blocks;
  cols = di.cols;
  rows = di.rows;
  rows_pr_block = di.rows_pr_block;
} 

void DmdInfo::toBytes(QByteArray& buf)
{
  uint16_to_char(dmd_type,buf);
  uint16_to_char(blocks,buf);
  uint16_to_char(cols,buf);
  uint16_to_char(rows,buf);
  uint16_to_char(rows_pr_block,buf);
}

/*-----------------------------------------------------------------------------------------------*/

namespace UDP_Helper
{
  
  UDPPayload* ReplyAck(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetInumSize(unsigned short int rows) { return new UDPPayload(SET_INUM_SIZE,new InumSize(rows)); }
  UDPPayload* RequestInumSize() { return new UDPPayload(REQUEST_INUM_SIZE); }
  UDPPayload* ReplyInumSize(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetImageType(unsigned short int image_type) { return new UDPPayload(SET_IMAGE_TYPE,new ImageType(image_type)); }
  UDPPayload* RequestImageType() { return new UDPPayload(REQUEST_IMAGE_TYPE); }
  UDPPayload* ReplyImageType(QByteArray buf) { return new UDPPayload(buf); }
  
  /*-----------------------------------------------------------------------------------------------*/
  
  /*
    get image
  image size is N rows
  calculate number of packets where each data packet is an integer number of lines
  an image is image_type 1 (binary 1 bit bmp) WUXGA (1920x1200) 1200 lines
  a row is 1920/8 = 240 bytes and must be divisible by 4
  maximum rows per payload is MAX_IMAGE_LEN `div` 240 bytes * 240 bytes
  
  */
  
  
  UDPPayload** LoadImageData(unsigned long int inum
                 ,int& pln, size_t ln, QByteArray data)
  {
    int max_rows = MAX_IMAGE_LEN/240;
    int max_rows_bytes = max_rows*240;
    int num_full = ln/max_rows_bytes;
    
    fprintf(stderr,"Size: %d\t\tTotal: %d\n",ln,data.size());
    fprintf(stderr,"Rows per payload: %d, Bytes: %d, Num: %d\n",max_rows,max_rows_bytes,num_full);

    int remaining = ln%max_rows_bytes;
    
    int num_images = num_full;
    if (remaining > 0) num_images += 1;
    
    UDPPayload** pls = new UDPPayload* [num_images];
    
    char* buf = new char[max_rows_bytes];
    
    unsigned short int i;
    for (i = 0; i < num_full; i++) {
        memcpy(buf,data.data(),max_rows_bytes);
        fprintf(stderr,"Iteration: %d, buf size: %d, data size: %d, bytes copied: %d\n",i,strlen(buf),data.size(),max_rows_bytes);
        data.remove(0,max_rows_bytes);
	
        pls[i] = new UDPPayload(LOAD_IMAGE_DATA,new ImageData(i+1,inum,i*max_rows,max_rows_bytes,buf));
    }
    
    if (remaining % 4 == 0) {
        if (remaining > 0) {
            memset(buf,0,max_rows_bytes);
            memcpy(buf,data.data(),remaining);
            fprintf(stderr,"Iteration: %d, buf size: %d, data size: %d\n",i,strlen(buf),data.size());
            data.remove(0,remaining);

            pls[i] = new UDPPayload(LOAD_IMAGE_DATA,new ImageData(i+1,inum,i*max_rows,remaining,buf));
        }
    }
    else {// not aligned correctly
        return 0;
    }

    pln = num_images;

    return pls;
  }
  
  /*-----------------------------------------------------------------------------------------------*/
  
  UDPPayload* RequestSeqNoError() { return new UDPPayload(REQUEST_SEQ_NO_ERROR); }
  UDPPayload* ReplySeqNoError(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* ResetSeqNo() { return new UDPPayload(RESET_SEQ_NO); }
  
  /*-----------------------------------------------------------------------------------------------*/
  
  UDPPayload** LoadFlatnessCorrectionMask(unsigned short int ln
                      ,int& pln, QByteArray data)
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
    memcpy(buf,data.data(),max_bytes);
    data.remove(0,i*max_bytes);

	pls[i] = new UDPPayload(LOAD_FLAT_CORR_MASK,new FlatnessCorrectionMask(i+1,num_images,max_bytes,buf));
      }
    
    pln = i;
    
    if (remaining > 0)
      {
	i += 1;
	memset(buf,0,max_bytes);
    memcpy(buf,data.data(),remaining);
    data.remove(0,remaining);
	
	pls[i] = new UDPPayload(LOAD_FLAT_CORR_MASK,new FlatnessCorrectionMask(i+1,num_images,remaining,buf));
      }
    
    pln = i+1;
    
    return pls;
  }
  
  /*-----------------------------------------------------------------------------------------------*/
  
  UDPPayload* SetFlatnessMaskOn(bool enable) { return new UDPPayload(SET_FLAT_MASK_ON,new EnableFCM(enable)); }
  UDPPayload* RequestFlatnessMaskOn() { return new UDPPayload(REQUEST_FLAT_MASK_ON); }
  UDPPayload* ReplyFlatnessMaskOn(QByteArray buf) { return new UDPPayload(buf); }

  UDPPayload* SetLoadInternalImage(char img_no) { return new UDPPayload(LOAD_INTERNAL_IMAGE,new InternalImage(img_no)); }
  UDPPayload* RequestLoadInternalImage() { return new UDPPayload(REQUEST_LOAD_INT_IMG); }
  UDPPayload* ReplyLoadInternalImage(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetAutoloadInternalImage(bool autoload, char img_no) { return new UDPPayload(SET_AUTO_INT_IMAGE,new AutoloadInternalImage(autoload,img_no)); }
  UDPPayload* RequestAutoloadInternalImage() { return new UDPPayload(REQUEST_AUTO_INT_IMG); }
  UDPPayload* ReplyAutoloadInternalImage(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetSaveInternalImage(char img_no) { return new UDPPayload(SAVE_INTERNAL_IMAGE,new InternalImage(img_no)); }

  /*-----------------------------------------------------------------------------------------------*/
  
  UDPPayload** LoadSequenceFile(unsigned short int ln
                ,int& pln, QByteArray data)
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
        memcpy(buf,data.data(),max_bytes);
        data.remove(0,max_bytes);

	pls[i] = new UDPPayload(LOAD_SEQUENCE_FILE,new SequenceFile(i+1,num_images,max_bytes,buf));
      }
    
    pln = num_full;
    
    if (remaining > 0)
      {
        memset(buf,0,max_bytes);
        memcpy(buf,data.data(),remaining);
        data.remove(0,remaining);

	pls[i] = new UDPPayload(LOAD_SEQUENCE_FILE,new SequenceFile(i+1,num_images,remaining,buf));

        pln += 1;
      }
        
    return pls;
  }
  
  /*-----------------------------------------------------------------------------------------------*/
  
  UDPPayload* SetSequencerState(char seq_cmd, bool enable) { return new UDPPayload(SET_SEQUENCER_STATE,new SequencerState(seq_cmd,enable)); }
  UDPPayload* RequestSequencerState(char seq_cmd) { return new UDPPayload(REQUEST_SEQ_STATE,new SequencerState(seq_cmd)); }
  UDPPayload* ReplySequencerState(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* RequestSeqFileErrorLog() { return new UDPPayload(REQUEST_SEQ_ERR_LOG); }
  UDPPayload* ReplySeqFileErrorLog(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetSoftwareSync(bool enable) { return new UDPPayload(SET_SOFTWARE_SYNC,new Sync(enable)); }
  UDPPayload* RequestSoftwareSync() { return new UDPPayload(REQUEST_SOFT_SYNC); }
  UDPPayload* ReplySoftwareSync(QByteArray buf) { return new UDPPayload(buf); }

  UDPPayload* RequestTriggerCount() { return new UDPPayload(REQUEST_TRIGGER_CNT); }
  UDPPayload* ReplyTriggerCount(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetInternalSyncFreq(unsigned short int s
				  , unsigned short int m
				  , unsigned short int u)
  { return new UDPPayload(SET_INT_SYNC_FRQ,new InternalSyncFreq(s,m,u)); }
  UDPPayload* RequestInternalSyncFreq() { return new UDPPayload(REQUEST_INT_SYNC_FRQ); }
  UDPPayload* ReplyInternalSyncFreq(QByteArray buf) { return new UDPPayload(buf); }
  
UDPPayload* SetSequenceReg(unsigned short int rn
			   ,unsigned short int rv)
{ return new UDPPayload(SET_SEQUENCE_REG,new SequenceReg(rn,rv)); }
  UDPPayload* RequestSequenceReg(unsigned short int rn) { return new UDPPayload(REQUEST_SEQ_REG,new SequenceReg(rn)); }
  UDPPayload* ReplySequenceReg(QByteArray buf) { return new UDPPayload(buf); }

  UDPPayload* SetLightPulseDuration(unsigned short int duration) { return new UDPPayload(SET_LIGHT_PLS_DR,new LightPulseDuration(duration)); }
  UDPPayload* RequestLightPulseDuration() { return new UDPPayload(REQUEST_LIGHT_PLS_DR); }
  UDPPayload* ReplyLightPulseDuration(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* RequestTrigInfo() { return new UDPPayload(REQUEST_TRIG_INFO); }
  UDPPayload* ReplyTrigInfo(QByteArray buf) { return new UDPPayload(buf); }

  UDPPayload* SetTrigDelay(unsigned short int delay) { return new UDPPayload(SET_TRIG_DELAY,new TrigDelay(delay)); }
  UDPPayload* RequestTrigDelay() { return new UDPPayload(REQUEST_TRIG_DELAY); }
  UDPPayload* ReplyTrigDelay(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetTrigDivide(unsigned short int factor) { return new UDPPayload(SET_TRIG_DIVIDE,new TrigDivide(factor)); }
  UDPPayload* RequestTrigDivide() { return new UDPPayload(REQUEST_TRIG_DIVIDE); }
  UDPPayload* ReplyTrigDivide(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetActiveSequence(char type) { return new UDPPayload(SET_ACTIVE_SEQ,new ActiveSequence(type)); }
  UDPPayload* RequestActiveSequence() { return new UDPPayload(REQUEST_ACTIVE_SEQ); }
  UDPPayload* ReplyActiveSequence(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetAutostartSequencer(bool mode) { return new UDPPayload(SET_AUTO_START_SEQ,new AutostartSequencer(mode ? (char)1 : (char)0)); }
  UDPPayload* RequestAutostartSequencer() { return new UDPPayload(REQUEST_AUTO_START); }
  UDPPayload* ReplyAutostartSequencer(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetDmdMirrorShake(bool enable) { return new UDPPayload(SET_DMD_MIRROR_SHAKE,new DmdMirrorShake(enable ? (char)1 : (char)0)); }
  UDPPayload* RequestDmdMirrorShake() { return new UDPPayload(REQUEST_DMD_MIR_SHK); }
  UDPPayload* ReplyDmdMirrorShake(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetTrigSource(bool optical,bool negative) { return new UDPPayload(SET_TRIG_SOURCE,new TrigSource(optical ? (char)1 : (char)0,negative ? (char)1 : (char)0)); }
  UDPPayload* RequestTrigSource() { return new UDPPayload(REQUEST_TRIG_SOURCE); }
  UDPPayload* ReplyTrigSource(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SetIllumLed(char type) { return new UDPPayload(SET_ILLUM_LED,new IllumLed(type)); }
  UDPPayload* RequestIllumLed() { return new UDPPayload(REQUEST_ILLUM_LED); }
  UDPPayload* ReplyIllumLed(QByteArray buf) { return new UDPPayload(buf); }

  UDPPayload* SetNetworkSettings(unsigned long int ip,unsigned long int sn
				 ,unsigned long int gw,bool d) { return new UDPPayload(SET_NETWORK_SETTINGS,new NetworkSettings(ip,sn,gw,d)); }
  UDPPayload* RequestNetworkSettings() { return new UDPPayload(REQUEST_NETWORK_SET); }
  UDPPayload* ReplyNetworkSettings(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* SaveSettings() { return new UDPPayload(SAVE_SETTINGS); }
  
  UDPPayload* SetFactoryDefaults() { return new UDPPayload(SET_FACTORY_DEFAULTS); }

  UDPPayload* SetSafeShutdown() { return new UDPPayload(SET_SAFE_SHUTDOWN); }
  
  UDPPayload* RequestFwVersion() { return new UDPPayload(REQUEST_FW_VERSION); }
  UDPPayload* ReplyFwVersion(QByteArray buf) { return new UDPPayload(buf); }
  
  UDPPayload* RequestDmdInfo() { return new UDPPayload(REQUEST_FW_VERSION); }
  UDPPayload* ReplyDmdInfo(QByteArray buf) { return new UDPPayload(buf); }

  UDPPayload* RequestErrorStatus() { return new UDPPayload(REQUEST_ERROR_STATUS); }
  UDPPayload* ReplyErrorStatus(QByteArray buf) { return new UDPPayload(buf); }

  UDPPayload* SetClearErrorStatus() { return new UDPPayload(SET_CLEAR_ERROR_STAT); }
  

}

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
