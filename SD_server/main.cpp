/*
1. Rozdzielić funkcjonalnośc okan głównego na okienka dedykowane:
   a) Konfiguracja parametrów fizycznych drukarki
   b) Konfiguracja komunikacji z hostem
   c) Okienko do generowania plików z obliczeniami dynamicznymi
   d) okienko do ręcznego uruchamiania linii GCode
2. Zrobic moduł czytania komend GCode z pliku
   - czytamy
   - konwersja do KBAOR
   - wysyłka do kosta
   - odczyt potwierdzenia o hosta
   - interpretacja i obsługa błędów
3. przewidzieć, że może być wiele komunikatów zwrotnych z hosta w ramach jednej sesji (jednej linijki GCode)



*/

#include <atSocketServer.h>
#include <atDataBuffer.h>
#include <at_X3.h>
#include <atErrors.h>
#include <atFunctions.h>
#include <atI2C.h>
#include <atI2C_X3.h>
#include <atSPI_ads1256.h>
#include <atSPI.h>
#include <at_File.h>
#include <time.h>
#include <signal.h>
#include <at_Konfiguracja.h>
#include <atStoper.h>

//------------------------------------------------

#define I2C_PI_ID 0x56

atDataBuffer *dbrSS;
atDataBuffer *dbwSS;
atSocketServer* atSS;

atI2C* ati2c;
atI2C_X3* X3Motor;
atI2C_X3* X3Wave;

atSPI* spi;
atSPI_ADS1256 *ads1256_0;

at_File *FileRap;
at_Konfiguracja *cnf;


uint32_t devADS_0,devX3Wave,devX3Motor,devPI,devSD,devError; //liczniki ramek adresowanych do poszczególnych urządzeń
uint32_t errADS,errADS_0,errX3Wave,errX3Motor,errSD,errPI,errDev,errCRC,errX3;          //liczniki błędów poszczególnych urządzeń, na poziomie funkcji komunikacyjnych
uint64_t ErrorsADS_0,ErrorsX3Wave,ErrorsX3Motor;    //liczniki błędów urządzeń na poziomie procedur wykonawczych
uint64_t socketFrames,loopIndex;
int logLevel;
string logDev;
int logCH0,logCH1;
int ads_tot;
//--------------------------------------------------------------
void ResetErrors(){
    devADS_0=0;devX3Wave=0;devX3Motor=0;devSD=0;devPI=0;devError=0;
    errDev=0;errADS=0;errADS_0=0;errX3Wave=0;errX3Motor=0;errSD=0;errPI=0;errCRC=0,errX3=0;
    ErrorsADS_0=0;ErrorsX3Wave=0; ErrorsX3Motor=0;
    socketFrames=0;loopIndex=0;
}
//------------------------------------------------------------
void Rap(string s){
    time_t czas;
    struct tm* czas_info;
    time(&czas);
    czas_info=localtime(&czas);
    string s1=Time_to_string(czas_info)+":  "+s+"\n";
    FileRap->Append(s1);
    printf(s1.c_str());

}
//------------------------------------------------------------
void dispHelp(){
    printf("Użycie programu socket_server:\n");
    printf("1: socket_server\n");
    printf("2: socket_server kalibracja\n");
    printf("3: socket_server ads# ch_od ch_do log_level\n");
    printf("     ads# - który przetwornik [ADS0,ADS1,ADS2]\n");
    printf("     ch_od - kanał początkowy\n");
    printf("     ch_do - kanał końcowy\n");
    printf("     log_level - szczegółowośc raportowania [0-tylko błędy,..,10-max]\n\n");
}
//------------------------------------------------------------------
void dispPodsumowanie(){
  if(logLevel>=1){
    string s="Stop.\n";
    s+="Socket Frames: "                  +to_string(socketFrames)+"\n";
    s+="Socket CRC errors: "              +to_string(errCRC)+"\n";
    s+="Socket ID Device errors: "        +to_string(errDev)+"\n";
    s+="Socket PI Frames:        "        +to_string(errPI)+"/"+to_string(devPI)+"\n";
    s+="Socket SD:               "        +to_string(errSD)+"/"+to_string(devSD)+"\n";
    s+="Socket X3Motor:          "        +to_string(errX3Motor)+"/"+to_string(devX3Motor)+"\n";
    s+="Socket X3Wave:           "        +to_string(errX3Wave)+"/"+to_string(devX3Wave)+"\n";
    s+="Socket X3 Adress Device Error: "  +to_string(errX3)+"\n";
    s+="Socket ADS_0 waiting:    "        +to_string(errADS_0)+"/"+to_string(devADS_0)+"\n";
    s+="Socket ADS Address Device Error: "+to_string(errADS)+"\n";
    s+="\n";
    s+="Not ready ADS_0: "+to_string(ErrorsADS_0)+"\n";
    s+="Errors X3Motor: " +to_string(ErrorsX3Motor)+"\n";
    s+="Errors X3Wave: "  +to_string(ErrorsX3Wave)+"\n";
    s+="Loop Index: "     +to_string(loopIndex)+"\n";
    Rap(s);
  }
}
//--------------------------------
void koniec(){
    dispPodsumowanie();
    delete atSS;
    delete dbrSS;
    delete dbwSS;
    delete ati2c;
    delete X3Motor;
    delete X3Wave;
    delete spi;
    delete ads1256_0;
    delete FileRap;
    delete cnf;
    exit(0);
}
//--------------------------------
void  Handler(int signo){ koniec(); exit(0); }

