
#ifndef atDataBuffer_H
#define atDataBuffer_H

#include <stdio.h>
#include <stdint.h>
#include <sstream>
#include <at_X3.h>

using namespace std;


class atDataBuffer {
 protected:
    uint8_t *frame;
    unsigned frameLen;    //wielkosc ramki
    unsigned dataLen;     //wskaznik danych
    uint8_t nNag;         //dlugosc naglowka dla ramki.
    uint8_t id;

 public:
    atDataBuffer(uint8_t i)                         {id=i;frame=NULL;frameLen=0;dataLen=0;nNag=0;}
 //   atDataBuffer(uint8_t i, uint16_t fL)            {id=i;frame=NULL;Ini(fL);nNag=0;}
    atDataBuffer(uint8_t i, uint16_t fL,uint8_t n)  {id=i;frame=NULL;Ini(fL);nNag=n;}
    ~atDataBuffer()                       {if(frame!=NULL)delete []frame;}
    void Ini(uint16_t fL);
    void Clear()                     {dataLen=0;}
    void setDataLen(unsigned dL)     {dataLen=dL;}
    unsigned getDataLen()            {return dataLen;}
    unsigned getFrameLen()           {return frameLen;}
    uint8_t* getFrame()              {return frame;}
    uint8_t* getBytesFrom(unsigned i){return frame+i;}

    void setByte(unsigned i, uint8_t b)              { if(i<frameLen){ frame[i]=b;  }}
    void setReg(unsigned i, uint16_t b)              { if((i+1)<frameLen){ frame[i]=(uint8_t)((b>>8)&0xFF); frame[i+1]=(uint8_t)(b&0xFF); }}
    void setReg32(unsigned i, uint32_t b)            { if((i+3)<frameLen){ frame[i]=(uint8_t)((b>>24)&0xFF); frame[i+1]=(uint8_t)((b>>16)&0xFF); frame[i+2]=(uint8_t)((b>>8)&0xFF); frame[i+3]=(uint8_t)(b&0xFF); }}
    void setInt(unsigned i, int16_t v)                    { if((i+1)<frameLen){ frame[i]=(v>>8)&0xFF; frame[i+1]=v&0xFF;   }}
    void setInt32(unsigned i, int32_t v)                  { if((i+3)<frameLen){ frame[i]=(v>>24)&0xFF; frame[i+1]=(v>>16)&0x00FF; frame[i+2]=(v>>8)&0xFF; frame[i+3]=v&0xFF;   }}

