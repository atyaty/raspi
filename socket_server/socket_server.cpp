#include "socket_server.h"

//-------------------------------------------------
programSD::programSD(at_Konfiguracja *c){
    cnf=c;
    stat=SD_STATUS_STOP;
    stat_pomiar=SD_POMIAR_STOP;
    stat_up=SD_UP_STOP;
    stat_dn=SD_DN_STOP;
    stat_zatrzymaj=SD_ZATRZYMAJ_STOP;
    stat_zasilanie=SD_ZASILANIE_STOP;
    X3WaveDataLen=AT_WAVE_INBUFLEN-AT_MODBUS_REQUNAG-2;
    X3MotorDataLen=AT_MOTOR_INBUFLEN-AT_MODBUS_REQUNAG-2;//2-crc
    Error=0;
    X3Motor_freq=10000; //wczytać z cnf
    nWrite=0;
    nSend=0;
    zasilanie.stop();

    debug=0;
}

//-------------------------------------------------
int programSD::go(){
    Rap("..go: "+to_string(debug++) , AT_LOG_DEBUG);
    Zasilanie(); //to wkładamy do głównej pętli dlatego że niedorobiona jest klasa atStoper.
                 //objekt zasilanie steruje włączeniem i wyłączeniem zasilaczy i niniejszą akcję powinny wykonywać jedo zdarzenia onUp i onDn
                 //wtedy on sam powinien być jakoś umieszczany w pętli głównej .. może tak samo jak teraz SocketSerwer na poziomie main.
    if(     stat==SD_STATUS_GOUP)      goUp();
    else if(stat==SD_STATUS_GODN)      goDn();
    //else if(stat==SD_STATUS_POMIAR)    goPomiar();
    //else if(stat==SD_STATUS_ZATRZYMAJ) goZatrzymaj();
    else if(stat==SD_STATUS_STOP)      return Error; //jeśli Error!=0 to jet tu wyjście
    else if(stat==SD_STATUS_OFF )      return -1;    //wyjście
    return 0;
}
//-------------------------------------------------
//tylko procedury główne mogą ustawiać SD_STATUS_STOP
bool programSD::goUp(){
    Rap("..goUp ("+to_string(stat_up)+").",AT_LOG_DEBUG);
    if(stat_up==SD_UP_START)        upStart();
    else if(stat_up==SD_UP_WAIT)    upWaiting();
    else if(stat_up==SD_UP_STOP)   {Rap("stop Up",AT_LOG_CLASSMAIN); stat=SD_STATUS_STOP;}
    return true;
}
//-------------------------------------------------
bool programSD::goDn(){
    Rap("..goDn.", AT_LOG_DEBUG);
    if(stat_dn==SD_DN_START)        dnStart();
    else if(stat_dn==SD_DN_WAIT)    dnWaiting();
    else if(stat_dn==SD_DN_STOP)    {Rap("stop Dn",AT_LOG_CLASSMAIN); stat=SD_STATUS_STOP;}
    return true;
}
//-------------------------------------------------
bool programSD::goZatrzymaj(){
    if(stat_zatrzymaj==SD_ZATRZYMAJ_START)         zatrzymajStart();
    else if(stat_zatrzymaj==SD_ZATRZYMAJ_WAIT)     zatrzymajWaiting();
    else if(stat_zatrzymaj==SD_ZATRZYMAJ_STOP)     {Rap("stop Zatrzymaj",AT_LOG_CLASSMAIN); stat=SD_STATUS_STOP;}
     return true;
}

//-------------------------------------------------

