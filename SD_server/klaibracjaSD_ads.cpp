#include "kalibracjaSD_ads.h"

bool kalibracja_ads::go() {

    if(status==KALIBRACJA_STATUS_START) {
        initGain(ADS1256_GAIN_1);
        ok0=0;N=0;
        status=KALIBRACJA_STATUS_GAIN1;
    }
    else if(status==KALIBRACJA_STATUS_GAIN1) {
        getVars();
        if(checkGain1()){
            initGain(ADS1256_GAIN_2);
            ok0=0; N=0; Sm0=0; Sp0=0; Sl0=0;
            status=KALIBRACJA_STATUS_GAIN2;
        }
        disp();
    }
    else if(status==KALIBRACJA_STATUS_GAIN2) {
        getVars();
        if(checkGain2()){ status=KALIBRACJA_STATUS_DONE;}
        disp();
    }
    else if(status==KALIBRACJA_STATUS_DONE){
        cout <<string(20,'\n');
        cout <<"Koniec klaibracji.\nWyjmij płytki!\n\n";
        return true;
    }
    return false;
}
//-------------------------------------------------------------------------------
void kalibracja_ads::getVars() {
    m0=(ads1256_0->getADC_buforSr(1)+ads1256_0->getADC_buforSr(3)+ads1256_0->getADC_buforSr(5)+ads1256_0->getADC_buforSr(7))/4;
    p0=(ads1256_0->getADC_buforSr(0)+ads1256_0->getADC_buforSr(2)+ads1256_0->getADC_buforSr(6))/3;
    l0=ads1256_0->getADC_buforSr(4);
    r0=(p0-m0)/2-(l0-m0);
    if(ok0){ Sm0+=m0; Sp0+=p0; Sl0+=l0; N++;  }
    else{    Sm0=0; Sp0=0; Sl0=0; N=0;    }

    if(status==KALIBRACJA_STATUS_GAIN1) {
        v0=((double)(p0-m0))*ADS1256_VREF_INTERNAL/ADS1256_DZIELNIK;
    }
    else{
        v0=((double)(l0-m0))*ADS1256_VREF_INTERNAL/ADS1256_DZIELNIK;
    }
}
//-------------------------------------------------------------------------------
bool kalibracja_ads::checkGain1() {
    if( m0>KALIBRACJA_MAXGND1 || abs(r0)>KALIBRACJA_MAXR1  ){ok0=0;} else{ ok0++; }
    if(N>KALIBRACJA_OK_COUNT){
        cnf->setInt(ads1256_0->getName()+"."+to_string(ADS1256_GAIN_1)+".m",Sm0/N);
        cnf->setInt(ads1256_0->getName()+"."+to_string(ADS1256_GAIN_1)+".p",Sp0/N);
        cnf->setInt(ads1256_0->getName()+"."+to_string(ADS1256_GAIN_1)+".l",Sl0/N);
        cnf->ZapiszPlikBak(".bak");
        return true;
    }
    return false;
}
//-------------------------------------------------------------------------------
bool kalibracja_ads::checkGain2() {
    if(p0!=ADS1256_MAXBUFFER || m0>KALIBRACJA_MAXGND2  ){ok0=0;} else{ ok0++; }
    if(N>KALIBRACJA_OK_COUNT){
        cnf->setInt(ads1256_0->getName()+"."+to_string(ADS1256_GAIN_2)+".m",Sm0/N);
        cnf->setInt(ads1256_0->getName()+"."+to_string(ADS1256_GAIN_2)+".p",Sp0/N);
        cnf->setInt(ads1256_0->getName()+"."+to_string(ADS1256_GAIN_2)+".l",Sl0/N);
        cnf->ZapiszPlikBak(".bak");
        return true;
    }
    return false;
}
//--------------------------------
void kalibracja_ads::disp() {
   int32_t P=0,L=0,M=0,P0=0,P1=0,P2=0,L0=0,L1=0,L2=0,M0=0,M1=0,M2=0,R0=0,R1=0,R2=0,roz0=0,roz1=0,roz2=0;
   if(N!=0){
       P=(Sp0+Sp1+Sp2)/(3*N);
       L=(Sl0+Sl1+Sl2)/(3*N);
       M=(Sm0+Sm1+Sm2)/(3*N);
       P0=Sp0/N;
       P1=Sp1/N;
       P2=Sp2/N;
       L0=Sl0/N;
       L1=Sl1/N;
       L2=Sl2/N;
       M0=Sm0/N;
       M1=Sm1/N;
       M2=Sm2/N;
       roz0=(P0-M0)/2-(L0-M0);
       roz1=(P1-M1)/2-(L1-M1);
       roz2=(P2-M2)/2-(L2-M2);
       R0=(L0-M0);
       R1=(L1-M1);
       R2=(L2-M2);

   }


    printf("(%1i)        ADS0       ADS1       ADS2\n",status) ;
    printf("           %2i       %2i       %2i\n",ads1256_0->readGain(),ads1256_1->readGain(),ads1256_2->readGain()) ;
    printf("VCC: %10i %10i %10i\n",ads1256_0->getADC_buforSr(0),ads1256_1->getADC_buforSr(0),ads1256_2->getADC_buforSr(0));
    printf("GND: %10i %10i %10i\n",ads1256_0->getADC_buforSr(1),ads1256_1->getADC_buforSr(1),ads1256_2->getADC_buforSr(1));
    printf("VCC: %10i %10i %10i\n",ads1256_0->getADC_buforSr(2),ads1256_1->getADC_buforSr(2),ads1256_2->getADC_buforSr(2));
    printf("GND: %10i %10i %10i\n",ads1256_0->getADC_buforSr(3),ads1256_1->getADC_buforSr(3),ads1256_2->getADC_buforSr(3));
    printf("1/2: %10i %10i %10i\n",ads1256_0->getADC_buforSr(4),ads1256_1->getADC_buforSr(4),ads1256_2->getADC_buforSr(4));
    printf("GND: %10i %10i %10i\n",ads1256_0->getADC_buforSr(5),ads1256_1->getADC_buforSr(5),ads1256_2->getADC_buforSr(5));
    printf("VCC: %10i %10i %10i\n",ads1256_0->getADC_buforSr(6),ads1256_1->getADC_buforSr(6),ads1256_2->getADC_buforSr(6));
    printf("GND: %10i %10i %10i\n",ads1256_0->getADC_buforSr(7),ads1256_1->getADC_buforSr(7),ads1256_2->getADC_buforSr(7));
    printf("\n");
    printf("VCC: %10i %10i %10i (%10i)\n",P0,P1,P2,P);
    printf("GND: %10i %10i %10i (%10i)\n",M0,M1,M2,M);
    printf("Lin: %10i %10i %10i (%10i)\n",L0,L1,L2,L);
    printf("roz: %10i %10i %10i\n",roz0,roz1,roz2);
    printf("L-M: %10i %10i %10i\n",R0,R1,R2);
    printf(" ok: %10i %10i %10i (%4i)\n",ok0,ok1,ok2,N);
    printf("\n");
    printf("  V:  %2.7f  %2.7f  %2.7f\n",v0,v1,v2 );

//    if(klaibracja_ok>300){ printf("\nOK, kalibracja zakończona.\n Wyjmij płytki kalibracyjne!!!\n"); return 1;}
    printf("\33[19A");

//    return 0;


}
