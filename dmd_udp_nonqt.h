#ifndef __AVHM__DMD_UDP_H__
#define __AVHM__DMD_UDP_H__

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

#define SERVER "192.168.0.10"

#define PORT 52985   //The port on which to send data
#define IMG_PORT 52986 // the image data port

#define BUFLEN 8972  //Max length of buffer

#define MAX_IMAGE_LEN 8956

#define SET_INUM_SIZE        102
#define SET_IMAGE_TYPE       103
#define LOAD_IMAGE_DATA      104
#define RESET_SEQ_NO         112
#define LOAD_FLAT_CORR_MASK  116
#define SET_FLAT_MASK_ON     148
#define LOAD_INTERNAL_IMAGE  169
#define SET_AUTO_INT_IMAGE   177
#define SAVE_INTERNAL_IMAGE  170
#define LOAD_SEQUENCE_FILE   105
#define SET_SEQUENCER_STATE  106
#define SET_SOFTWARE_SYNC    120
#define SET_INT_SYNC_FRQ     144
#define SET_SEQUENCE_REG     122
#define SET_LIGHT_PLS_DR     123
#define SET_TRIG_DELAY       161
#define SET_TRIG_DIVIDE      164
#define SET_ACTIVE_SEQ       171
#define SET_AUTO_START_SEQ   172
#define SET_DMD_MIRROR_SHAKE 195
#define SET_TRIG_SOURCE      160
#define SET_ILLUM_LED        196
#define SET_NETWORK_SETTINGS 108
#define SAVE_SETTINGS        110
#define SET_FACTORY_DEFAULTS 113
#define SET_SAFE_SHUTDOWN    180
#define SET_CLEAR_ERROR_STAT 174

#define REQUEST_INUM_SIZE    302
#define REQUEST_IMAGE_TYPE   303
#define REQUEST_SEQ_NO_ERROR 311
#define REQUEST_FLAT_MASK_ON 348
#define REQUEST_LOAD_INT_IMG 369
#define REQUEST_AUTO_INT_IMG 377
#define REQUEST_SEQ_STATE    306
#define REQUEST_SEQ_ERR_LOG  307
#define REQUEST_SOFT_SYNC    320
#define REQUEST_TRIGGER_CNT  321
#define REQUEST_INT_SYNC_FRQ 344
#define REQUEST_SEQ_REG      322
#define REQUEST_LIGHT_PLS_DR 323
#define REQUEST_TRIG_INFO    352
#define REQUEST_TRIG_DELAY   361
#define REQUEST_TRIG_DIVIDE  364
#define REQUEST_ACTIVE_SEQ   371
#define REQUEST_AUTO_START   372
#define REQUEST_DMD_MIR_SHK  395
#define REQUEST_TRIG_SOURCE  360
#define REQUEST_ILLUM_LED    396
#define REQUEST_NETWORK_SET  308
#define REQUEST_FW_VERSION   314
#define REQUEST_DMD_INFO     315
#define REQUEST_ERROR_STATUS 376

#define REPLY_ACK            501
#define REPLY_INUM_SIZE      502
#define REPLY_IMAGE_TYPE     503
#define REPLY_SEQ_NO_ERROR   511
#define REPLY_FLAT_MASK_ON   548
#define REPLY_LOAD_INT_IMG   569
#define REPLY_AUTO_INT_IMG   577
#define REPLY_SEQ_STATE      506
#define REPLY_SEQ_ERR_LOG    507
#define REPLY_SOFT_SYNC      520
#define REPLY_TRIGGER_CNT    521
#define REPLY_INT_SYNC_FRQ   544
#define REPLY_SEQ_REG        522
#define REPLY_LIGHT_PLS_DR   523
#define REPLY_TRIG_INFO      552
#define REPLY_TRIG_DELAY     561
#define REPLY_TRIG_DIVIDE    564
#define REPLY_ACTIVE_SEQ     571
#define REPLY_AUTO_START     572
#define REPLY_DMD_MIR_SHK    595
#define REPLY_TRIG_SOURCE    560
#define REPLY_ILLUM_LED      596
#define REPLY_NETWORK_SET    508
#define REPLY_FW_VERSION     514
#define REPLY_DMD_INFO       515
#define REPLY_ERROR_STATUS   576

#define ERROR_PARSE_SEQ_FILE 10003

#define IMAGE_TYPE_BINARY  1
#define IMAGE_TYPE_RLE     2