bool programSD::upStart(){
    Rap("..upStart ("+to_string(stat_up)+")",AT_LOG_DEBUG);
    atDataBuffer* dbr=X3Motor->getDbr();
    atDataBuffer* dbw=X3Motor->getDbw();
    dbw->Clear();
    dbw->buforSetNag(AT_MODBUS_SET,X3Motor->getAddrDev(),0,X3MotorDataLen);
    dbw->buforAddReg(0);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_GO,1);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_DIR,0);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_EN,1);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_ONOFF,1);
    dbw->buforAddReg32(0);
    dbw->buforAddReg32(X3Motor_freq);
    dbw->buforAddCRC();
    X3Motor->setSendIsHot();
    Rap("..upStart "+ dbw->charToHex()+">>",AT_LOG_DEBUG);
    if( X3Motor->go() ){
        stat_up=SD_UP_WAIT;
        Rap("..upStart ["+ dbr->charToHex()+">>]",AT_LOG_DEBUG);
    }
    else{
        stat_up=SD_UP_STOP;
        Error=ATERROR_SD_X3_START_UP;
    }
    return true;
}
//-------------------------------------------------
bool programSD::upWaiting(){
    Rap("..upWaiting ("+to_string(stat_up)+")",AT_LOG_DEBUG);
    atDataBuffer* dbr=X3Motor->getDbr();
    atDataBuffer* dbw=X3Motor->getDbw();
    dbw->Clear();
    dbw->buforSetNag(AT_MODBUS_READ,X3Motor->getAddrDev(),0,X3MotorDataLen);
    dbw->buforAddCRC();
    X3Motor->setSendIsHot();
    if( X3Motor->go() ){
        upStop=dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_UPSTOP);
        dnStop=dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_DNSTOP);
        motor_pos=dbr->modbusGetInt32(AT_MOTOR_AO_POS);
        if(upStop){
            //Rap("upWaiting..OK\n");
            stat_up=SD_UP_STOP;
        }
    }
    else{
        stat_up=SD_UP_STOP;
        Error=ATERROR_SD_X3_WAIT_UP;
    }
    return true;
}
//-------------------------------------------------
bool programSD::dnStart(){
    Rap("dnStart",AT_LOG_CLASSMAIN);
    atDataBuffer* dbw=X3Motor->getDbw();
    dbw->Clear();
    dbw->buforSetNag(AT_MODBUS_SET,X3Motor->getAddrDev(),0,X3MotorDataLen);
    dbw->buforAddReg(0);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_GO,1);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_DIR,1);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_EN,1);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_ONOFF,1);
    dbw->buforAddReg32(0);
    dbw->buforAddReg32(X3Motor_freq);
    dbw->buforAddCRC();
    X3Motor->setSendIsHot();
    if( X3Motor->go() ){
        stat_dn=SD_UP_WAIT;
    }
    else{
        stat_dn=SD_DN_STOP;
        Error=ATERROR_SD_X3_START_DN;
    }
    return true;
}
//-------------------------------------------------
bool programSD::dnWaiting(){
    atDataBuffer* dbr=X3Motor->getDbr();
    atDataBuffer* dbw=X3Motor->getDbw();
    dbw->Clear();
    dbw->buforSetNag(AT_MODBUS_READ,X3Motor->getAddrDev(),0,X3MotorDataLen);
    dbw->buforAddCRC();
    X3Motor->setSendIsHot();
    if( X3Motor->go() ){
        upStop=dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_UPSTOP);
        dnStop=dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_DNSTOP);
        motor_pos=dbr->modbusGetInt32(AT_MOTOR_AO_POS);
        if(dnStop){
            stat_dn=SD_DN_STOP;
        }
    }
    else{
        stat_dn=SD_DN_STOP;
        Error=ATERROR_SD_X3_WAIT_DN;
    }
    return true;
}
//-------------------------------------------------
bool programSD::zatrzymajStart(){
    atDataBuffer* dbw=X3Motor->getDbw();
    dbw->Clear();
    dbw->buforSetNag(AT_MODBUS_SET,X3Motor->getAddrDev(),0,X3MotorDataLen);
    dbw->buforAddReg(0);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_GO,0);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_DIR,0);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_EN,0);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_ONOFF,1);
    dbw->buforAddReg32(0);
    dbw->buforAddReg32(X3Motor_freq);
    dbw->buforAddCRC();
    X3Motor->setSendIsHot();
    if( X3Motor->go() ){
        stat_zatrzymaj=SD_ZATRZYMAJ_WAIT;
    }
    else{
        Error=ATERROR_SD_X3_START_ZATRZYMAJ;
        stat_zatrzymaj=SD_ZATRZYMAJ_STOP;
    }
    return true;
}
//-------------------------------------------------
bool programSD::zatrzymajWaiting(){
    atDataBuffer* dbr=X3Motor->getDbr();
    atDataBuffer* dbw=X3Motor->getDbw();
    dbw->Clear();
    dbw->buforSetNag(AT_MODBUS_READ,X3Motor->getAddrDev(),0,X3MotorDataLen);
    dbw->buforAddCRC();
    X3Motor->setSendIsHot();
    if( X3Motor->go() ){
        upStop=dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_UPSTOP); //na wszelki wypadek
        dnStop=dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_DNSTOP); //jw
        motor_pos=dbr->modbusGetInt32(AT_MOTOR_AO_POS);
        uint8_t busy=dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_GO);
        if(busy){
            stat_zatrzymaj=SD_ZATRZYMAJ_STOP;
        }
    }
    else{
        stat_zatrzymaj=SD_ZATRZYMAJ_STOP;
        Error=ATERROR_SD_X3_WAIT_ZATRZYMAJ;
    }
    return true;
}

