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
#include <atFileLog.h>
#include <atFunctions.h>
#include <time.h>
#include <signal.h>     //signal()
#include <at_Konfiguracja.h>
#include <atStoper.h>

//------------------------------------------------


atDataBuffer *dbrSS;
atDataBuffer *dbwSS;
atSocketServer* atSS;

atFileLog *FileRap;
at_Konfiguracja *cnf;
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
  cout <<"Koniec programu socket_server.\n";
  delete atSS;
  delete dbrSS;
  delete dbwSS;
  delete FileRap;
  delete cnf;
}
//-------------------------------------------------------------------
void  Handler(int signo){ koniec();  exit(0); }
//-------------------------------------------------------------------
int serverGoFun( atDataBuffer *dbr, atDataBuffer *dbw ){
    if( !dbr->checkCRC() ){
         dbw->buforSetNag(AT_MODBUS_ID_PI,0x80,0,0, ATERROR_SOCKET_CRC ); dbw->buforAddCRC(); errCRC++;
    }
    else if(  dbr->checkId(AT_MODBUS_ID_PI) ) {
         dbw->buforSetNag(dbr); dbw->buforAddReg(0x77); dbw->buforAddCRC();
    }
    else {
        dbw->buforSetNag(AT_MODBUS_ID_PI,0x80,0,0, ATERROR_SOCKET_ID_DEVICE ); dbw->buforAddCRC();           errID++;
    }
    if( dbr->getDataLen() !=0 ){ //to tylko zapis niepustych ramek do pliku
        if(logLevel>=4)Rap("Socket: ["+dbr->charToHex(  )+">>]  <<"+dbw->charToHex(  )+"\n");
        dbr->Clear();
    }
    return 0;

}

//-------------------------------------------------------------------------------------
int initSocket(){
    dbrSS = new atDataBuffer( AT_MODBUS_ID_PI, AT_SOCKET_FRAME_LEN,AT_MODBUS_REQUNAG );
    dbwSS = new atDataBuffer( AT_MODBUS_ID_PI, AT_SOCKET_FRAME_LEN,AT_MODBUS_RESPNAG );
    atSS  = new atSocketServer( dbrSS, dbwSS, &serverGoFun,AT_SOCKET_PORTNO );
    return 0;
}
//-------------------------------------------------------------------
void initVars(){
    FileRap= new atFileLog("","socket_server",".log",AT_LOG_ZAWSZE);
    Rap("program: socket_server");
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
    //--------------------------------------------------------------------
    while(1)    {
        atSS->go();
        loopIndex++;
    }
    //--------------------------------------------------------------------
    koniec();
    return 0;
}