//-------------------------------------------------------------------------------
void dispADS(atSPI_ADS1256 *ads){
    if(logLevel>=5 ){
        if(ads->getName()!=logDev) return;
        for(int i=logCH0; i<=logCH1;i++){
            Rap(
                ads->getName()+";  " +
                to_string(i)+"; ----   " +
                to_string(ads->getADC_N(i))+";  "+
                to_string(ads->getADC_valueSr(i))+";  U:"+
                to_string(ads->getADC_U_uV(i))+";  LU:"
             );
        }
    }
}
//-------------------------------------------------------------------------------
void dispX3( atI2C_dev * d ){
    if(d->getName()==logDev && logLevel>=5 ){
        Rap("X3: "+d->getDbw()->charToHex(  )+">> [<<"+d->getDbr()->charToHex(  )+"]");
    }
}

//--------------------------------------------------------------------------------
// return 1 oznacza błąd
int GoADS( atSPI_ADS1256 *ads ){
 if(ads->scan()){
     if(ads->getBuforyN()>0){
         ads->next();
         ads_tot++;
         dispADS(ads);
     }
     return 0;
 }
 return 1;
}


//--------------------------------------------------------------------------------
int GoX3( atI2C_X3 * d ){
    if(d->getName()==logDev && logDev=="MOTOR"  && !d->SendIsHot()){
        atDataBuffer db=atDataBuffer( I2C_PI_ID, AT_SOCKET_FRAME_LEN,AT_MODBUS_REQUNAG);
        db.buforSetNag(AT_MODBUS_ID_X3,AT_MODBUS_READ,AT_REGADRES_MOTOR1,0,0);
        db.buforAddCRC();
        d->setSendFrom(&db);
    }

    if(!d->go()){
        if(logLevel>=1)  Rap("X3 Error: "+d->getDbw()->charToHex(  )+">> [<<"+d->getDbr()->charToHex(  )+"]");
        return 1;
    }
    dispX3( d );
    return 0;
}

//--------------------------------------------------------------------------------
int serverGoADS1256_mV( atSPI_ADS1256 *ads ){
    dbwSS->buforSetNag(dbrSS);  //to jest ramka Resp a więc powinna mieć długośćnagłówka AT_MODBUS_RESPNAG
    for(int i=0; i<ads->getLen();i++){
         dbwSS->buforAddInt32(ads->getADC_U_uV(i));
    }
    dbwSS->buforAddCRC();
    return 0;
}

