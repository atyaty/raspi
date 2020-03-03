#include "kalibracja_ads.h"
bool kalibracja_ads::go() {
    if(status==KALIBRACJA_STATUS_OFF || ads->getBuforyN()<=0){ return false; }
    if(status==KALIBRACJA_STATUS_START){ reset(); status=KALIBRACJA_STATUS_GOING; }
    disp1();
    ads->next();
    getVars();
    disp();
    if(check()){
        status=KALIBRACJA_STATUS_OFF;
        return true;
    }
    return false;
}
//-------------------------------------------------------------------------------
void kalibracja_ads::getVars() {
    m0=(ads->getADC_buforSr(1)+ads->getADC_buforSr(3)+ads->getADC_buforSr(5)+ads->getADC_buforSr(7))/4;
    p0=(ads->getADC_buforSr(0)+ads->getADC_buforSr(2)+ads->getADC_buforSr(6))/3;
    l0=ads->getADC_buforSr(4);
    r0=(p0-m0)/2-(l0-m0);
    v0=((double)(l0-m0))*ADS1256_VREF_INTERNAL/ADS1256_DZIELNIK;
}
//----------------------------------------------------
void kalibracja_ads::reset(){
    ok0=0;  Sm0=0; Sp0=0; Sl0=0;
    P0=0; L0=0;  M0=0;   roz0=0;  R0=0;
    stoper.start(2000000000);
}

//-------------------------------------------------------------------------------
void kalibracja_ads::sum() {
   ok0+=1;
   Sm0+=m0; Sp0+=p0; Sl0+=l0;
   P0=Sp0/ok0; L0=Sl0/ok0;  M0=Sm0/ok0;   R0=(L0-M0); roz0=(P0-M0)/2-R0;

}


//-------------------------------------------------------------------------------
void kalibracja_ads::setCnf() {
   string s=to_string(ads->getGain())+"_0x"+IntToHex(ads->getDataRate());
   cnf->setInt(ads->getName()+"."+s+".m",M0);
   cnf->setInt(ads->getName()+"."+s+".p",P0);
   cnf->setInt(ads->getName()+"."+s+".l",L0);
   cnf->setInt(ads->getName()+"."+s+".t",ok0);
   cnf->ZapiszPlikBak(".bak");
}

//-------------------------------------------------------------------------------
bool kalibracja_ads::check() {
    if(!ads->GainOK() || !ads->DataRateOK() ) {reset();return false;}
    if( stoper.time_ms()>1000  ){  sum();}
    if(stoper.time_ms()>2000){ setCnf();  return true;    }
    return false;
}
//--------------------------------
void kalibracja_ads::disp() {

    printf(".Status: %1i\n",status) ;
    printf("Gain: %2i, DAtaRate: %4i \n",ads->readGain(),ads->readDataRate()) ;
    printf("Kanał 0 - VCC: %10i\n",ads->getADC_buforSr(0));
    printf("Kanał 1 - GND: %10i\n",ads->getADC_buforSr(1));
    printf("Kanał 2 - VCC: %10i\n",ads->getADC_buforSr(2));
    printf("Kanał 3 - GND: %10i\n",ads->getADC_buforSr(3));
    printf("Kanał 4 - 1/2: %10i\n",ads->getADC_buforSr(4));
    printf("Kanał 5 - GND: %10i\n",ads->getADC_buforSr(5));
    printf("Kanał 6 - VCC: %10i\n",ads->getADC_buforSr(6));
    printf("Kanał 7 - GND: %10i\n",ads->getADC_buforSr(7));
    printf("\n");
    printf("VCC: %10i (%10i)\n",p0,P0);
    printf("GND: %10i (%10i)\n",m0,M0);
    printf("1/2: %10i (%10i)\n",l0,L0);
    printf("roz: %10i\n",roz0);
    printf("L-M: %10i\n",R0);
    printf(" ok: %10i \n ", ok0);
    printf("\n");
    printf("  V:  %2.7f \n",v0 );
    printf("\33[19A");

}
//--------------------------------
void kalibracja_ads::disp1() {

    printf(".Status: %1i\n",status) ;
    printf("Gain: %2i, DAtaRate: %4i \n",ads->readGain(),ads->readDataRate()) ;
    printf("Kanał 0 - VCC:             %10i %10i\n",ads->getADC_bufor(0),ads->getADC_N(0));
    printf("Kanał 1 - GND:             %10i %10i\n",ads->getADC_bufor(1),ads->getADC_N(1));
    printf("Kanał 2 - VCC:             %10i %10i\n",ads->getADC_bufor(2),ads->getADC_N(2));
    printf("Kanał 3 - GND:             %10i %10i\n",ads->getADC_bufor(3),ads->getADC_N(3));
    printf("Kanał 4 - 1/2:             %10i %10i\n",ads->getADC_bufor(4),ads->getADC_N(4));
    printf("Kanał 5 - GND:             %10i %10i\n",ads->getADC_bufor(5),ads->getADC_N(5));
    printf("Kanał 6 - VCC:             %10i %10i\n",ads->getADC_bufor(6),ads->getADC_N(6));
    printf("Kanał 7 - GND:             %10i %10i\n",ads->getADC_bufor(7),ads->getADC_N(7));
    printf("\n");
    printf("VCC: %10i (%10i)\n",p0,P0);
    printf("GND: %10i (%10i)\n",m0,M0);
    printf("1/2: %10i (%10i)\n",l0,L0);
    printf("roz: %10i\n",roz0);
    printf("L-M: %10i\n",R0);
    printf(" ok: %10i \n ", ok0);
    printf("\n");
    printf("  V:  %2.7f \n",v0 );
    printf("\33[19A");

}
