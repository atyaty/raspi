#include "atSPI_dev.h"
#include <atSPI.h>

//-------------------------------
atSPI_dev::atSPI_dev(uint8_t p,string N){
    Setup(p,N);
    dbr=new atDataBuffer(AT_REGADRES_TERMO,AT_MODBUS_FRAME_LEN,AT_MODBUS_RESPNAG);
    dbw=new atDataBuffer(AT_REGADRES_TERMO,AT_MODBUS_FRAME_LEN,AT_MODBUS_REQUNAG);
    dbOwn=true;
}
//-------------------------------
atSPI_dev::atSPI_dev(uint8_t p,string N,atDataBuffer*dr,atDataBuffer*dw){ Setup(p,N); dbr=dr; dbw=dw;  }
//-------------------------------
void atSPI_dev::Setup(uint8_t p,string N){
    pinCE=p;
    Name=N;
    Error=0;
    dbOwn=false;
    dbr=NULL;
    dbw=NULL;
    spi_mode = SPI_MODE_1;
    spi_bitsPerWord = 8;
    spi_speed = 1000000;		//1000000 = 1MHz (1uS per bit)
 }
//---------------------------------------
void atSPI_dev::SetupDev( atSPI* d ){    dev=d; }
//-------------------------------------------------
atSPI_dev::~atSPI_dev(){ if(dbOwn){delete dbw; delete dbr;} }
//---------------------------------------
bool atSPI_dev::Init( ){
    bcm2835_gpio_fsel(pinCE, BCM2835_GPIO_FSEL_OUTP);
    delay_ns(1000000);
    return true;
}
//---------------------------------------------------
void atSPI_dev::setON ( ){
    bcm2835_gpio_write(pinCE,LOW);
}
//---------------------------------------------------
void atSPI_dev::setOFF ( ){
    bcm2835_gpio_write(pinCE,HIGH);
}
//---------------------------------------------------
void atSPI_dev::go ( ){
    if(Error) return;
    dbr->Clear();
    for (unsigned i = 0 ; i < dbw->getDataLen() ; i++){
        uint8_t ret=bcm2835_spi_transfer(dbw->buforGetByte(i));
        dbr->setByte(i,ret);
    }
    dbr->setDataLen(dbw->getDataLen());
    return ;
}
