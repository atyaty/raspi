#include "atDataBuffer.h"

//--------------------------------------------------------
void atDataBuffer::Ini( uint16_t fL){
	if(frame!=NULL && frameLen!=fL) {delete [] frame; frame=NULL;}
	frameLen=fL;
    if(frame==NULL) frame=new uint8_t[frameLen] ;
    for(uint16_t i=0;i<fL;i++) frame[i]=0;
	dataLen=0;
}
//--------------------------------------------------------------------------
uint16_t atDataBuffer::getCRC( unsigned len ){
    uint16_t crc = 0xFFFF;
    for (unsigned pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)frame[pos];
        for (int i = 8; i != 0; i--) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
				crc ^= 0xA001;
			}
            else
            crc >>= 1;
		}
	}
	return crc;
}
//--------------------------------------------------------
bool atDataBuffer::checkCRC( ){
 if (dataLen<2) { return false; }
 uint16_t crc=getCRC(dataLen-2);
 return (frame[dataLen-2]==(crc >> 8)) && (frame[dataLen-1]==(crc & 0x00FF));
}
//--------------------------------------------------------
//void atDataBuffer::buforAddCRC( ){
// uint16_t crc=CRC(dataLen);
// frame[dataLen]=(crc >> 8);
// frame[dataLen+1]=(crc & 0x00FF);
// dataLen+=2;
//}
//----------------------------------------------------------
void atDataBuffer::setFrame(uint8_t* f,uint16_t fL){
    for(uint16_t i=0;i<fL;i++){  frame[i]=f[i];  }
    dataLen=fL;
}
//---------------------------------------------------------
string atDataBuffer::frameToHex(){
    if(dataLen<=nNag){                      return charToHex(frame,dataLen)+" L="+to_string(dataLen); }
    string s=charToHex(frame,nNag)+"- ";
    if(!checkCRC()||((dataLen-2)<nNag)){  return s+charToHex(frame+nNag,dataLen-nNag)+" L="+to_string(dataLen);    }
    return s+charToHex(frame+nNag,dataLen-nNag-2)+"("+charToHex(frame+(dataLen-2),2)+") L="+to_string(dataLen);
}
//---------------------------------------------------------
string atDataBuffer::charToHex(uint8_t* f,unsigned len){
    stringstream ss;
    for(unsigned i=0;i<len;i++){
     ss << std::hex << (unsigned int) f[i];
     ss << " ";
    }
    return ss.str();

}
//---------------------------------------------------------
string atDataBuffer::charToStr(){
    string s;
    for(unsigned i=0;i<dataLen;i++){ s+=frame[i];}
    return s;

}
//--------------------------------------------------------
string atDataBuffer::charToBit(uint8_t * s,unsigned len){
    stringstream ss;
    for(unsigned i=0;i<len;i++){
        ss << charToBit(s[i])<< " ";
    }
    return (string) ss.str();
}
//--------------------------------------------------------
string atDataBuffer::charToBit(uint8_t s){
    stringstream ss;
    for(int j=0;j<8;j++){
            ss <<  ((s & (1 << (7-j))) >> (7-j));
    }
    return (string) ss.str();
}

//-----------------------------------------------
//zgodne z mcp3424
int32_t atDataBuffer::getInt18(int i){
    uint32_t d0= ((uint32_t)frame[i])<<16;
    d0 |= ((uint16_t) frame[i+1])<<8;
    d0 |=  frame[i+2];
    if(frame[i]&0b00000010) d0 |= ((uint32_t)0b11111111)<<24;
    return  d0;
}
//-----------------------------------------------
//zgodne z ads1256
int32_t atDataBuffer::getInt24(int i){
    uint32_t d0= ((uint32_t)frame[i])<<16;
    d0 |= ((uint16_t) frame[i+1])<<8;
    d0 |=  frame[i+2];
    if(frame[i]&0b10000000) d0 |= ((uint32_t)0b11111111)<<24;
    return  (int32_t)d0;
}
//--------------------------------------------------------------------------
void atDataBuffer::buforSetCRC(uint8_t l){
    unsigned short int crc = getCRC(l);
    frame[l] = ((crc>>8)&0x00FF);
    frame[l+1] = (crc&0x00FF);
}
