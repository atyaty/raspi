#include "atDataBuffer.h"

//--------------------------------------------------------
void atDataBuffer::Ini( unsigned int fL, uint8_t n, uint8_t i){
	//now¹ tablice alokujemy gdy nie by³a zaalokowana lub gdy zmienia sie rozmiar
	if(frame!=NULL && frameLen!=fL) {delete [] frame; frame=NULL;}
	frameLen=fL;
	if(frame==NULL) frame=new char[frameLen] ;
	for(unsigned int i=0;i<fL;i++) frame[i]=0;
	dataLen=0;
	nNag=n;
	id=i;
}
//--------------------------------------------------------------------------
unsigned short int atDataBuffer::getCRC( int len ){
	unsigned short int crc = 0xFFFF;
	for (int pos = 0; pos < len; pos++) {
		crc ^= (unsigned short int)frame[pos];	                // XOR byte into least sig. byte of crc
		for (int i = 8; i != 0; i--) {							// Loop over each bit
			if ((crc & 0x0001) != 0) {							// If the LSB is set
				crc >>= 1;										// Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else												// Else LSB is not set
			crc >>= 1;											// Just shift right
		}
	}
	return crc;
}
//--------------------------------------------------------
bool atDataBuffer::checkCRC( int len ){
 unsigned short int crc=getCRC(dataLen-2);
 return frame[len]==((crc>>8)&0x00FF) && frame[len+1]==(crc&0x00FF) ;
}
//--------------------------------------------------------------------------
void atDataBuffer::buforSetCRC(uint8_t l){
	unsigned short int crc = getCRC(l);
	frame[l] = ((crc>>8)&0x00FF);
	frame[l+1] = (crc&0x00FF);
}

