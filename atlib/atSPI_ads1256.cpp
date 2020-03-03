/*at test*/
 
#include "atSPI_ads1256.h"
#include "atSPI.h"

//---------------------------------------------------------------------------------
atSPI_ADS1256::atSPI_ADS1256(uint8_t p,string N):atSPI_dev(p,N){ len=0;}
//---------------------------------------------------------------------------------
void atSPI_ADS1256::Config(ADS1256_SCANMODE _ucScanMode,ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate){
    len      = _ucScanMode & 0x0F; //połówka bajtu zawiera informację o długości bufora
    ScanMode = _ucScanMode;
    Gain     = _gain;
    for(int ch=0;ch<len;ch++) {    bufor[ch].setGain(Gain);   }
    DataRate = _drate;
    Channel  = 0;
    id       = 0;
    reset();
}
//--------------------------------------------------------------------------
bool atSPI_ADS1256::Init(void) {
    atStoper* stoper=new atStoper(1000000000);
    if(!atSPI_dev::Init()) return false;

    while (id!=3 && stoper->waiting()){
        reg_status=readReg( REG_STATUS );
        id = (reg_status >> 4);
    }
    if(id!=3){ Error=ATERROR_ADS1256_ID; return false; }

    writeCnf();
    return true;
}
//------------------------------------------------------
void atSPI_ADS1256::writeCnf(  ){
    dbr->Clear();
    dbw->Clear();
    dbw->buforAddByte( CMD_WREG | REG_STATUS );
    dbw->buforAddByte( 0x03 );
    dbw->buforAddByte( (0 << 3) | (1 << 2) | (0 << 1) );
    dbw->buforAddByte( 0x08 );
    dbw->buforAddByte( (0 << 5) | (0 << 3) | (Gain << 0) );
    dbw->buforAddByte( DataRate );
    setON();
    go();
    setOFF();
    delay_ns(SPI_DELAY_CFG);
}
//------------------------------------------------------
uint8_t atSPI_ADS1256::readReg( uint8_t reg ){
    setON();
    dbw->Clear();
    dbw->buforAddByte( CMD_RREG |reg);
    dbw->buforAddByte( 0x00);
    go();
    delay_ns(SPI_DELAY_READREG);
    dbw->Clear();
    dbw->buforAddByte( 0xff);
    go();
    setOFF();
    return dbr->buforGetByte(0);
}
//------------------------------------------------------
void atSPI_ADS1256::writeReg( uint8_t reg,uint8_t val ){
    setON();
    dbw->Clear();
    dbw->buforAddByte( CMD_WREG |reg);
    dbw->buforAddByte( 0x00);
    dbw->buforAddByte( val );
    go();
    setOFF();
    return;
}
//-------------------------------------------------------------------------
void atSPI_ADS1256::writeCmd(uint8_t cmd) {
    setON();
    dbw->Clear();
    dbw->buforAddByte( cmd);
    go();
    setOFF();
}
//--------------------------------------------------------------------------
int32_t atSPI_ADS1256::readData(void){
    setON();
    dbw->Clear();
    dbw->buforAddByte( CMD_RDATA);
    go();
    delay_ns(SPI_DELAY_READDATA);
    dbw->Clear();
    dbw->buforAddByte(0xff);
    dbw->buforAddByte(0xff);
    dbw->buforAddByte(0xff);
    go();
    setOFF();
    return dbr->getInt24(0);
}