#define SEQUENCER_RUN      1
#define SEQUENCER_RESET    2

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

long int char_to_int32_be(char const*);
unsigned long int char_to_uint32_be(char const*);

short int char_to_int16_be(char const*);
unsigned short int char_to_uint16_be(char const*);

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

class Payload;

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

class UDPPayload
{
 public:
  
  UDPPayload(const char*);
  UDPPayload(unsigned short int, Payload* pl = 0);
  UDPPayload(const UDPPayload&);
  virtual ~UDPPayload();

  inline unsigned short int getSize() { return tot_size; }
  inline unsigned short int getRecID() { return rec_id; }

  inline Payload* getPayload() { return payload; }
  

  virtual void toBytes(char*);
  
 protected:

  unsigned short int tot_size;
  unsigned short int rec_id;

  Payload* payload;
};
  
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

class Payload
{
 public:
  
  Payload() {}
  virtual ~Payload() {}
  
  virtual int size() { return 0; }
  virtual int dataSize() { return 0; }
  virtual Payload* clone() { return new Payload(); }
  
  virtual void toBytes(char*) {}
};

/*-----------------------------------------------------------------------------------------------*/

class Char : public Payload
{
 public:
  
  Char(const char*);
  Char(char ch) { char_value = ch; }
  Char(const Char&);
  ~Char() {}
  
  int size() { return 1; }
  Char* clone() { return new Char(char_value); }

  void toBytes(char*);
    
 protected:
  
  char char_value;
};  

/*-----------------------------------------------------------------------------------------------*/

class UnsignedShortInt : public Payload
{
 public:
  
  UnsignedShortInt(const char*);
  UnsignedShortInt(unsigned short int si) { unsigned_short_int = si; }
  UnsignedShortInt(const UnsignedShortInt&);
  ~UnsignedShortInt() {}
  
  int size() { return 2; }
  UnsignedShortInt* clone() { return new UnsignedShortInt(unsigned_short_int); }

  void toBytes(char*);
    
 protected:
  
  unsigned short int unsigned_short_int;
};  

/*-----------------------------------------------------------------------------------------------*/

class UnsignedLongInt : public Payload
{
 public:
  
  UnsignedLongInt(const char*);
  UnsignedLongInt(unsigned short int li) { unsigned_long_int = li; }
  UnsignedLongInt(const UnsignedLongInt&);
  ~UnsignedLongInt() {}
  
  int size() { return 4; }
  UnsignedLongInt* clone() { return new UnsignedLongInt(unsigned_long_int); }

  void toBytes(char*);
    
 protected:
  
  unsigned long int unsigned_long_int;
};  

/*-----------------------------------------------------------------------------------------------*/

class Ack : public UnsignedShortInt
{
 public:
  
 Ack(const char* buf) : UnsignedShortInt(buf) {}
  Ack(unsigned short int si) : UnsignedShortInt(si) {}
 Ack(const Ack& ack) : UnsignedShortInt(ack) {}
  ~Ack() {}
  
  inline unsigned short int getErrorCode() { return unsigned_short_int; }
  
};  

/*-----------------------------------------------------------------------------------------------*/

class InumSize : public UnsignedShortInt
{
 public:

  InumSize(const char* buf) : UnsignedShortInt(buf) {}
  InumSize(unsigned short int si) : UnsignedShortInt(si) {}
  InumSize(const InumSize& ins) : UnsignedShortInt(ins) {}
  ~InumSize() {}
  
 inline unsigned short int getRows() { return unsigned_short_int; }

};

/*-----------------------------------------------------------------------------------------------*/

class ImageType : public UnsignedShortInt
{
 public:
  
  ImageType(const char* buf) : UnsignedShortInt(buf) {}
  ImageType(unsigned short int si) : UnsignedShortInt(si) {}
  ImageType(const ImageType& it) : UnsignedShortInt(it) {}
  ~ImageType() {}
  
 inline unsigned short int getImageType() { return unsigned_short_int; }
  
};

/*-----------------------------------------------------------------------------------------------*/

class ImageData : public Payload
{
 public:

  ImageData(const int,const char*);
  ImageData(const unsigned short int
	    ,const unsigned long int
	    ,const unsigned long int
	    ,const size_t
	    ,const char*);
  ImageData(const ImageData&);
  ~ImageData();

  int size() { return 2 + 4 + 4 + length; }
  int dataSize() { return length; }
  ImageData* clone() { return new ImageData(*this); }

