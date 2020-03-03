
#ifndef atDataBufferH
#define atDataBufferH

#include <stdio.h>
#include "atError.h"
#include "atlib\PCCards\at_X3.h"

class atDataBuffer {
 protected:
	unsigned char *frame;
	unsigned frameLen;    //wielkoœæ ramki
	unsigned dataLen;     //wskaŸnik danych, u¿ywany np do wskazania ile jest danych u¿ytecznych w buforze
	atError *Error;
	uint8_t nNag;   //d³ugoœæ nag³ówka dla ramki.
	uint8_t id;

 public:
	atDataBuffer(){frame=NULL;frameLen=0;Error=NULL;dataLen=0;nNag=0;}
	atDataBuffer(unsigned int fL, uint8_t n){frame=NULL;Error=NULL;Ini(fL,n,0);}
	atDataBuffer(unsigned int fL, uint8_t n,uint8_t i){frame=NULL;Error=NULL;Ini(fL,n,i); }
	~atDataBuffer(){if(frame!=NULL)delete []frame;if(Error!=NULL)delete Error;};
	void Ini(unsigned int fL, uint8_t n,uint8_t i);
	void buforClear() {dataLen=0;}
	void setDataLen(unsigned dL){dataLen=dL;};
	unsigned getDataLen(){return dataLen;};
	void updateDataLen(unsigned dL){if(dL>dataLen)dataLen=dL;};
	unsigned getFrameLen(){return frameLen;};
	unsigned char* getFrame(){return frame;};
	unsigned char* getBytesFrom(unsigned i){return frame+i;};

	void setByte(unsigned i, char b)               {  frame[i]=b;  };
	void setReg(unsigned i, unsigned int b)        {  frame[i]=(char)((b>>8)&0xFF); frame[i+1]=(char)(b&0xFF); };
	void setInt(unsigned i, int b)                 {  frame[i]=(char)((b>>8)&0xFF); frame[i+1]=(char)(b&0xFF); };
	void setReg32(unsigned i, unsigned long  int b){  frame[i]=(char)((b>>24)&0xFF); frame[i+1]=(char)((b>>16)&0xFF); frame[i+2]=(char)((b>>8)&0xFF); frame[i+3]=(char)(b&0xFF); };
	void buforSetNag( char b)                                    { frame[0]=id; frame[1]=b; dataLen=2; };
	void buforSetNag( char i, char b)                            { frame[0]=i;  frame[1]=b; dataLen=2; };
	void buforSetNag( char b,char d,char a,char l)               { frame[0]=id; frame[1]=b; frame[2]=d;frame[3]=a; setReg(4,l); dataLen=6; };
	void buforSetNag( char i, char b,char d,char a,char l)       { frame[0]=i;  frame[1]=b; frame[2]=d;frame[3]=a; setReg(4,l); dataLen=6; };
	void buforAddByte(char c)               { if(dataLen<frameLen){ setByte(dataLen,c);dataLen++; } else{SetError(atMODBUS_ERROR_OVERFLOW);} }
	void buforAddReg( short unsigned int c) { if((dataLen+1)<frameLen){ setReg(dataLen,c); dataLen+=2; } else{SetError(atMODBUS_ERROR_OVERFLOW);} }
	void buforAddReg32(long unsigned int c) { if((dataLen+3)<frameLen){ setReg32(dataLen,c); dataLen+=4;} else{SetError(atMODBUS_ERROR_OVERFLOW);}   }
	void buforAddStr(char* c)               { while( dataLen<frameLen && (*c!=0) ){ buforAddByte(*c); c+=1; } }
	bool Function(char f)                   { return frame[1]==f; }
	bool DeviceAddress(char a)              { return frame[2]==a; }
	bool checkErrorFrame()                  { return (frame[1]&0x80); } //w innych versjach by³o: frame[1]&0xF0==0x80;, ale wydaje siê byæ s³uszniejsze

