#ifndef ATSPI_H
#define ATSPI_H

#include <atSPI_dev.h>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <bcm2835.h>
#include <atErrors.h>
#include <string>
#include <at_File.h>
#include <at_Konfiguracja.h>

#define DEV_RST_PIN     18 //uwaga powtórzone


using namespace std;
//---------------------------------------
class atSPI_dev;
//---------------------------------------
class atSPI {
private:
    string sDev;                        //na razie nie uzywane
    vector <atSPI_dev*> devs;           //tablica urządzeń
    bcm2835SPIClockDivider ClockDivider;

    int Error;                 //
    at_File *FileRap;          //
    int LevelRap;              //
public:
    atSPI(string s);
    ~atSPI();
    void addDev(atSPI_dev* d);
    bool Init();
    void go();
    void go(unsigned i);

    int getError(){ return Error; }
    string getErrorOpis();
    void confRap(at_File *f,int l){FileRap=f;LevelRap=l;}
    void Rap(string s,int l){ if(FileRap!=NULL && l<=LevelRap ) FileRap->Append(s); }
    int setParams(at_Konfiguracja* cnf);
};

#endif // ATSPI_H