  inline unsigned short int getSeqNo() { return seq_no; }
  inline unsigned long int getInum() { return inum; }
  inline unsigned long int getOffset() { return offset; }
  
  int getData(size_t&,char*&);

  void toBytes(char*);
  
 protected:

  unsigned short int seq_no;
  unsigned long int inum;
  unsigned long int offset;

  size_t length;
  char* data;
};


/*-----------------------------------------------------------------------------------------------*/

class SeqNoError : public Payload
{
 public:

  SeqNoError(const char*);
  SeqNoError(const bool ie
	     ,const unsigned short int lsn) { ie ? is_error = (char)1 : is_error = (char)0, last_seq_no = lsn; } 
  SeqNoError(const SeqNoError&);
  ~SeqNoError();

  int size() { return 1 + 2; }

  void toBytes(char*);
  SeqNoError* clone() { return new SeqNoError(*this); }
  
  inline bool getIsError() { return (is_error == 1); }
  inline unsigned short int getLastSeqNo() { return last_seq_no; }
  
 protected:
  
  char is_error;
  unsigned short int last_seq_no;
};

/*-----------------------------------------------------------------------------------------------*/

class FlatnessCorrectionMask : public Payload
{
 public:
  
  FlatnessCorrectionMask(const unsigned short int
			 ,const unsigned short int
			 ,const unsigned short int
			 ,const char*);
  FlatnessCorrectionMask(const FlatnessCorrectionMask&);
  ~FlatnessCorrectionMask();

  void toBytes(char*);
  
  int size() { return 2 + 2 + 2 + data_size; }
  int dataSize() { return data_size; }
  FlatnessCorrectionMask* clone() { return new FlatnessCorrectionMask(*this); }
  
 protected:

  unsigned short int pkg_no;
  unsigned short int tot_pkg;
  unsigned short int data_size;

  char* data;
};

/*-----------------------------------------------------------------------------------------------*/

class EnableFCM : public Char
{
 public:
  
  EnableFCM(const char* buf) : Char(buf) {}
 EnableFCM(bool ch) : Char(ch ? (char)1 : (char)0) {}
  EnableFCM(const EnableFCM& fcm) : Char(fcm) {}
  ~EnableFCM() {}
  
  inline bool getEnable() { return (char_value == 1); }
  
};

/*-----------------------------------------------------------------------------------------------*/

class InternalImage : public Char
{
 public:
  
  InternalImage(const char* buf) : Char(buf) {}
  InternalImage(char ch) : Char(ch) {}
  InternalImage(const InternalImage& ii) : Char(ii) {}
  ~InternalImage() {}
  
  inline char getImageNo() { return char_value; } // image no 0-4, 99 if no image loaded
  
};

/*-----------------------------------------------------------------------------------------------*/

class AutoloadInternalImage : public Payload
{
 public:

  AutoloadInternalImage(const char*);
  AutoloadInternalImage(const bool al
			,const char in) { al ? autoload = (char)1 : (char)0; img_no = in; } 
  AutoloadInternalImage(const AutoloadInternalImage&);
  ~AutoloadInternalImage();

  int size() { return 1 + 1; }
  AutoloadInternalImage* clone() { return new AutoloadInternalImage(*this); }
  
  void toBytes(char*);
  
  inline bool getAutoload() { return (autoload == 1); }
 inline unsigned short int getImageNo() { return img_no; }

 protected:

  char autoload;
  char img_no;
};

/*-----------------------------------------------------------------------------------------------*/

class SequenceFile : public Payload
{
 public:

  SequenceFile(const unsigned short int
		   ,const unsigned short int
		   ,const unsigned short int
		   ,const char*);
  SequenceFile(const SequenceFile&);
  ~SequenceFile();

  int size() { return 2 + 2 + 2 + data_size; }
  int dataSize() { return data_size; }
  SequenceFile* clone() { return new SequenceFile(*this); }
  
  void toBytes(char*);
  
 protected:

  unsigned short int pkg_no;
  unsigned short int tot_pkg;
  unsigned short int data_size;

  char* data;
};

/*-----------------------------------------------------------------------------------------------*/

class SequencerState : public Payload
{
 public:

  SequencerState(const char*);
  SequencerState(const char sc
         ,const char en = 2
         ,const char vc = 2) { seq_cmd = sc; enable = en; valid_cmd = vc; }
  SequencerState(const SequencerState&);
  ~SequencerState();