	void buforOnBit(int r, int b)               { if(r<frameLen && b<8) {frame[r]|=(1<<b);} }                      //jw ..zapla bit b w bajcie r
	void buforOffBit(int r, int b)              { if(r<frameLen && b<8) {frame[r]&=(~(1<<b));} }                     //jw
	void buforSetBit(int r, int b,uint8_t v)    {if(v)buforOnBit(r,b);else buforOffBit(r,b);}           //
	void buforRegOnBit(int r, int b)            { if(b<8) r++; else b-=8; buforOnBit(r,b);  }       //jw ale w rejestrze 16 bitowym
	void buforRegOffBit(int r, int b)           { if(b<8) r++; else b-=8; buforOffBit(r,b); }       //..
	void buforRegSetBit(int r, int b,uint8_t v) { if(b<8) r++; else b-=8; buforSetBit(r,b,v); }     //..
	void buforRegSetBit( int b,uint8_t v)       { uint8_t r=dataLen-2; if(b<8) r++; else b-=8; buforSetBit(r,b,v); }     //ustawienie bitu na ostatnim rejestrze (..dataLen-2)
	void buforSetCRC(){buforSetCRC(dataLen-2);}                 //zmiana (nadpisanie)sumy kontrolnej w buforze ramki. suma zapisywana jest w bajtach: dataLen-2 i dataLen-1
	void buforSetCRC(uint8_t l);                             //zmiana (nadpisanie)sumy kontrolnej w buforze ramki. suma zapisywana jest w bajtach: l, l+1
	void buforAddCRC(){buforSetCRC(dataLen);dataLen+=2;}           //dodanie sumy kontrolnej do bufora ramki (bajty dataLen i dataLen+1)

