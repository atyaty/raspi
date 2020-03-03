/*
    ZMIANA KONCEPCJI.
    W raspberry pi, poszczególne urządzenia SPI, działające pod róznymi pinami CE traktowane są niezależnie.
    Dla każdego z nich system tworzy oddzielny dev. W programie, każde z nich dostanie oddzielny wskaźnik pliku.
    Nie ma więc potrzeby tworzenia zbiorczej klasy atSPI do której podpięte są hierarchicznie urządzenia atSPI_dev.
    Klasa atSPI_dev jest samodzielna!
*/

#ifndef ATSPI_DEV_H
#define ATSPI_DEV_H

#include <atDataBuffer.h>
#include <atErrors.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <string>
#include <cstring>
#include <bcm2835.h>
#include <atFunctions.h>
#include <at_File.h>


//-------------------------------------------------------------
class atSPI;
//-------------------------------------------------------------
class atSPI_dev{

protected:
    atSPI* dev;
    uint8_t pinCE;                              /* pin CE sterujący*/
    string Name;                                /* Nazwa nadana*/

    unsigned char spi_mode;
    unsigned char spi_bitsPerWord;
    unsigned int  spi_speed;

    atDataBuffer* dbr;
    atDataBuffer* dbw;
    bool dbOwn;     //wskaźnik że bufory są własne i trzeba je obsłużyć przy destrukcji

    int Error;      //

public:
    atSPI_dev(uint8_t p,string N,atDataBuffer*dr,atDataBuffer*dw);
    atSPI_dev(uint8_t p,string N);
    virtual ~atSPI_dev();

    void Setup(uint8_t p,string N);
    void SetupDev(atSPI* d);
    int getError(){return Error;}

    void setON ( );
    void setOFF ( );

    atDataBuffer* getDbr(){ return dbr; }
    atDataBuffer* getDbw(){ return dbw; }
    virtual bool Init() ;     //
    virtual void go();       //
    uint8_t getPinCE(){return pinCE;}
    string getName(){return Name;}
};

#endif // ATSPI_DEV_H