  int size() { return 1 + (enable > 1 ? 0 : 1) + (valid_cmd > 1 ? 0 : 1); }
  SequencerState* clone() { return new SequencerState(*this); }
  
  void toBytes(char*);
  
  inline char getSeqCmd() { return seq_cmd; }
  inline bool getEnable() { return (enable != 1); }
  inline bool getValidCmd() { return (valid_cmd != 1); }
  
 protected:
  
  char seq_cmd;
  char enable;
  char valid_cmd;

};

/*-----------------------------------------------------------------------------------------------*/

class ErrorLog : public Payload
{
 public:

  ErrorLog(const char*);
  ErrorLog(const unsigned short int
	   ,const char*);
  ErrorLog(const ErrorLog&);
  ~ErrorLog();

  int size() { return 2 + data_size; }
  int dataSize() { return data_size; }
  char* getErrorLog();

  ErrorLog* clone() { return new ErrorLog(*this); }
  
  void toBytes(char*);

  inline bool isError() { return (data_size != 0); }
    
 protected:

  unsigned short int data_size;

  char* data;
};

/*-----------------------------------------------------------------------------------------------*/

class Sync : public Char
{
 public:
  
  Sync(const char* buf) : Char(buf) {}
 Sync(bool sync) : Char(sync ? (char)1 : (char)0) {}
  Sync(const Sync& ii) : Char(ii) {}
  ~Sync() {}
  
  inline bool getSync() { return (char_value == 1); } // image no 0-4, 99 if no image loaded
  
};

/*-----------------------------------------------------------------------------------------------*/

class TriggerCount : public UnsignedShortInt
{
 public:
  
 TriggerCount(const char* buf) : UnsignedShortInt(buf) {}
  TriggerCount(unsigned short int si) : UnsignedShortInt(si) {}
 TriggerCount(const TriggerCount& ack) : UnsignedShortInt(ack) {}
  ~TriggerCount() {}
  
  inline unsigned short int getTriggerCount() { return unsigned_short_int; }
  
};  

/*-----------------------------------------------------------------------------------------------*/

class InternalSyncFreq : public Payload
{
 public:

  InternalSyncFreq(const char*);
  InternalSyncFreq(const unsigned short int s
		   ,const unsigned short int m
		   ,const unsigned short int u) { sec = s; ms = m; us = u; }
  InternalSyncFreq(const InternalSyncFreq& isf) { sec = isf.sec; ms = isf.ms; us = isf.us; }
  ~InternalSyncFreq() {}

  int size() { return 2 + 2 + 2; }
  InternalSyncFreq* clone() { return new InternalSyncFreq(*this); }
  
  void toBytes(char*);

  inline unsigned short int getSec() { return sec; }
  inline unsigned short int getMs() { return ms; }
  inline unsigned short int getUs() { return us; }
  
 protected:

  unsigned short int sec;
  unsigned short int ms;
  unsigned short int us;
};

/*-----------------------------------------------------------------------------------------------*/

class SequenceReg : public Payload
{
 public:

  SequenceReg(const char*);
  SequenceReg(const unsigned short int rn
	      ,const unsigned short int rv = 65535
              ,const char vr = 127) { reg_no = rn; reg_val = rv; valid_reg = vr; }
  SequenceReg(const SequenceReg& sr) { reg_no = sr.reg_no; reg_val = sr. reg_val; valid_reg = sr.valid_reg; }
  ~SequenceReg() {}

  int size() {
      int num = (valid_reg == 127) ? 0 : 1;
      return 2 + (reg_val == 65535 ? 0 : (2 + num));
  }
  
  void toBytes(char*);
  SequenceReg* clone() { return new SequenceReg(*this); }
  
  inline unsigned short int getRegNo() { return reg_no; }
  inline unsigned short int getRegVal() { return reg_val; }
  inline bool getValidReg() { return (valid_reg == 1); }
  
 protected:
  
  unsigned short int reg_no;
  unsigned short int reg_val;
  char valid_reg;
};

/*-----------------------------------------------------------------------------------------------*/

class LightPulseDuration : public UnsignedShortInt
{
 public:
  
 LightPulseDuration(const char* buf) : UnsignedShortInt(buf) {}
 LightPulseDuration(unsigned short int si) : UnsignedShortInt(si) {}
 LightPulseDuration(const LightPulseDuration& ack) : UnsignedShortInt(ack) {}
  ~LightPulseDuration() {}
  