    void buforSetNag(uint8_t fun)                    { if(frameLen>=2) frame[0]=id; frame[1]=fun; dataLen=2; }
    void buforSetNag(uint8_t f,uint8_t d,uint8_t a)  { if(frameLen>=4) frame[0]=id; frame[1]=f; frame[2]=d; frame[3]=a; dataLen=4; }
    void buforSetNag(uint8_t f,uint8_t d,uint8_t a,uint16_t l){ if(frameLen>=6) frame[0]=id; frame[1]=f; frame[2]=d; frame[3]=a; setReg(4,l); dataLen=6; }
    void buforSetNag(uint8_t i,uint8_t f,uint8_t d,uint8_t a,uint16_t l){ if(frameLen>=6) frame[0]=i; frame[1]=f; frame[2]=d; frame[3]=a; setReg(4,l); dataLen=6; }
    void buforSetNag(atDataBuffer* d)                { if(frameLen>=4) setFrame(d->getFrame(),4); dataLen=4; }
    void buforSetNag(atDataBuffer* d,uint16_t l)     { if(frameLen>=6) setFrame(d->getFrame(),4); setReg(4,l); dataLen=6; }
    void buforAddByte(uint8_t c)                     { if(dataLen<frameLen){ setByte(dataLen,c);dataLen++; }  }
    void buforAddReg( uint16_t c)                    { if((dataLen+1)<frameLen){ setReg(dataLen,c); dataLen+=2; }  }
    void buforAddReg32(uint32_t c)                   { if((dataLen+3)<frameLen){ setReg32(dataLen,c); dataLen+=4;}    }
    void buforAddInt( int16_t c)                     { if((dataLen+1)<frameLen){ setInt(dataLen,c); dataLen+=2; }  }
    void buforAddInt32(int32_t c)                    { if((dataLen+3)<frameLen){ setInt32(dataLen,c); dataLen+=4;}    }
    void buforAddStr(const uint8_t* c)               { while( dataLen<frameLen && (*c!=0) ){ buforAddByte(*c); c+=1; } }
    void buforAddStr(const char* c)                  { while( dataLen<frameLen && (*c!=0) ){ buforAddByte((uint8_t) (*c)); c+=1; } }
    void buforOnBit(unsigned r, int b)                    { if(r<frameLen && b<8) {frame[r]|=(1<<b);} }//jw ..zapala bit b w bajcie r
    void buforOffBit(unsigned r, int b)                   { if(r<frameLen && b<8) {frame[r]&=(~(1<<b));} } //jw
    void buforSetBit(unsigned r, int b,uint8_t v)    {if(v)buforOnBit(r,b);else buforOffBit(r,b);}           //
    void buforRegOnBit(unsigned r, int b)            { if(b<8) r++; else b-=8; buforOnBit(r,b);  }       //jw ale w rejestrze 16 bitowym
    void buforRegOffBit(unsigned r, int b)           { if(b<8) r++; else b-=8; buforOffBit(r,b); }       //..
    void buforRegSetBit(unsigned r, int b,uint8_t v) { if(b<8) r++; else b-=8; buforSetBit(r,b,v); }     //..
    void buforRegSetBit( unsigned b,uint8_t v)       { uint8_t r=dataLen-2; if(b<8) r++; else b-=8; buforSetBit(r,b,v); }     //ustawienie bitu na ostatnim rejestrze (..blen-2)
    void setError()                                  { setError(0);}
    void setError(uint16_t e)                        { frame[1]+=0x80; dataLen=4;  buforAddReg(e); buforAddCRC();}
    void setError(atDataBuffer* d,uint16_t e)        { setFrame(d->getFrame(),4); frame[1]+=0x80; dataLen=4; buforAddReg(e); buforAddCRC(); }
    void setFrame(uint8_t* f,uint16_t fL);
    void setFrame(atDataBuffer* d,uint16_t fL)       {setFrame(d->getFrame(),fL);}
    void setFrame(atDataBuffer* d)                   {setFrame(d->getFrame(),d->getDataLen());}

    bool checkId(uint8_t i)                          { return frame[0]==i; }
    bool checkId()                                   { return frame[0]==id; }
    bool checkFunction(uint8_t f)                    { return frame[1]==f; }
    bool checkDeviceAddress(uint8_t a)               { return frame[2]==a; }
    bool checkErrorFrame()                           { return (frame[1]&0x80); }

    uint8_t  buforGetByte(unsigned i)            { return frame[i];}
    uint16_t buforGetReg(unsigned i)             { return (((uint16_t)frame[i])<<8) + frame[i+1]; }
    uint32_t buforGetReg32(unsigned i)           { return (((uint32_t)frame[i])<<24) + (((uint32_t)frame[i+1])<<16) + (((uint32_t)frame[i+2])<<8) + frame[i+3]; }
    uint8_t  buforGetBit(unsigned i,unsigned j)  { return (frame[i]>>j)&(uint8_t)1;}
    uint8_t  buforGetRegBit(uint8_t i,uint8_t j) { uint8_t r=0; if(j<8)r++;else j-=8; return buforGetBit(i+r,j);}
    int32_t  getInt18(int i);
    int32_t  getInt24(int i);
    int      getInt(unsigned i)                  { short int v=0; 	v|=frame[i]; v=v<<8;  v|=frame[i+1];  return v;  }
    int      getInt32(unsigned i)                { int v=0;  v|=frame[i]; v=v<<8;  v|=frame[i+1]; v=v<<8;  v|=frame[i+2]; v=v<<8;  v|=frame[i+3];  return v;  }

