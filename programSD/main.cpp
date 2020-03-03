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
#include <at_X3Error.h>
#include <atI2C.h>
#include <atI2C_X3.h>
#include <atSPI_ads1256.h>
#include <atSPI.h>
#include <at_File.h>
#include <atFunctions.h>
#include <time.h>
#include <signal.h>     //signal()
#include <at_Konfiguracja.h>
#include <atStoper.h>
#include <programSD.h>

//------------------------------------------------

#define I2C_PI_ID 0x56

atDataBuffer *dbrSS;
atDataBuffer *dbwSS;
atSocketServer* atSS;

atSPI* spi;
atSPI_ADS1256 *ads1256_0;

atI2C* ati2c;
atI2C_X3* X3Motor;
atI2C_X3* X3Wave;

at_File *FileRap;
at_Konfiguracja *cnf;
programSD* kal;
uint64_t loopIndex;
int logLevel;
unsigned long errCRC=0, errPIRA=0,devSD=0,errID=0;



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
//------------------------------------------------------------------
void koniec(){
  Rap("Loop Index: "+to_string(loopIndex));
  cout <<"Koniec programu SD.\n";
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
  delete kal;
}
//-------------------------------------------------------------------
void  Handler(int signo){ koniec();  exit(0); }
//-------------------------------------------------------------------
int serverGoFun( atDataBuffer *dbr, atDataBuffer *dbw ){
    if( !dbr->checkCRC() ){ dbw->buforSetNag(AT_MODBUS_ID_PI,0x80,0,0, ATERROR_SOCKET_CRC ); dbw->buforAddCRC(); errCRC++;  }
    else if(  dbr->checkId(AT_MODBUS_ID_PI) ) {
        if( dbr->checkDeviceAddress(AT_REGADRES_SD) ) {  kal->DataProcess(dbr,dbw);                              devSD++;   }
        else{   dbw->buforSetNag(dbr); dbw->buforAddReg(0x77); dbw->buforAddCRC();                               errPIRA++; }
    }
    else {  dbw->buforSetNag(AT_MODBUS_ID_PI,0x80,0,0, ATERROR_SOCKET_ID_DEVICE ); dbw->buforAddCRC();           errID++;   }
    if( dbr->getDataLen() !=0 ){ //to tylko zapis niepustych ramek do pliku
        if(logLevel>=4)Rap("Socket: ["+dbr->charToHex(  )+">>]  <<"+dbw->charToHex(  )+"\n");
        dbr->Clear();
    }
    return 0;

}
//-------------------------------------------------------------------
int initADS(){
    ads1256_0  = new atSPI_ADS1256( ADS1256_PINCE_15 ,"ADS0");
    spi = new atSPI("SPI");
    spi->confRap(FileRap,logLevel); //jako pierwsze, żeby kontrolować błądy podczas konfiguracji CNF
    spi->setParams(cnf);
    spi->addDev( (atSPI_dev*)ads1256_0 );
    ads1256_0->setParams(cnf);  //dopiero tu ma dostęp do dev->RapFile
    spi->Init();
    if(spi->getError()!=0){ Rap( spi->getErrorOpis() );  return 1;  }
    if(logLevel>=1){  Rap( ads1256_0->OpisRejestrow() );    }
    return 0;
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
//-------------------------------------------------------------------------------------
int initSocket(){
    dbrSS = new atDataBuffer( AT_MODBUS_ID_PI, AT_SOCKET_FRAME_LEN,AT_MODBUS_REQUNAG );
    dbwSS = new atDataBuffer( AT_MODBUS_ID_PI, AT_SOCKET_FRAME_LEN,AT_MODBUS_RESPNAG );
   // atSS  = new atSocketServer( dbrSS, dbwSS, NULL,AT_SOCKET_PORTNO );
    atSS  = new atSocketServer( dbrSS, dbwSS, &serverGoFun,AT_SOCKET_PORTNO );
    return 0;
}
//-------------------------------------------------------------------
void initVars(){
    FileRap= new at_File("/var/log/SD_server","SD_server",".log");
    Rap("program SD");
    cnf=new at_Konfiguracja("config/config");
    logLevel=AT_LOG_ERRORS;
    loopIndex=0;
}

//------------------------------------------------------------//
//                   M A I N                                  //
//------------------------------------------------------------//
int main(int argc, char *argv[]) {
    signal(SIGINT, Handler);
    initVars();
    if( initSocket() ) return 1;
    if( initADS() ) return 1;
    if( initX3() ) return 1;
    kal=new programSD(cnf);
    kal->confRap(FileRap,AT_LOG_WORK);
    kal->setDev(ads1256_0,X3Motor,X3Wave);
    kal->setAmpl(32767,45874,6553);
    kal->setFreq(5,30,5);
    int err=kal->ini();
    if(err){ printf("Error: 0x"); printf(IntToHex(err).c_str()); printf("\n"); koniec(); exit(1); }
    //--------------------------------------------------------
    if (argc>1 ){
        printf("......argc\n");
        string arg=argv[1];
        if(arg=="pomiar")  { printf("pomiar started\n"); kal->start_pomiar(); }
        else if(arg=="up") { printf("up started\n"); kal->start_up(); }
        else if(arg=="dn") { printf("dn started\n"); kal->start_dn(); }
    }
    //--------------------------------------------------------------------
    while(1)    {
        atSS->go();
        int err=kal->go();
        if(err){ printf("Error: 0x"); printf(IntToHex(err).c_str()); printf("\n"); break; }
        loopIndex++;
    }
    //--------------------------------------------------------------------
    koniec();
    return 0;
}