  inline unsigned short int getDuration() { return unsigned_short_int; }
  
};  

/*-----------------------------------------------------------------------------------------------*/

class TrigInfo : public Payload
{
 public:
  
  TrigInfo(const char*);
  TrigInfo(const TrigInfo& sr);
  ~TrigInfo() {}
  
  int size() { return 4 + 4 + 2 + 2 + 2; }
  TrigInfo* clone() { return new TrigInfo(*this); }
  
  void toBytes(char*);

  inline unsigned long int getInputPulseElectrical() { return in_puls_e; }
  inline unsigned long int getInputPulseOptical() { return in_puls_o; }
  inline unsigned short int getMinPeriodElectrical() { return min_per_e; }
  inline unsigned short int getMinPeriodOptical() { return min_per_o; }
  inline unsigned short int getNumSeqPulses() { return seq_puls; }
  
 protected:

  unsigned long int in_puls_e;
  unsigned long int in_puls_o;
  unsigned short int min_per_e;
  unsigned short int min_per_o;
  unsigned short int seq_puls;

};

/*-----------------------------------------------------------------------------------------------*/

class TrigDelay : public UnsignedShortInt
{
 public:
  
 TrigDelay(const char* buf) : UnsignedShortInt(buf) {}
  TrigDelay(unsigned short int si) : UnsignedShortInt(si) {}
 TrigDelay(const TrigDelay& ack) : UnsignedShortInt(ack) {}
  ~TrigDelay() {}
  
  inline unsigned short int getTrigDelay() { return unsigned_short_int; }
  
};  

/*-----------------------------------------------------------------------------------------------*/

class TrigDivide : public UnsignedShortInt
{
 public:
  
 TrigDivide(const char* buf) : UnsignedShortInt(buf) {}
  TrigDivide(unsigned short int si) : UnsignedShortInt(si) {}
 TrigDivide(const TrigDivide& ack) : UnsignedShortInt(ack) {}
  ~TrigDivide() {}
  
  inline unsigned short int getTrigDivide() { return unsigned_short_int; }
  
};  

/*-----------------------------------------------------------------------------------------------*/

class ActiveSequence : public Char
{
 public:
  
  ActiveSequence(const char* buf) : Char(buf) {}
  ActiveSequence(char ch) : Char(ch) {}
  ActiveSequence(const ActiveSequence& ii) : Char(ii) {}
  ~ActiveSequence() {}
  
  inline char getActiveSequence() { return char_value; } // image no 0-4, 99 if no image loaded
  
};

/*-----------------------------------------------------------------------------------------------*/

class AutostartSequencer : public Char
{
 public:
  
  AutostartSequencer(const char* buf) : Char(buf) {}
  AutostartSequencer(char ch) : Char(ch) {}
  AutostartSequencer(const AutostartSequencer& ii) : Char(ii) {}
  ~AutostartSequencer() {}
  
  inline bool getAutostartSequencer() { return (char_value > 0); } 
  
};

/*-----------------------------------------------------------------------------------------------*/

class DmdMirrorShake : public Char
{
 public:
  
  DmdMirrorShake(const char* buf) : Char(buf) {}
  DmdMirrorShake(char ch) : Char(ch) {}
  DmdMirrorShake(const DmdMirrorShake& ii) : Char(ii) {}
  ~DmdMirrorShake() {}
  
  inline char getDmdMirrorShake() { return char_value; } 
  
};

/*-----------------------------------------------------------------------------------------------*/

class TrigSource : public Payload
{
 public:
  
  TrigSource(const char*);
  TrigSource(bool,bool);
  TrigSource(const TrigSource&);
  ~TrigSource() {}
  
  int size() { return 2 + 2; }
  
  void toBytes(char*);
  
  inline bool getOptical() { return (source == 1); }
  inline bool getNegativeEdge() { return (edge == 1); }
  
 protected:
  
  unsigned short int source;
  unsigned short int edge;

};

/*-----------------------------------------------------------------------------------------------*/

class IllumLed : public Char
{
 public:
  
  IllumLed(const char* buf) : Char(buf) {}
  IllumLed(char ch) : Char(ch) {}
  IllumLed(const IllumLed& ii) : Char(ii) {}
  ~IllumLed() {}
  
  inline char getIllumVal() { return char_value; } 
  
};

/*-----------------------------------------------------------------------------------------------*/