//--------------------------------------------------------------------------------
int serverGoPI(){
    dbwSS->buforSetNag(dbrSS);
    dbwSS->buforAddReg32(ErrorsADS_0);
    dbwSS->buforAddReg32(ErrorsX3Motor);
    dbwSS->buforAddReg32(ErrorsX3Wave);
    dbwSS->buforAddCRC();
    return 0;
}
//--------------------------------------------------------------------------------
int serverGoSD(){
    dbwSS->buforSetNag(dbrSS);
    dbwSS->buforAddReg(77);
    dbwSS->buforAddReg(1);
    dbwSS->buforAddReg(2);
    dbwSS->buforAddReg(3);
    dbwSS->buforAddReg(4);
    dbwSS->buforAddReg(5);
    dbwSS->buforAddReg(6);
    dbwSS->buforAddReg(7);
    dbwSS->buforAddReg(8);
    dbwSS->buforAddCRC();
    return 0;
}
//--------------------------------------------------------------------------------
//to jest funkcja obsługująca komunikację SOCKET, wołana jest ze środka jako ProcedData
//analizuje zawartość ramki dbr i ustawia wartośc dbw do wysłania
int serverGoFun( atDataBuffer *dbr, atDataBuffer *dbw ){

    socketFrames++;

    if( !dbr->checkCRC() ){ dbw->buforSetNag(AT_MODBUS_ID_PI,0x80,0,0, ATERROR_SOCKET_CRC ); dbw->buforAddCRC(); errCRC++; }
    else if(  dbr->checkId(AT_MODBUS_ID_PI) ) {
        if( dbr->checkDeviceAddress(AT_REGADRES_SD) )   {
            errSD+=serverGoSD();   devSD++;
        }
        else{
            errPI+=serverGoPI();   devPI++;
        }
    }
    else if(  dbr->checkId(AT_MODBUS_ID_X3)){
        if(      dbr->checkDeviceAddress(AT_REGADRES_MOTOR1) ) {
            devX3Motor++;
            X3Motor->setSendFrom(dbr);
            int err=!X3Motor->setReceInto(dbw); errX3Motor+=err;
        }
        else if( dbr->checkDeviceAddress(AT_REGADRES_WAVE) )   {
            devX3Wave++;
            X3Wave->setSendFrom(dbr);
            int err=!X3Wave->setReceInto(dbw); errX3Wave+=err;
        }
        else{dbw->setError( dbr, ATERROR_X3ADDRESSDEVICE ); errX3++;}
    }
    else if(  dbr->checkId(AT_MODBUS_ID_ADS1256) ){
        if(      dbr->checkDeviceAddress(0)){  errADS_0+=serverGoADS1256_mV(ads1256_0); devADS_0++;}
        else{dbw->setError( dbr, ATERROR_ADS1256ADDRESSDEVICE ); errADS++;}
    }
    else {  dbw->buforSetNag(AT_MODBUS_ID_PI,0x80,0,0, ATERROR_SOCKET_ID_DEVICE ); dbw->buforAddCRC();   errDev++; }

    if( dbr->getDataLen() !=0 ){
        if(logLevel>=4)Rap("Socket: ["+dbr->charToHex(  )+">>]  <<"+dbw->charToHex(  )+"\n");
        dbr->Clear();
     }

    return 0;

}
//-------------------------------------------------------------------------
void initSocket(){
    dbrSS = new atDataBuffer( AT_MODBUS_ID_PI, AT_SOCKET_FRAME_LEN ,AT_MODBUS_RESPNAG);
    dbwSS = new atDataBuffer( AT_MODBUS_ID_PI, AT_SOCKET_FRAME_LEN ,AT_MODBUS_REQUNAG);
    atSS  = new atSocketServer( dbrSS, dbwSS, &serverGoFun,AT_SOCKET_PORTNO );
}
//-------------------------------------------------------------------
int initX3(){
    X3Motor   = new atI2C_X3(I2C_PI_ID,"MOTOR",AT_MODBUS_ID_X3,AT_REGADRES_MOTOR1);
    X3Motor->SetFrameLen(AT_MOTOR_OUTBUFLEN);
    X3Wave     = new atI2C_X3(I2C_PI_ID,"WAVE",AT_MODBUS_ID_X3,AT_REGADRES_WAVE);
    X3Wave->SetFrameLen(AT_WAVE_OUTBUFLEN);
    ati2c  = new atI2C();
    ati2c->confRap(FileRap,logLevel);
    ati2c->setParams(cnf);
    ati2c->addDev( (atI2C_dev*)X3Motor );
    ati2c->addDev( (atI2C_dev*)X3Wave );
    ati2c->Init();
    if(ati2c->getError()!=0){ Rap( ati2c->getErrorOpis() );  return 1;  }
    return 0;
}
//-------------------------------------------------------------------
int initADS(){
    ads1256_0  = new atSPI_ADS1256( ADS1256_PINCE_15 ,"ADS0");
    spi = new atSPI(ADS1256_DEV00);
    spi->confRap(FileRap,logLevel);
    spi->addDev( (atSPI_dev*)ads1256_0 );
    ads1256_0->setParams(cnf);
    spi->Init();
    if(spi->getError()!=0){ Rap( spi->getErrorOpis() );  return 1;  }
    ads1256_0->OpisRejestrow();
    ads_tot=-1;
    return 0;
}
//-------------------------------------------------------------------------------------
void initVars(){
    FileRap= new at_File("/var/log/SD_server","SD_server",".log");
    Rap("SD_server, port:"+to_string(AT_SOCKET_PORTNO)+" Start.");
    cnf=new at_Konfiguracja("config/config");
    logLevel=AT_LOG_ERRORS;
    logCH0=0;logCH1=0;
    logDev="";
    ResetErrors();
}


//------------------------------------------------------------//
//                   M A I N                                  //
//------------------------------------------------------------//
int main(int argc, char *argv[]) {

    delay_ns(1000000000);
    signal(SIGINT, Handler);
    initVars();
    //--------------------------------------------------------
    if (argc>1 ){
        string arg=argv[1];
        if(arg=="ADS0" || arg=="MOTOR"|| arg=="Wave"|| arg=="debug"){  logLevel=5; logDev=arg;  }
        else{ dispHelp();  return 0;  }
    }
    if (argc>2 ){  logCH0=std::atoi(argv[2]); logCH1=logCH0;  logLevel=1;    }
    if (argc>3 ){  logCH1=std::atoi(argv[3]); logLevel=1;   }
    if (argc>4 ){  logLevel=std::atoi(argv[4]);    }
    Rap("logLevel:"+to_string(logLevel));
    //-------------------------------------------------------------------
    initSocket();
    if(initX3() && logDev=="") return 1;
    if(initADS() && logDev=="") return 1;
    //--------------------------------------------------------------------
    while(1)    {
        atSS->go();
        ErrorsX3Motor+=GoX3( X3Motor );
        ErrorsX3Wave +=GoX3( X3Wave );
        ErrorsADS_0  +=GoADS( ads1256_0 );
        loopIndex++;
    }
    koniec();
    return 0;
}
