#ifndef KALIBRACJASD_ADS_H
#define KALIBRACJASD_ADS_H

#include "atSPI_ads1256.h"
#include "at_Konfiguracja.h"

#define KALIBRACJA_STATUS_OFF   0
#define KALIBRACJA_STATUS_START 1
#define KALIBRACJA_STATUS_GAIN1 2
#define KALIBRACJA_STATUS_GAIN2 3
#define KALIBRACJA_STATUS_DONE  4

#define KALIBRACJA_OK_COUNT 200
#define KALIBRACJA_MAXGND1 20000
#define KALIBRACJA_MAXGND2 80000
#define KALIBRACJA_MAXR1 10000
#define KALIBRACJA_MAXR2 10000


class kalibracja_ads
{
private:
    atSPI_ADS1256* ads1256_0;
    int status;

    int32_t m0,p0,l0,r0; //GND, plus, 1/2, róznica
    int64_t Sm0,Sp0,Sl0; //sumy GND, plus, 1/2
    int N; //iliść próbek w sumie
    unsigned ok0;
    double v0;

    at_Konfiguracja *cnf;

public:
    kalibracja_ads(at_Konfiguracja *c){cnf=c; status=KALIBRACJA_STATUS_OFF;}
    void setDev(atSPI_ADS1256* a0){ads1256_0=a0; }
    void initGain(ADS1256_GAIN_E g){ ads1256_0->initGain(g);}
    bool active(){return status!=KALIBRACJA_STATUS_OFF;}
    void start(){status=KALIBRACJA_STATUS_START;}
    bool go();
    void getVars();
    bool checkGain1();
    bool checkGain2();
    void disp();
};

#endif // KALIBRACJASD_ADS_H
