#ifndef KALIBRACJA_ADS_H
#define KALIBRACJA_ADS_H

#include "atSPI_ads1256.h"
#include "at_Konfiguracja.h"

#define KALIBRACJA_STATUS_OFF   0
#define KALIBRACJA_STATUS_START 1
#define KALIBRACJA_STATUS_GOING 2


class kalibracja_ads {
private:
    atSPI_ADS1256* ads;
    int status;

    int32_t m0,p0,l0,r0; //GND, plus, 1/2, róznica
    int64_t Sm0,Sp0,Sl0; //sumy GND, plus, 1/2
    int32_t P0=0,L0=0,M0=0,R0=0,roz0=0;
    unsigned ok0; //iliść próbek w sumie
    double v0;

    at_Konfiguracja *cnf;
    atStoper stoper;
public:
    kalibracja_ads(at_Konfiguracja *c){cnf=c; status=KALIBRACJA_STATUS_OFF;}
    void setDev(atSPI_ADS1256* a0){ads=a0; }
    void reset();
    bool active(){return status!=KALIBRACJA_STATUS_OFF;}
    void start(){status=KALIBRACJA_STATUS_START;}
    bool go();
    void getVars();
    bool check();
    void disp();
    void disp1();
    void setCnf() ;
    void sum();
};
#endif // KALIBRACJA_ADS_H
