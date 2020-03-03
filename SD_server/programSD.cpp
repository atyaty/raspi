#include "programSD.h"

//-------------------------------------------------
programSD::programSD(){
    status=SD_STATUS_STOP;
    pomiar=SD_POMIAR_STOP;
    stoper=new atStoper( SD_SAMPLE_TIME);
    Error=0;
    //------------
    X3WaveDataLen=4;
}
//-------------------------------------------------
programSD::~programSD(){
    delete stoper;
}

//-------------------------------------------------
bool programSD::go(){
    goSS();
    if(status==SD_STATUS_GOUP)        goUp();
    else if(status==SD_STATUS_GODN)   goDn();
    else if(status==SD_STATUS_POMIAR) goPomiar();
}
//-------------------------------------------------
bool programSD::goSS(){
    return true;
}
//-------------------------------------------------
bool programSD::goUp(){
    return true;
}
//-------------------------------------------------
bool programSD::goDn(){
    return true;
}
//-------------------------------------------------
bool programSD::goPomiar(){
    if(pomiar==SD_POMIAR_START)        pomiarStart();
    else if(pomiar==SD_POMIAR_ZWLOKA)  pomiarZwloka();
    else if(pomiar==SD_POMIAR_GETDATA) pomiarGetData();
    else if(pomiar==SD_POMIAR_WYNIK)   pomiarWynik();
    return true;
}
//-------------------------------------------------
bool programSD::pomiarStart(){
    atDataBuffer* dbr=X3Wave->getDbr();
    atDataBuffer* dbw=X3Wave->getDbw();
    dbw->Clear();
    dbw->buforSetNag(AT_MODBUS_SET,X3Wave->getAddrDev(),0,X3WaveDataLen);
    dbw->buforAddReg(S);
    dbw->buforAddReg(f);
    dbw->buforAddCRC();
    if( X3Wave->go() ){
        pomiar=SD_POMIAR_ZWLOKA;
        stoper->start_ms(SD_SAMPLE_TIME);
    }
    else{
        pomiar=SD_POMIAR_STOP;
        Error=ATERROR_SD_SETWAVE;
    }
    return true;
}
//-------------------------------------------------
bool programSD::pomiarZwloka(){
    if(!stoper->waiting()){
        pomiar=SD_POMIAR_GETDATA;
        stoper->start_ms(SD_SAMPLE_TIME);
        nWrite=0;
        nSend=0;
    }
    return true;
}
//-------------------------------------------------
bool programSD::pomiarGetData(){
    if(ads->scan()){
            uint16_t N=ads->getBuforyNSum(); //ilość próbek w buforze
            if(N>0 && stoper->time_ms()>(SD_SAMPLE_TIME*nWrite/SD_BUFOR_LEN)){
                int32_t S=ads->getBuforyU_uVSum(); //suma danych w buforze
                input[nWrite++]=S/N;
                ads->next();
                if(nWrite>SD_BUFOR_LEN){
                    pomiar=SD_POMIAR_WYNIK;
                    pomiar_time=stoper->time_ms();
                }
            }
    }
    else{
        pomiar=SD_POMIAR_STOP;
        Error=ATERROR_SD_GETDATA;
    }
    return true;
}
//-------------------------------------------------
bool programSD::pomiarWynik(){

    return true;
}
