#include "atI2C_dev.h"
#include "atI2C.h"

//-------------------------------
atI2C_dev::atI2C_dev( string n){    Name=n; FileRap=NULL;}
//-------------------------------
atI2C_dev::atI2C_dev( string n,atDataBuffer*dr,atDataBuffer*dw){    Name=n;  dbr=dr; dbw=dw; FileRap=NULL;}
//---------------------------------------
void atI2C_dev::SetupDev( atI2C* d ){  dev=d;  }
//---------------------------------------------------
bool atI2C_dev::go(){
   bcm2835_i2c_setSlaveAddress( dev->getId() );
   bcm2835_i2c_set_baudrate(500000); //100000 ? tu sprawdzić

   char buf[AT_MODBUS_FRAME_LEN];
   for(int i=0;i<AT_MODBUS_FRAME_LEN;i++) buf[i]=0xff;
   for(unsigned i=0;i<dbw->getDataLen();i++) buf[i]=dbw->buforGetByte(i);
   uint8_t ret=bcm2835_i2c_write(buf,dbw->getDataLen());
   if(ret){ SetError(ATERROR_I2C_DEV_GOWRITE,"bcm error: "+to_string(ret)); return false;}
   delay_ns(1000000);  //zrobić analizę tych parametrów: delay i baudrate
                        // przy ustawieniach baudrate 500000 i delay 1000000 uzyskuję czysty przebieg bez błedów
                        // b=500000 i d=500000 już daje błedy
                        // nie wiem czy to ma znaczenie ale rezystory podciągające są teraz 47k||10k, też sprawdzić jak się zachowuje przy zmianie
   dbr->Clear();
   ret= bcm2835_i2c_read(buf,dbr->getFrameLen());
   if(ret){ SetError(ATERROR_I2C_DEV_GOREAD,"bcm error: "+to_string(ret)); return false;}
   for(int i=0;i<AT_MODBUS_FRAME_LEN;i++){ dbr->setByte(i,buf[i]); }
   dbr->setDataLen(AT_MODBUS_FRAME_LEN);
   return true;
}
//----------------------------------------------------------------------------
void atI2C_dev::SetError(int c,string e){
    Error.SetError(c,e);
    Rap(Name+", Error: 0x"+IntToHex(c)+" ("+e+") dbw: "+dbw->charToHex()+" dbr: "+dbr->charToHex()+"\n",AT_LOG_ERRORS);
}
//----------------------------------------------------------------------------
void atI2C_dev::SetError(int c){
    Error.SetError(c);
    Rap(Name+", Error: 0x"+IntToHex(c)+" dbw: "+dbw->charToHex()+" dbr: "+dbr->charToHex()+"\n",AT_LOG_ERRORS);
}



