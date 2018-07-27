/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
+                           HDLCCodec.h                            +
+     --------------------------------------------------------     +
+                                                                  +
+ժҪ: HDLC(High level Data Link Control������                     +
+����: Sharper Huang                                               +
+����: 2011-7-14 0:31:51                                           +
+  (C) Hangzhou Synway software., ltd                              +
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#ifndef __HDCLCODEC_H__
#define __HDCLCODEC_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define  MIN_FRAME_BUFFER 16
#define  DEF_FRAME_BUFFER 256
#define  MAX_FRAME_BUFFER 4096

//#define  HDLC_FRAME_END    0 //�ļ�����
#define  HDLC_FRAME_DATA   1 //����֡
#define  HDLC_FRAME_ABORT  2 //�쳣֡
#define  HDLC_FRAME_DONE   3 

#define  HDLC_FRAME_MAXBYTES 2048

extern unsigned char C_BYTEINVERSE_MAP[256];      //BIG-ENDIAN <==>LITTLE-ENDIAN

unsigned short int ComputeFCS (const unsigned char *szData, int nLen);
bool   CheckFCS (const unsigned char *szData, int nLen);
//-------------------------------------------------------------
//������
//-------------------------------------------------------------
//�ص���������һ֡���ݽ�����ɺ󣬵��ô˺���
typedef void (__stdcall *ONFRAMEDECODED)(int nType,const char* szFrame,int nLen,void* pUser);
class CHDLCDecode  
{
public:
	CHDLCDecode();
	virtual ~CHDLCDecode();
public:
	void StartDecode(ONFRAMEDECODED pfCB,unsigned short wBufferSize,void* pUserData);
 	int  DecodeBuffer(const char* pData,int nSize);
private:
	inline void OnFrameFlagByte(int nOffset);
	inline void OnFrameAbort();
    inline void AddBit(unsigned char ucBit);
    inline int  OnByteDecoded(unsigned char ucByte);
	void        ResetDecode(unsigned int dwBufSize=HDLC_FRAME_MAXBYTES);
private:
    unsigned int   m_nDecodedBytes;
    unsigned int   m_nBlockSize;
    unsigned char* m_pszDecodedBuf;
    unsigned char  m_ucDecodingByte;
    unsigned char  m_nHighLevelBits;
    bool           m_bFlagByte;
    unsigned int   m_nOffsetInStream;
    unsigned char  m_nDecodedBits;
    ONFRAMEDECODED m_pfCB; 
    void*  m_pUserData;

};

//-------------------------------------------------------------
//������
//-------------------------------------------------------------
typedef void (__stdcall *ONFRAMEENCODED)(int nType,const char* szBuffer,int nLen,void* pUser);
class CHDLCEncode  
{
public:
	CHDLCEncode();
	virtual ~CHDLCEncode();
//public:
//    void __stdcall CBBlockEncoded(int nType,const char* szBuffer,int nLen,void* pUser);
public:
	unsigned int EncodeFrame( const char *szRaw,
                                       unsigned int dwRawSize,
                                       char* szEncodedBuffer,
                                       unsigned int dwBufferSize);

	unsigned char GetFlagByte() const { return m_ucFlagByte;}
	unsigned char GetFlagByteInverse() const ;
	int EncodeFrameStream(int nType,const char* szBuffer,unsigned int dwLen);
    void SetPlaybackBaudRate(unsigned int nBaudRate);
    void StartEncode(ONFRAMEENCODED   pfCB, 
                                unsigned short wMaxFrameSize, 
                                void*          pUserData);

private:
	inline  void  InsertFlagByte(bool bFrameDone=false);
    __forceinline  void  EncodeByte(unsigned char ucTemp);
	inline  void  AddBit(unsigned char ucBit);
	inline  void  ResetEncode(unsigned int dwBufSize);
	inline  void  OnByteEncoded(unsigned char ucByte);
    inline  void  OnFrameEncoded();
private:
    unsigned int     m_nBytesPerSec;
    bool             m_bFlagByteInserted;
    ONFRAMEENCODED   m_pfCB;

	unsigned char    m_ucFlagByte;    //��־λ
    void*            m_pUserData;

    unsigned int     m_nBlockSize;      
    unsigned int     m_nEncodedBytes;   //�ѱ�����ֽ���
    unsigned char*   m_pszStreamEncodedBuf;   //�ѱ�����ֽڻ���

	unsigned  char   m_nHighLevelBits; //�����ߵ�ƽ��BIT 1)λ��
    unsigned  char   m_ucEncodingByte; //���ڱ�����ֽ�
    unsigned  char   m_nEncodedBits;   //�ѱ����λ��BIT)��
	//unsigned char*   
     unsigned char*   m_psResultBuf;       //�ѱ�����ֽڻ���
     unsigned int     m_nResultBufSize;    //�ѱ�����ֽڻ���
     unsigned int     m_nResultBufPtr;     //�ѱ�����ֽڻ���
};

#endif //__HDCLCODEC_H__