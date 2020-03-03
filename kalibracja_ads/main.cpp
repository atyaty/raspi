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
#include <kalibracja_ads.h>
#include <atStoper.h>

//------------------------------------------------

#define I2C_PI_ID 0x56


atSPI* spi;
atSPI_ADS1256 *ads1256_0;
at_File *FileRap;
at_Konfiguracja *cnf;
kalibracja_ads* kal;
uint64_t loopIndex;
int logLevel;



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
  delete spi;
  delete ads1256_0;
  delete FileRap;
  delete cnf;
  delete kal;
}
//--------------------------------
void  Handler(int signo){ koniec();  exit(0); }
//-------------------------------------------------------------------
int initADS(string ads){
    uint8_t pinCE=ADS1256_PINCE_15;
    if(ads=="ADS1")pinCE=ADS1256_PINCE_16;
    else if(ads=="ADS2")pinCE=ADS1256_PINCE_18;
    ads1256_0  = new atSPI_ADS1256( pinCE ,ads);
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
//-------------------------------------------------------------------------------------
void initVars(){
    FileRap= new at_File("/var/log/SD_server","SD_server",".log");
    Rap("Kalibracja ADS1256");
    cout <<"\n\nProszę włożyć płytki kalibracyjne.\n\n\n";
    cnf=new at_Konfiguracja("config/config");
    logLevel=AT_LOG_ERRORS;
    loopIndex=0;
}


//------------------------------------------------------------//
//                   M A I N                                  //
//------------------------------------------------------------//
int main(int argc, char *argv[]) {
    if(argc!=2){ cout<< "podj parametr.. ADS0 lub ADS1 lub ADS2\n"; return 1;}
    string arg=argv[1];
    //string arg="ADS0";
    if( arg!="ADS0" && arg!="ADS1" && arg!="ADS2"){ cout<< "prawidłowa składnia: kalibracja_ads ADS0\n..ewentualnie ADS1 lub ADS2\n"; return 1;}

    signal(SIGINT, Handler);
    initVars();
    if( initADS(arg) ) return 1;

    kal=new kalibracja_ads(cnf);
    kal->setDev(ads1256_0);
    kal->start();
    //--------------------------------------------------------------------
    while(1)    {
        ads1256_0->scan();
        if(kal->go()){
            cout <<string(20,'\n');
            if(logLevel>=1){  Rap("Loop Index: "+to_string(loopIndex));  }
            cout <<"Koniec klaibracji.\nWyjmij płytki!\n\n";
            break;
        }
        loopIndex++;
    }
    koniec();
    return 0;
}