	uint8_t  modbusGetByte(uint8_t i)            {return buforGetByte(  nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}
	uint8_t  modbusGetRegHi(uint8_t i)           {return buforGetByte(  nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}    //odczyt bajtu HI wed³ug numeracji rejestrów modbus
	uint8_t  modbusGetRegLo(uint8_t i)           {return buforGetByte(  nNag+1+i-buforGetByte(AT_MODBUS_REGOFFSET));}  //odczyt bajtu LO wed³ug numeracji rejestrów modbus
	uint16_t modbusGetReg(uint8_t i)             {return buforGetReg(   nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}       //odczyt rejestru..
	uint32_t modbusGetReg32(uint8_t i)           {return buforGetReg32( nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}   //odczyt reg32..
	uint8_t  modbusGetBit(uint8_t i,uint8_t j)   {return buforGetBit(   nNag+i-buforGetByte(AT_MODBUS_REGOFFSET),j);} //odczyt bitu ..ale w rejestrze 16bitowym
	uint8_t  modbusGetRegBit(uint8_t i,uint8_t j){return buforGetRegBit(nNag+i-buforGetByte(AT_MODBUS_REGOFFSET),j);} //odczyt bitu ..ale w rejestrze 16bitowym
	int      modbusGetInt(uint8_t i)           	 {return getInt(        nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}       //odczyt rejestru..
	int      modbusGetInt32(uint8_t i)         	 {return getInt32(      nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}   //odczyt reg32..

	uint8_t  buforGetByte(int i){	return i<dataLen?frame[i]:0;}                             //odczyt jednego bajtu z ramki
	uint16_t buforGetReg(int i){  return (((uint16_t)frame[i])<<8) + frame[i+1]; }                              //odczyt s³owa (rejestru) z bufora ramki
	uint32_t buforGetReg32(int i){  return (((uint32_t)frame[i])<<24) + (((uint32_t)frame[i+1])<<16) + (((uint32_t)frame[i+2])<<8) + frame[i+3]; }                            //odczyt reg32 z bufora ramki
	uint8_t  buforGetBit(int i,int j){ return (frame[i]>>j) & 1 ; }                        //odczyt bitu z bufora ramki
	uint8_t  buforGetRegBit(uint8_t i,uint8_t j){ uint8_t r=0; if(j<8)r++;else j-=8; return buforGetBit(i+r,j);} //odczyt bitu ..ale w rejestrze 16bitowym................sprawdŸ!
	uint16_t buforGetCRC( int len){return getCRC(len);}                           //funkcja oblicza sumê kontroln¹ na buforze ramki (..z "len" pierwszych bajtów, bajty o numerze "len" i "len+1" przeznaczone s¹ na sumê kontroln¹)

	void modbusSetDataLen(atDataBuffer *db,int l)			{ for(int i=0;i<3;i++)frame[i]=db->getByte(i); for(int i=3;i<l;i++)frame[i]=0; dataLen=l;}
	void modbusSetRespNag(atDataBuffer *db)             	{ for(int i=0;i<3;i++)frame[i]=db->getByte(i); for(int i=3;i<nNag;i++)frame[i]=0; dataLen=nNag;}  //pierwsze 3 bajty zostaj¹, zerujemy pozosta³e bajty nag³ówka
	void modbusSetByte( uint8_t i, uint8_t c)				{ setByte( 		nNag+i  ,c);}
	void modbusSetHi( uint8_t i, uint8_t c)  				{ setByte(		nNag+i  ,c);}
	void modbusSetLo( uint8_t i, uint8_t c)					{ setByte(		nNag+1+i,c);}
	void modbusSetReg( uint8_t i, uint16_t c)				{ setReg(		nNag+i  ,c);}
	void modbusSetReg32( uint8_t i, uint32_t c)				{ setReg32(		nNag+i  ,c);}
	void modbusSetOnBit( uint8_t i,uint8_t b)				{ buforOnBit( 	nNag+i  ,b);}
	void modbusSetOffBit( uint8_t i,uint8_t b)			  	{ buforOffBit(	nNag+i  ,b);}
	void modbusSetBit( uint8_t i, uint8_t b,uint8_t v)    	{ buforSetBit(	nNag+i  ,b,v);}
	void modbusSetRegBit( uint8_t i, uint8_t b,uint8_t v) 	{ buforRegSetBit(	nNag+i  ,b,v);}
	void modbusSetCRC(uint8_t i)						  	{ buforSetCRC(nNag+i); dataLen=nNag+i+2;}


	unsigned char getByte(unsigned i)             { return frame[i];};
	unsigned int getReg(unsigned i)               { return (((unsigned int)frame[i])<<8) + frame[i+1]; }
	long unsigned int getReg32(unsigned i)        { return (((long unsigned int)frame[i])<<24) + (((long unsigned int)frame[i+1])<<16) + (((long unsigned int)frame[i+2])<<8) + frame[i+3]; }
	unsigned char getBit(unsigned i,unsigned j)   { return (frame[i]>>j)&(char)1;};
	unsigned char getRegBit(unsigned i,unsigned j){ if(i<8) j++; else i-=8;   return getBit(i,j);};
	int getInt(unsigned i)                        { short int v=0; 	v|=frame[i]; v=v<<8;  v|=frame[i+1];  return v;  }
	int getInt32(unsigned i)                      { int v=0;  v|=frame[i]; v=v<<8;  v|=frame[i+1]; v=v<<8;  v|=frame[i+2]; v=v<<8;  v|=frame[i+3];  return v;  }

	atError *GetError(){return Error;};
	void SetError(int e){ SetError(e,""); };
	void SetError(int e, string s){ if(!Error) Error=new atError(e,"",NULL,NULL,s);	 };
	void ClearError(){ if(!Error) delete Error; Error=NULL; };

	unsigned short int getCRC( int len );
	unsigned short int getCRC(  ){return getCRC(dataLen-2);};
	bool checkCRC( int len );
	bool checkCRC(){if(dataLen<2){return false;} return checkCRC(dataLen-2);}
	AnsiString HexFrame(){ return CharTabToHex(frame,dataLen); }
	
};

#endif