//-------------------------------------------------
bool programSD::Zasilanie(){

    if( zasilanie.waiting() && stat_zasilanie==SD_ZASILANIE_GOING) return true;
    if( !zasilanie.waiting() && stat_zasilanie!=SD_ZASILANIE_GOING) return true;
    Rap("Zasilanie, stat_zasilanie="+to_string(stat_zasilanie)+" ..zasilanie="+to_string(zasilanie.waiting())+" \n" , AT_LOG_DEBUG);

    atDataBuffer* dbw=X3Motor->getDbw();
    dbw->Clear();
    dbw->buforSetNag(AT_MODBUS_SET,X3Motor->getAddrDev(),0,X3MotorDataLen);
    dbw->buforAddReg(0);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_ONOFF,zasilanie.waiting());
    dbw->buforAddReg32(0);
    dbw->buforAddReg32(X3Motor_freq);
    dbw->buforAddCRC();
    X3Motor->setSendIsHot();
    if( X3Motor->go() ){
        stat_zasilanie=!stat_zasilanie;
        stat=SD_STATUS_STOP;
        Rap("Zasilanie, stat_zasilanie="+to_string(stat_zasilanie)+" \n" , AT_LOG_DEBUG);
    }
    else{
        Error=ATERROR_SD_X3_ZASILANIE;
    }
    return true;
}


//-------------------------------------------------
int programSD::ini(){
    atDataBuffer* dbr=X3Motor->getDbr();
    atDataBuffer* dbw=X3Motor->getDbw();
    dbw->Clear();
    dbw->buforSetNag(AT_MODBUS_READ,X3Motor->getAddrDev(),0,X3MotorDataLen);
    dbw->buforAddCRC();
    X3Motor->setSendIsHot();
    if( X3Motor->go() ){
        upStop=dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_UPSTOP);
        dnStop=dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_DNSTOP);
        motor_pos=dbr->modbusGetInt32(AT_MOTOR_AO_POS);
    }
    else{
        Error=ATERROR_SD_X3_INI;
        return Error;
    }
    return 0;
}

//----------------------------Start procedur może odbywać się z zewnątrz
void programSD::start_pomiar()    {
    stat=SD_STATUS_POMIAR;
    stat_pomiar=SD_POMIAR_START;
    //Rap("start_pomiar\n");
}
void programSD::start_up()        {
    stat=SD_STATUS_GOUP;
    stat_up=SD_UP_START;
    //Rap("start_up\n");

}
void programSD::start_dn()        {
    stat=SD_STATUS_GODN;
    stat_dn=SD_DN_START;
    //Rap("start_dn\n");
}
void programSD::start_zatrzymaj() {
    stat=SD_STATUS_ZATRZYMAJ;
    stat_zatrzymaj=SD_ZATRZYMAJ_START;
    //Rap("start_zatrzymaj\n");
}

//---------------------------------------------------------------------------------------------
void programSD::DataProcess(atDataBuffer *mbRece,atDataBuffer *mbSend){
    zasilanie.start_ms(5000);	//przesuwamy moment wyłączenia, trwająca komunikacja z programem oznacza że zasilacze mają być włączone
    if( mbRece->checkFunction( AT_MODBUS_SET ) || mbRece->checkFunction( AT_MODBUS_WRITE ) ){setFromModbus(mbRece);}	putToModbus(mbRece,mbSend); }