    uint8_t  modbusGetByte(uint8_t i)            {return buforGetByte(  nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}
    uint8_t  modbusGetRegHi(uint8_t i)           {return buforGetByte(  nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}    //odczyt bajtu HI wedlug numeracji rejestrow modbus
    uint8_t  modbusGetRegLo(uint8_t i)           {return buforGetByte(  nNag+1+i-buforGetByte(AT_MODBUS_REGOFFSET));}  //odczyt bajtu LO wedlug numeracji rejestrow modbus
    uint16_t modbusGetReg(uint8_t i)             {return buforGetReg(   nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}       //odczyt rejestru..
    uint32_t modbusGetReg32(uint8_t i)           {return buforGetReg32( nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}   //odczyt reg32..
    uint8_t  modbusGetBit(uint8_t i,uint8_t j)   {return buforGetBit(   nNag+i-buforGetByte(AT_MODBUS_REGOFFSET),j);} //odczyt bitu ..ale w rejestrze 16bitowym
    uint8_t  modbusGetRegBit(uint8_t i,uint8_t j){return buforGetRegBit(nNag+i-buforGetByte(AT_MODBUS_REGOFFSET),j);} //odczyt bitu ..ale w rejestrze 16bitowym
    int      modbusGetInt(uint8_t i)           	 {return getInt(        nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}       //odczyt rejestru..
    int      modbusGetInt32(uint8_t i)         	 {return getInt32(      nNag+i-buforGetByte(AT_MODBUS_REGOFFSET));}   //odczyt reg32..

    void modbusSetDataLen(atDataBuffer *db,int l)			{ for(int i=0;i<3;i++)frame[i]=db->buforGetByte(i); for(int i=3;i<l;i++)frame[i]=0; dataLen=l;}
    void modbusSetRespNag(atDataBuffer *db)             	{ for(int i=0;i<3;i++)frame[i]=db->buforGetByte(i); for(int i=3;i<nNag;i++)frame[i]=0; dataLen=nNag;}  //pierwsze 3 bajty zostaja, zerujemy pozostale bajty naglowka
    void modbusSetByte( uint8_t i, uint8_t c)				{ setByte( 		nNag+i  ,c);}
    void modbusSetHi( uint8_t i, uint8_t c)  				{ setByte(		nNag+i  ,c);}
    void modbusSetLo( uint8_t i, uint8_t c)					{ setByte(		nNag+1+i,c);}
    void modbusSetReg( uint8_t i, uint16_t c)				{ setReg(		nNag+i  ,c);}
    void modbusSetReg32( uint8_t i, uint32_t c)				{ setReg32(		nNag+i  ,c);}
    void modbusSetInt32( uint8_t i, int32_t c)				{ setInt32(		nNag+i  ,c);}
    void modbusSetOnBit( uint8_t i,uint8_t b)				{ buforOnBit( 	nNag+i  ,b);}
    void modbusSetOffBit( uint8_t i,uint8_t b)			  	{ buforOffBit(	nNag+i  ,b);}
    void modbusSetBit( uint8_t i, uint8_t b,uint8_t v)    	{ buforSetBit(	nNag+i  ,b,v);}
    void modbusSetRegBit( uint8_t i, uint8_t b,uint8_t v) 	{ buforRegSetBit(	nNag+i  ,b,v);}
    void modbusSetCRC(uint8_t i)						  	{ buforSetCRC(nNag+i); dataLen=nNag+i+2;}

//    void buforAddCRC( );
    void buforSetCRC(){buforSetCRC(dataLen-2);}                 //zmiana (nadpisanie)sumy kontrolnej w buforze ramki. suma zapisywana jest w bajtach: dataLen-2 i dataLen-1
    void buforSetCRC(uint8_t l);                             //zmiana (nadpisanie)sumy kontrolnej w buforze ramki. suma zapisywana jest w bajtach: l, l+1
    void buforAddCRC(){buforSetCRC(dataLen);dataLen+=2;}           //dodanie sumy kontrolnej do bufora ramki (bajty dataLen i dataLen+1)
    uint16_t getCRC( unsigned len );
    uint16_t CRC(){return getCRC(dataLen-2);}
    bool checkCRC();

    string frameToHex();
    string charToHex(uint8_t* f,unsigned len);
    string charToHex(){ return charToHex(frame,dataLen); }
    string charToStr();
    string intToHex(unsigned i) {   stringstream s; s << std::hex << (unsigned int)i;  return s.str(); }
    string charToBit(uint8_t s);
    string charToBit(uint8_t* s,unsigned len);
    string charToBit(){ return charToBit(frame,dataLen); }

};

#endif