//------------------------------------------------------
// czy dane są gotowe
bool atSPI_ADS1256::DRDY(  ){
    readReg( REG_STATUS );
    return !dbr->buforGetBit( dbr->getDataLen()-1, 0);
}
//--------------------------------------------------------------------------
void atSPI_ADS1256::setChannel(uint8_t _ch) {
   if(ScanMode==SCANMODE_DIFFERENTIAL){  writeReg(REG_MUX, (_ch*2 << 4) | ((_ch*2)+1) );   }
   else if(ScanMode==SCANMODE_NEGATIVE){ writeReg(REG_MUX, (_ch &&0b00000111) | (1<<7) );   }
   else{                                 writeReg(REG_MUX, (_ch << 4)   | (1 << 3)  );   }
}
//--------------------------------------------------------------------------
uint8_t atSPI_ADS1256::scan(void){
    if(!DRDY()) return 0;
    uint8_t Channel_next=Channel+1;
    if (Channel_next >= len){ Channel_next = 0;	}
    setChannel(Channel_next);    delay_ns(SPI_DELAY_WRITECMD); //dla szybkości przestawiamy od razu przetwornik na następny kanał, żeby już zaczął pracować
    writeCmd(CMD_SYNC);          delay_ns(SPI_DELAY_WRITECMD);
    writeCmd(CMD_WAKEUP);        delay_ns(SPI_DELAY_READCHANN);
    int32_t d=readData();
    bufor[ Channel ].add( d );                        //odczytane dane dotyczą poprzedniego cyklu, dlatego zapisujemy w poprzednim buforze
    //string s="Ch: "+to_string(Channel)+" "+to_string(Channel_next)+" "+to_string(d)+"\n";
    //dev->Rap(s,0);
    Channel=Channel_next;                                      //po odczytaniu przestawiamy nr kanału
    return 1;
}
//--------------------------------------------------------------------------
int atSPI_ADS1256::setParams(at_Konfiguracja* cnf){
    uint8_t sm=cnf->getHexInt(Name+".ScanMode");    if(cnf->getError()){ sm=0x08;}
    uint8_t ga=cnf->getInt(Name+".Gain");           if(cnf->getError()){ ga=0;}
    uint8_t dr=cnf->getHexInt(Name+".DataRate");    if(cnf->getError()){ dr=0x33;}
    Config(static_cast<ADS1256_SCANMODE>(sm),static_cast<ADS1256_GAIN_E>(ga),static_cast<ADS1256_DRATE_E>(dr));
    string sCNF=Name+"."+to_string(ga)+"_0x"+IntToHex(dr);


                                    //wskazania z płytki kalibracyjnej
                                    //kalibrację robimy dla różnych wartosći wzmocnienia
                                    //a tu odczutujemy właściwe dla wybranego ga
    int m=cnf->getInt(sCNF+".m");   //wskazanie dla zwartej masy dla wzmocnienia ga
    int p=cnf->getInt(sCNF+".p");   //wskazanie dla pełnego zakresu Vref ..
    int l=cnf->getInt(sCNF+".l");   //wskazanie z dzielnika 1/2 - czyli dla Vref/2 ..
//    if(p==0 || l==0){ p=ADS1256_MAXBUFFER; l=ADS1256_MAXBUFFER/2; Error=ATERROR_ADS1256_CNF_KALIBRACJA;}
    if(p==0){ p=ADS1256_MAXBUFFER; dev->Rap("Błąd, CNF - Kalibracja ["+sCNF+".p]\n",0);}
    if(l==0){ l=p/2; dev->Rap("Błąd, CNF - Kalibracja ["+sCNF+".l]\n",0);}
    setOffsets( m,p,l );

    double vref =cnf->getFloat(Name+".Vref");         //wpis do CFG na podst. wskazań miernika
    double R    =cnf->getFloat(Name+".R");            //wpis do CFG wgodnie z wartościa użytych oporników

                                                      //dany ADS pracuje w różnych zakresach temperatur, dlatego korzysta z różnych fragmentów charakterystyki PT1000
                                                      //poniższe zmienna mówią których punktów użyć
    string T0   =cnf->getTag(Name+".T0");             //pierwszy punkt charakterystyki do przeliczeń temperatury
    string T1   =cnf->getTag(Name+".T1");             //drugi punkt charakterystyki ..
//    if(T0=="" || T1==""){ T0="0"; T1="10"; Error=ATERROR_ADS1256_CNF_T; }
    if(T0=="" || T1==""){ T0="0"; T1="10"; dev->Rap("Błąd, CNF - T (punkty charakterystyki)\n",0); }
    double t0   =std::stod(T0);
//    double dT   =std::stod(T1)-std::stod(T0);  if(dT==0){ dT=1; Error=ATERROR_ADS1256_CNF_dT; }
//    double r0   =cnf->getFloat("PT1000."+T0);  if(cnf->getError()){ Error=ATERROR_ADS1256_CNF_PT100;}
//    double r1   =cnf->getFloat("PT1000."+T1);  if(cnf->getError()){ Error=ATERROR_ADS1256_CNF_PT100;}
    double dT   =std::stod(T1)-std::stod(T0);  if(dT==0){ dT=1; dev->Rap("Błąd, CNF - dT\n",0); }
    double r0   =cnf->getFloat("PT1000."+T0);  if(cnf->getError()){ dev->Rap("Błąd, CNF - PT1000,0\n",0);}
    double r1   =cnf->getFloat("PT1000."+T1);  if(cnf->getError()){ dev->Rap("Błąd, CNF - PT1000,1\n",0);}
    double a    =(r1-r0)/dT;
    for(int ch=0;ch<len;ch++) {
        bufor[ch].setGain(Gain);
        bufor[ch].setParams(vref,R,a,t0,r0);
    }

    return 0;
}
//--------------------------------------------------------------------------
string atSPI_ADS1256::OpisRejestrow(){

    uint32_t ofc0=readReg(REG_OFC0);
    uint32_t ofc1=readReg(REG_OFC1);
    uint32_t ofc2=readReg(REG_OFC2);
    int32_t ofc = ofc0<<0 | ofc1<<8 | ofc2<<16;
    if((ofc2>7)) ofc =ofc | (0xFF<<24);

    uint32_t fsc0=readReg(REG_FSC0);
    uint32_t fsc1=readReg(REG_FSC1);
    uint32_t fsc2=readReg(REG_FSC2);
    int32_t fsc = fsc0<<0 | fsc1<<8 | fsc2<<16;

    string sAdcon="";
    uint8_t adcon=readReg(REG_ADCON);
    adcon= adcon & 0b00000111;
    if(adcon==1)      sAdcon="2";
    else if(adcon==2) sAdcon="4";
    else if(adcon==3) sAdcon="8";
    else if(adcon==4) sAdcon="16";
    else if(adcon==5) sAdcon="32";
    else if(adcon==4) sAdcon="64";

    string s=" Rejestry "+Name+"("+to_string(getId())+")("+to_string(getRegStatus())+")";
    s+="\n stat: "+charToBit(readReg(REG_STATUS));
    s+="\n  mux: "+charToBit(readReg(REG_MUX));
    s+="\nadcon: "+charToBit(adcon)+"     --"+sAdcon;
    s+="\ndrate: "+charToBit(readReg(REG_DRATE));
    s+="\n   io: "+charToBit(readReg(REG_IO));
    s+="\n  ofc: "+int32ToBit(ofc) +" "+ to_string(ofc) ;
    s+="\n  fsc: "+int32ToBit(fsc) +" "+ to_string(fsc) ;
    s+="\n";
    return s;
}


