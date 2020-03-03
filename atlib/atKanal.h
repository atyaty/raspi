#ifndef ATKANAL_H
#define ATKANAL_H

#include <QString>
#include <atFunctions.h>
#include <atSPI_ads1256.h>

class atKanalT {
private:
    int NrKanal;
    double R;              //Opór rezystora w dzielniku napięcia
    int32_t przelicznik;   //Przelicznik napięcia (bezpośredni odczyt dzilimy przez tę liczbę żeby dostać napięcie w woltach)


    atBufor* bufor;
    double v,r,T;          //napięcie

public:
    atKanalT(int nr, double rr,atBufor* b){/*reset();NrKanal=nr;R=rr;*/} //nr kanalu, opornik w k_omh
    void setValue(int32_t u);
    void setValue(){/*atBufor->next(); setValue(atBufor->getValueSr());*/ }
    double getV(){return v;}
    double getR(){return r;}
    double getT(){return T;}

};


#endif // ATKANAL_H
