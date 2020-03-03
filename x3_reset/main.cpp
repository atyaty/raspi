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
#include <atFileLog.h>
#include <atFunctions.h>
#include <time.h>
#include <signal.h>     //signal()
#include <at_Konfiguracja.h>
#include <atStoper.h>
#include <x3_reset.h>
#include <atGPIO.h>

#define AT_GPIO_RESET 20
#define AT_GPIO_E5    26
//------------------------------------------------

atGPIO* gpio;
atFileLog *FileRap;
at_Konfiguracja *cnf;




//------------------------------------------------------------------
void koniec(){
  FileRap->Rap("Koniec programu \n",AT_LOG_ZAWSZE);
  cout <<"Koniec programu \n";
  delete gpio;
  delete FileRap;
  delete cnf;

}
//-------------------------------------------------------------------
void  Handler(int signo){ koniec();  exit(0); }
//-------------------------------------------------------------------
void initVars(){
    cnf=new at_Konfiguracja("config/config");
    int logLevel=cnf->getInt("logLevel");
    string logPath=cnf->getTag("logPath");
    FileRap= new atFileLog(logPath,"x3_reset",".log",logLevel);

}

//------------------------------------------------------------//
//                   M A I N                                  //
//------------------------------------------------------------//
int main(int argc, char *argv[]) {
    signal(SIGINT, Handler);
    initVars();
    string tryb_pracy="Program x3_reset: reset \n";
    if (argc>1 && ((string)argv[1])=="install"){ tryb_pracy="Program x3_reset: install \n";}
    cout << tryb_pracy;
    FileRap->RapDate(tryb_pracy,AT_LOG_ZAWSZE);
    gpio= new atGPIO();
    gpio->confRap(FileRap);
    gpio->Init();
    if(gpio->IsError() ){ return 1;}
    gpio->SetOutput(AT_GPIO_RESET);
    gpio->SetOutput(AT_GPIO_E5);

    //--------------------------------------------------------
    if (argc>1 && ((string)argv[1])=="install"){
        gpio->writeGPIO(AT_GPIO_E5,0);
    }
    else{
        gpio->writeGPIO(AT_GPIO_E5,1);
    }
    delay_ms(200);
    gpio->writeGPIO(AT_GPIO_RESET,0);
    delay_ms(200);
    gpio->writeGPIO(AT_GPIO_RESET,1);
    koniec();
    return 0;
}
