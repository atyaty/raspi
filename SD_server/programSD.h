#ifndef PROGRAMSD_H
#define PROGRAMSD_H

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
#include <atStoper.h>
#include <stdint.h>

#define SD_BUFOR_LEN    512
#define SD_SAMPLE_TIME  1000    //czas przygotowania/pomiaru [ms]
                                      // głównw procedury programu
                                      // są wymuszane przez PC za pośrednictwem SS
#define SD_STATUS_STOP          0     //oczekiwanie na pomiar
#define SD_STATUS_POMIAR        1     //pomiar
#define SD_STATUS_GOUP          2     //płyta w górę
#define SD_STATUS_GODN          3     //płyta w dół

                                      //procedura pomiaru - etapy
                                      //etap start zapoczątkwany jest przez proc główną,
                                      //pozostałe etapy uruchamiają sie nawzajem
#define SD_POMIAR_STOP          0     //pomiar zatrzymany
#define SD_POMIAR_START         1     //pomiar puktu zakończony
#define SD_POMIAR_ZWLOKA        2     //zwłoka przezd zapisem danych pukntu
#define SD_POMIAR_GETDATA       3     //zbieranie danych
#define SD_POMIAR_WYNIK         4     //

class programSD {
private:
    uint16_t Smin, Smax, Sd, S;
    uint16_t fmin, fmax, fd, f;
    uint16_t input[SD_BUFOR_LEN];
    uint16_t nWrite,nSend;  //wskaźnik zapsu do bufora, wskaźnik wysyłki do PC
    uint32_t pomiar_time;
    //-----------------------------
    atDataBuffer *dbr;
    atDataBuffer *dbw;
    atSocketServer* atSS;
    atI2C* ati2c;
    atI2C_X3* X3Motor;
    atI2C_X3* X3Wave;
    atSPI* spi;
    atSPI_ADS1256 *ads;
    at_File *FileRap;
    at_Konfiguracja *cnf;
    atStoper* stoper;
    int status,pomiar;
    int Error;
    //-------------------------------
    uint16_t X3WaveDataLen; //długość danych w ramce send - przenieść zmienną do bufora..?


public:
    programSD();
    ~programSD();
    void pomiar_start(){status=SD_STATUS_POMIAR;pomiar=SD_POMIAR_START;}
    bool go();
    bool goUp();
    bool goDn();
    bool goPomiar();
    bool goSS();
    bool pomiarStart();
    bool pomiarZwloka();
    bool pomiarGetData();
    bool pomiarWynik();

};

#endif // PROGRAMSD_H