class NetworkSettings : public Payload
{
 public:

  NetworkSettings(const char*);
  NetworkSettings(const unsigned long int
		   ,const unsigned long int
		   ,const unsigned long int
		   ,const bool);
  NetworkSettings(const NetworkSettings&);
  ~NetworkSettings();

  int size() { return 4 + 4 + 4 + 1; }
  NetworkSettings* clone() { return new NetworkSettings(*this); }
  
  void toBytes(char*);

  inline unsigned long int getIpAddr() { return ip_addr; }
  inline unsigned long int getSubnet() { return subnet; }
  inline unsigned long int getGateway() { return gateway; }
  inline bool getDHCP() { return (dhcp == 1); }
  
  
 protected:

  unsigned long int ip_addr;
  unsigned long int subnet;
  unsigned long int gateway;
  char dhcp;
};

/*-----------------------------------------------------------------------------------------------*/

class FwVersion : public Payload
{
 public:
  
  FwVersion(const char*);
  FwVersion(const FwVersion&);
  ~FwVersion() {}
  
  int size() { return 2 + 2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 2 + 1 + 1 + 1 + 1 + 1; }
  FwVersion* clone() { return new FwVersion(*this); }
  
  void toBytes(char*);

  inline unsigned short int getSysFWMajor() { return sys_fw_major; }
  inline unsigned short int getSysFWMinor() { return sys_fw_minor; }
  inline unsigned short int getAppSwVersion() { return app_sw_ver; }
  inline unsigned short int getAppSwYear() { return app_sw_year; }

  inline char getAppSwMonth() { return app_sw_month; }
  inline char getAppSwDay() { return app_sw_day; }
  inline char getAppSwHour() { return app_sw_hour; }
  inline char getAppSwMin() { return app_sw_min; }
  inline char getAppSwSec() { return app_sw_sec; }

  inline unsigned short int getFPGAYear() { return FPGA_year; }

  inline char getFPGAMonth() { return FPGA_month; }
  inline char getFPGADay() { return FPGA_day; }
  inline char getFPGAHour() { return FPGA_hour; }
  inline char getFPGAMin() { return FPGA_min; }
  inline char getFPGASec() { return FPGA_sec; }

 protected:

  unsigned short int sys_fw_major;
  unsigned short int sys_fw_minor;
  unsigned short int app_sw_ver;
  unsigned short int app_sw_year;
  char app_sw_month;
  char app_sw_day;
  char app_sw_hour;
  char app_sw_min;
  char app_sw_sec;
  unsigned short int FPGA_year;
  char FPGA_month;
  char FPGA_day;
  char FPGA_hour;
  char FPGA_min;
  char FPGA_sec;

};

/*-----------------------------------------------------------------------------------------------*/

class DmdInfo : public Payload
{
 public:
  
  DmdInfo(const char*);
  DmdInfo(const DmdInfo&);
  ~DmdInfo() {}
  
  int size() { return 2 + 2 + 2 + 2 + 2; }
  DmdInfo* clone() { return new DmdInfo(*this); }
  
  void toBytes(char*);

  inline unsigned short int getDmdType() { return dmd_type; }
  inline unsigned short int getBlocks() { return blocks; }
  inline unsigned short int getCols() { return cols; }
  inline unsigned short int getRows() { return rows; }
  inline unsigned short int getRowsPerBlock() { return rows_pr_block; }

 protected:

  unsigned short int dmd_type;
  unsigned short int blocks;
  unsigned short int cols;
  unsigned short int rows;
  unsigned short int rows_pr_block;

};

/*-----------------------------------------------------------------------------------------------*/

class ErrorStatus : public UnsignedLongInt
{
 public:
  
 ErrorStatus(const char* buf) : UnsignedLongInt(buf) {}
  ErrorStatus(unsigned long int li) : UnsignedLongInt(li) {}
 ErrorStatus(const ErrorStatus& ack) : UnsignedLongInt(ack) {}
  ~ErrorStatus() {}
  
  inline unsigned long int getErrorStatus() { return unsigned_long_int; }
  
};  

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

namespace UDP_Helper
{
  UDPPayload* ReplyAck(char*);
  
  UDPPayload* SetInumSize(unsigned short int);
  UDPPayload* RequestInumSize();
  UDPPayload* ReplyInumSize(char*);
  
  UDPPayload* SetImageType(unsigned short int);
  UDPPayload* RequestImageType();
  UDPPayload* ReplyImageType(char*);
  