//--------------------------------------------------------------------------------
void programSD::setFromModbus(atDataBuffer *mbRece ){
    bool up=mbRece->modbusGetRegBit(AT_SD_DI,AT_SD_DI_UP);
    bool dn=mbRece->modbusGetRegBit(AT_SD_DI,AT_SD_DI_DN);
    bool po=mbRece->modbusGetRegBit(AT_SD_DI,AT_SD_DI_POMIAR);
    bool za=(!up && !dn);
    if(up && !upStop && (stat!=SD_STATUS_GOUP || stat_up==SD_UP_STOP)){
        start_up(); Rap("start_up "+to_string(stat_up),AT_LOG_CLASSMAIN);
    }
    else if(po && (stat!=SD_STATUS_POMIAR || stat_pomiar==SD_POMIAR_STOP)){
        setFreq(mbRece->modbusGetReg(AT_SD_AI_FMIN),mbRece->modbusGetReg(AT_SD_AI_FMAX),mbRece->modbusGetReg(AT_SD_AI_FD)   );
        setAmpl(mbRece->modbusGetReg(AT_SD_AI_SMIN),mbRece->modbusGetReg(AT_SD_AI_SMAX),mbRece->modbusGetReg(AT_SD_AI_SD)   );
        Rap("setFromModbus: Freq: "+to_string(Freq_min)+" - "+to_string(Freq_max)+" /"+to_string(Freq_d) , AT_LOG_WORK);
        Rap("setFromModbus: Ampl: "+to_string(Ampl_min)+" - "+to_string(Ampl_max)+" /"+to_string(Ampl_d) , AT_LOG_WORK);
        start_pomiar();Rap("start_pomiar "+to_string(stat_pomiar) , AT_LOG_DEBUG);
    }
    else if(dn && !dnStop && (stat!=SD_STATUS_GODN || stat_dn==SD_DN_STOP)){
        start_dn(); Rap("start_dn "+to_string(stat_dn) , AT_LOG_DEBUG);
    }
    else if(za && (stat==SD_STATUS_GOUP || stat==SD_STATUS_GODN )){
        start_zatrzymaj(); Rap("start_zatrzymaj "+to_string(stat_zatrzymaj) , AT_LOG_DEBUG);
    }
    else if(!po && (stat==SD_STATUS_POMIAR)){
        stat=SD_STATUS_STOP; Rap("zatrzymaj pomiar-----" , AT_LOG_WORK);
    }
    //Rap("Set from modbus- stat: "+to_string(stat)+", stat_up: "+to_string(stat_up)+", stat_zatrzymaj: "+to_string(stat_zatrzymaj)+" \n");
}
//--------------------------------------------------------------------------------
void programSD::putToModbus(atDataBuffer *mbRece, atDataBuffer *mbSend ){
    mbSend->modbusSetRespNag(mbRece);
    mbSend->modbusSetReg( AT_SD_DO ,0);
    mbSend->modbusSetRegBit( AT_SD_DO ,AT_SD_DO_POMIAR,(stat==SD_STATUS_POMIAR&&stat_pomiar<SD_POMIAR_FINISHED));
    mbSend->modbusSetRegBit( AT_SD_DO ,AT_SD_DO_UP,stat==SD_STATUS_GOUP);
    mbSend->modbusSetRegBit( AT_SD_DO ,AT_SD_DO_DN,stat==SD_STATUS_GODN);
    mbSend->modbusSetRegBit( AT_SD_DO ,AT_SD_DO_UPSTOP,upStop);
    mbSend->modbusSetRegBit( AT_SD_DO ,AT_SD_DO_DNSTOP,dnStop);
    mbSend->modbusSetReg( AT_SD_AO_N,nSend);

    int nSLen=0;
    if((nSend+8)<=nWrite){  nSLen=8; }
    else if(nWrite>=SD_BUFOR_LEN && nSend+8>=SD_BUFOR_LEN  && nSend<SD_BUFOR_LEN){ nSLen=SD_BUFOR_LEN-nSend;} //ostatnie rejestry - nie pełna ramka

    //string debug_s="putToModbus "+to_string(Ampl)+"/"+to_string(Freq);
    //debug_s+=" Time: "+to_string(pomiar_time)+" ("+to_string(nSend)+") ";
    for(int i=0;i<nSLen;i++){
        mbSend->modbusSetInt32( AT_SD_AO_0+(4*i),input[nSend+i]);
     //   debug_s+=to_string(input[nSend+i])+"  ";
    }
    //if((nSend+8)<=nWrite)
   //     Rap(debug_s);

    nSend+=nSLen;

    mbSend->modbusSetReg( AT_SD_AO_AMPL,Ampl);
    mbSend->modbusSetReg( AT_SD_AO_FREQ,Freq);
    mbSend->modbusSetReg( AT_SD_AO_TIME,pomiar_time);
    mbSend->modbusSetInt32( AT_SD_AO_POS,motor_pos);
    mbSend->modbusSetCRC( AT_SD_FRAMEOUT_CRC);

}


//-------------------------------------------------------------------------------
void programSD::setCnf() {
}

//------------------------------------------------------------
// LevelRap - aktualny level raportowania (z takim levelem uruchomiono program)
// L - wymagany level raportowania (taki musi być level żeby nastąpił zapis do raportu)
void programSD::Rap(string s, int L){
    if(LevelRap<L) return;             //za mały level więc raportowania nie ma
    time_t czas;
    struct tm* czas_info;
    time(&czas);
    czas_info=localtime(&czas);
    string s1=Time_to_string(czas_info)+":  "+s+"\n";
    FileRap->Append(s1);
    printf(s1.c_str());

}

