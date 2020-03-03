#ifndef KALIBRACJA_ADS_H
#define KALIBRACJA_ADS_H

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
    atSPI_ADS1256* ads1256_1;
    atSPI_ADS1256* ads1256_2;
    int status;

    int32_t m0,m1,m2,p0,p1,p2,l0,l1,l2,r0,r1,r2; //GND, plus, 1/2, róznica
    int64_t Sm0,Sm1,Sm2,Sp0,Sp1,Sp2,Sl0,Sl1,Sl2; //sumy GND, plus, 1/2
    int N; //iliść próbek w sumie
    unsigned ok0,ok1,ok2;
    double v0,v1,v2;

    at_Konfiguracja *cnf;

public:
 //   kalibracja_ads(){ status=KALIBRACJA_STATUS_OFF;}
    kalibracja_ads(at_Konfiguracja *c){cnf=c; status=KALIBRACJA_STATUS_OFF;}
    void setDevs(atSPI_ADS1256* a0,atSPI_ADS1256* a1,atSPI_ADS1256* a2){ads1256_0=a0;ads1256_1=a1;ads1256_2=a2; }
    void initGain(ADS1256_GAIN_E g){ ads1256_0->initGain(g);ads1256_1->initGain(g);ads1256_2->initGain(g);}
    bool ative(){return status!=KALIBRACJA_STATUS_OFF;}
    void start(){status=KALIBRACJA_STATUS_START;}
    bool go();
    void getVars();
    bool checkGain1();
    bool checkGain2();
    void disp();
};

#endif // KALIBRACJA_ADS_H