  UDPPayload** LoadImageData(unsigned long int
			     ,int&,size_t,const char*);
  
  UDPPayload* RequestSeqNoError();
  UDPPayload* ReplySeqNoError(char*);
  
  UDPPayload* ResetSeqNo();
  
  UDPPayload** LoadFlatnessCorrectionMask(unsigned short int
					  ,int&,const char*);
  
  UDPPayload* SetFlatnessMaskOn(bool);
  UDPPayload* RequestFlatnessMaskOn();
  UDPPayload* ReplyFlatnessMaskOn(char*);
  
  UDPPayload* SetLoadInternalImage(char);
  UDPPayload* RequestLoadInternalImage();
  UDPPayload* ReplyLoadInternalImage(char*);
  
  UDPPayload* SetAutoloadInternalImage(bool,char);
  UDPPayload* RequestAutoloadInternalImage();
  UDPPayload* ReplyAutoloadInternalImage(char*);
  
  UDPPayload* SetSaveInternalImage(char);
  
  UDPPayload** LoadSequenceFile(unsigned short int
				,int&,const char*);
  
  UDPPayload* SetSequencerState(char,bool);
  UDPPayload* RequestSequencerState(char);
  UDPPayload* ReplySequencerState(char*);
  
  UDPPayload* RequestSeqFileErrorLog();
  UDPPayload* ReplySeqFileErrorLog(char*);

  UDPPayload* SetSoftwareSync(bool);
  UDPPayload* RequestSoftwareSync();
  UDPPayload* ReplySoftwareSync(char*);

  UDPPayload* RequestTriggerCount();
  UDPPayload* ReplyTriggerCount(char*);

  UDPPayload* SetInternalSyncFreq(unsigned short int, unsigned short int, unsigned short int);
  UDPPayload* RequestInternalSyncFreq();
  UDPPayload* ReplyInternalSyncFreq(char*);
  
  UDPPayload* SetSequenceReg(unsigned short int,unsigned short int);
  UDPPayload* RequestSequenceReg(unsigned short int);
  UDPPayload* ReplySequenceReg(char*);

  UDPPayload* SetLightPulseDuration(unsigned short int);
  UDPPayload* RequestLightPulseDuration();
  UDPPayload* ReplyLightPulseDuration(char*);
  
  UDPPayload* RequestTrigInfo();
  UDPPayload* ReplyTrigInfo(char*);
  
  UDPPayload* SetTrigDelay(unsigned short int);
  UDPPayload* RequestTrigDelay();
  UDPPayload* ReplyTrigDelay(char*);
  
  UDPPayload* SetTrigDivide(unsigned short int);
  UDPPayload* RequestTrigDivide();
  UDPPayload* ReplyTrigDivide(char*);
  
  UDPPayload* SetActiveSequence(char);
  UDPPayload* RequestActiveSequence();
  UDPPayload* ReplyActiveSequence(char*);
  
  UDPPayload* SetAutostartSequencer(bool);
  UDPPayload* RequestAutostartSequencer();
  UDPPayload* ReplyAutostartSequencer(char*);
  
  UDPPayload* SetDmdMirrorShake(bool);
  UDPPayload* RequestDmdMirrorShake();
  UDPPayload* ReplyDmdMirrorShake(char*);
 
  UDPPayload* SetTrigSource(bool,bool);
  UDPPayload* RequestTrigSource();
  UDPPayload* ReplyTrigSource(char*);
  
  UDPPayload* SetIllumLed(char);
  UDPPayload* RequestIllumLed();
  UDPPayload* ReplyIllumLed(char*);

  UDPPayload* SetNetworkSettings(unsigned long int,unsigned long int,unsigned long int,bool);
  UDPPayload* RequestNetworkSettings();
  UDPPayload* ReplyNetworkSettings(char*);
  
  UDPPayload* SaveSettings();
  
  UDPPayload* SetFactoryDefaults();
  
  UDPPayload* SetSafeShutdown();
  
  UDPPayload* RequestFwVersion();
  UDPPayload* ReplyFwVersion(char*);
  
  UDPPayload* RequestDmdInfo();
  UDPPayload* ReplyDmdInfo(char*);
  
  UDPPayload* RequestErrorStatus();
  UDPPayload* ReplyErrorStatus(char*);
  
  UDPPayload* SetClearErrorStatus();
}

/*-----------------------------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/



#endif
