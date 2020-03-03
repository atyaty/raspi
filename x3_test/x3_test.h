#ifndef programSD_H
#define programSD_H

#include "atSPI_ads1256.h"
#include "at_Konfiguracja.h"
#include <atI2C_X3.h>

#define SD_BUFOR_LEN    256
#define SD_SAMPLE_TIME  2000    //czas przygotowania/pomiaru [ms]

#define SD_STATUS_STOP          0     //wyłącz
#define SD_STATUS_POMIAR        1     //pomiar
#define SD_STATUS_GOUP          2     //płyta w górę
#define SD_STATUS_GODN          3     //płyta w dół
#define SD_STATUS_OFF           4     //oczekiwanie
#define SD_STATUS_ZATRZYMAJ     5     //zatrzymanie silnika

#define SD_POMIAR_STOP          0     //pomiar zatrzymany
#define SD_POMIAR_START         1     //pomiar puktu zakończony
#define SD_POMIAR_ZWLOKA        2     //zwłoka przezd zapisem danych pukntu
#define SD_POMIAR_GETDATA       3     //zbieranie danych
#define SD_POMIAR_WYNIK         4     //
#define SD_POMIAR_SETWAVE       5
#define SD_POMIAR_NEXT          6
#define SD_POMIAR_FINISHED      7

#define SD_UP_STOP              0
#define SD_UP_START             1
#define SD_UP_WAIT              2

#define SD_DN_STOP              0
#define SD_DN_START             1
#define SD_DN_WAIT              2

#define SD_ZATRZYMAJ_STOP       0  //procedura zatrzymywania silnika
#define SD_ZATRZYMAJ_START      1
#define SD_ZATRZYMAJ_WAIT       2

#define SD_ZASILANIE_STOP       0
#define SD_ZASILANIE_GOING      1

#define SD_LEVEL_RAP_DEBUG      5
#define SD_LEVEL_RAP_WORK       2
#define SD_LEVEL_RAP_NIC        0

//test gita
//druga linijka
class programSD {
private:

    uint16_t Ampl_min, Ampl_max, Ampl_d, Ampl;
    uint16_t Freq_min, Freq_max, Freq_d, Freq;
    int32_t input[SD_BUFOR_LEN];
    uint16_t nWrite,nSend;  //wskaźnik zapsu do bufora, wskaźnik wysyłki do PC
    uint32_t pomiar_time;
    //-------------------------------------------
    atI2C_X3* X3Motor;
    atI2C_X3* X3Wave;
    atSPI_ADS1256* ads;
    //-----------------------------------
    at_Konfiguracja *cnf;
    atStoper stoper,zasilanie;
    at_File *FileRap;          //
    int LevelRap;              //
    int Error;
    int stat,stat_pomiar,stat_up,stat_dn,stat_zatrzymaj,stat_zasilanie;
    bool upStop,dnStop;
    int32_t motor_pos;
    //-------------------------------
    uint16_t X3WaveDataLen; //długość danych w ramce send - przenieść zmienną do bufora..?
    uint16_t X3MotorDataLen;
    uint32_t X3Motor_freq;   //częstotliwość pracy motora

    //----------------------------Procedury są prywatne - do użycia tylko wewnątrz klasy
    bool goUp();
    bool goDn();
    bool goZatrzymaj();
    bool goPomiar();
    bool pomiarStart();
    bool pomiarZwloka();
    bool pomiarGetData();
    bool pomiarWynik();
    bool pomiarNext();
    bool pomiarSetWave();
    bool upStart();
    bool upWaiting();
    bool dnStart();
    bool dnWaiting();
    bool zatrzymajStart();
    bool zatrzymajWaiting();

    int debug;

public:
    programSD(at_Konfiguracja *c);
    //void setDev(atSPI_ADS1256* a0,atI2C_X3* m, atI2C_X3* w){ads=a0; X3Motor=m; X3Wave=w; }
    void setDev( atI2C_X3* m, atI2C_X3* w){ X3Motor=m; X3Wave=w; }
    void setAmpl(    uint16_t min, uint16_t max, uint16_t d){ Ampl_min=min;Ampl_max=max;Ampl_d=d; }
    void setFreq(    uint16_t min, uint16_t max, uint16_t d){ Freq_min=min;Freq_max=max;Freq_d=d; }
    void confRap(at_File *f,int l){FileRap=f;LevelRap=l;}

    //----------------------------Start procedur może odbywać się z zewnątrz
    void start_pomiar()   ;// {stat=SD_STATUS_POMIAR; stat_pomiar=SD_POMIAR_START; }
    void start_up()       ;// {stat=SD_STATUS_GOUP;   stat_up=SD_UP_START; }
    void start_dn()        ;//{stat=SD_STATUS_GODN;   stat_dn=SD_DN_START; }
    void start_zatrzymaj() ;//{stat=SD_STATUS_ZATRZYMAJ;   stat_zatrzymaj=SD_ZATRZYMAJ_START; }
    int go();   //zdazenie do glownej petli programu
    int ini();  //inicjowanie programu
    bool Zasilanie();  //funkcja kontrolująca zasilacze

    //----------------------------Standardowa komunikacja przez modbus
    void DataProcess(atDataBuffer *mbRece,atDataBuffer *mbSend);
    void setFromModbus(atDataBuffer *mbRece);
    void putToModbus(atDataBuffer *mbRece,atDataBuffer *mbSend);
    //----------------------------
    void setCnf() ;
    void Rap(string s,int L);
};

#endif // programSD_H
