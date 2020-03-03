#include "atI2C_X3.h"
#include "atI2C.h"

//------------------------------------------
atI2C_X3::atI2C_X3(string n,uint8_t d,uint8_t a):atI2C_dev(n){
    addrDev=a;
    idModbusDev=d;
    dbr=new atDataBuffer(d,AT_MODBUS_FRAME_LEN,AT_MODBUS_RESPNAG);
    dbw=new atDataBuffer(d,AT_MODBUS_FRAME_LEN,AT_MODBUS_REQUNAG);
}
//------------------------------------------
bool atI2C_X3::Init(){
    dbw->Clear();
    dbw->buforSetNag(idModbusDev,AT_MODBUS_READ,addrDev,0,0);
    dbw->buforAddCRC();
    sendIsHot=true;
    go();
    return Error.IsEmpty();
}
//------------------------------------------
bool atI2C_X3::checkFrame(atDataBuffer* d ){
    return d->checkDeviceAddress(addrDev) && d->checkId(idModbusDev) && d->checkCRC();
}
//------------------------------------------
bool atI2C_X3::setSendFrom(atDataBuffer* d){
    if(checkFrame(d)){
        dbw->setFrame(d);
        sendIsHot=true;
        return true;
    }
    return false;
}
//------------------------------------------
bool atI2C_X3::setReceInto(atDataBuffer* d){
    if(receIsHot){
       if( checkReceFrame() ) {  d->setFrame(dbr); }
       else {  d->buforSetNag(idModbusDev,0x80,addrDev,0, ATERROR_X3MODBUS_CRC ); d->buforAddCRC(); return false;}
       receIsHot=false;
    }
    else{
        d->setError( ATERROR_X3WAITING);
    }
    return true;
}
//------------------------------------------
bool atI2C_X3::go(){
    if(sendIsHot){
        atI2C_dev::go();
        sendIsHot=false;
        receIsHot=true;
        if(dbr->getDataLen()>0){ //nie umiem odczytać długości danych transmitowanych (odbieranych) przez I2C. Funkcja Read zawsze zwraca pełny bufor. Dlatego konieczne jest przycięcie długości ramki. Wywalic jak opanue się odczyt długości.
            if(dbr->checkErrorFrame() )                dbr->setDataLen(AT_X3ERROR_OUTBUFLEN); //ramki błędów też obcinamy do długości z góry ustalonej
            else if(dbr->checkDeviceAddress(addrDev) ) dbr->setDataLen(frameModbusLen);
            else { SetError(ATERROR_X3UNRECOGNIZED); return false;     }
            if(!dbr->checkCRC()) {
                SetError(ATERROR_X3MODBUS_CRC);
                return false;
            }
        }
    }
    return true;
}

