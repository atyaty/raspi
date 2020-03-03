/*
1. Rozdzielić funkcjonalnośc okan głównego na okienka dedykowane:
   a) Konfiguracja parametrów fizycznych drukarki
   b) Konfiguracja komunikacji z hostem
   c) Okienko do generowania plików z obliczeniami dynamicznymi
   d) okienko do ręcznego uruchamiania linii GCode
2. Zrobic moduł czytania komend GCode z pliku
   - czytamy
   - konwersja do KBAOR
   - wysyłka do kosta
   - odczyt potwierdzenia o hosta
   - interpretacja i obsługa błędów
3. przewidzieć, że może być wiele komunikatów zwrotnych z hosta w ramach jednej sesji (jednej linijki GCode)



*/

#include <atSocketServer.h>
#include <atDataBuffer.h>
#include <at_X3.h>
#include <at_X3Error.h>
#include <atI2C.h>
#include <atI2C_X3.h>
#include <atSPI_ads1256.h>
#include <atSPI.h>
#include <at_File.h>
#include <time.h>
#include <signal.h>     //signal()
#include <at_Konfiguracja.h>
#include <kalibracja_ads.h>
#include <atStoper.h>

//------------------------------------------------

#define I2C_PI_ID 0x56

atDataBuffer *dbrSS;
atDataBuffer *dbwSS;
atSocketServer* atSS;

atI2C* ati2c;
atI2C_X3* X3Motor;
atI2C_X3* X3PWM;

atSPI* spi;
atSPI_ADS1256 *ads1256_0,*ads1256_1,*ads1256_2;

atFileLog *FileRap;
at_Konfiguracja *cnf;

kalibracja_ads* kal;

uint32_t devADS_0,devADS_1,devADS_2,devX3Motor,devX3PWM,devPI,devError; //liczniki ramek adresowanych do poszczególnych urządzeń
uint32_t errADS,errADS_0,errADS_1,errADS_2,errX3Motor,errX3PWM,errPI,errDev,errCRC,errX3;          //liczniki błędów poszczególnych urządzeń, na poziomie funkcji komunikacyjnych
uint64_t ErrorsADS_0,ErrorsADS_1,ErrorsADS_2,ErrorsX3Motor,ErrorsX3PWM;    //liczniki błędów urządzeń na poziomie procedur wykonawczych
uint64_t socketFrames,loopIndex;

time_t czas;
struct tm* czas_info;
int dzien;

int logLevel;
string logDev;
int logCH0,logCH1;



//------------------------------------------------------------
void Rap(string s){
    string s1=Time_to_string(czas_info)+":  "+s+"\n";
    FileRap->Append(s1);
    printf(s1.c_str());

}
//------------------------------------------------------------
void dispHelp(){
    printf("Użycie programu socket_server:\n");
    printf("1: socket_server\n");
    printf("2: socket_server kalibracja\n");
    printf("3: socket_server ads# ch_od ch_do log_level\n");
    printf("     ads# - który przetwornik [ADS0,ADS1,ADS2]\n");
    printf("     ch_od - kanał początkowy\n");
    printf("     ch_do - kanał końcowy\n");
    printf("     log_level - szczegółowośc raportowania [0-tylko błędy,..,10-max]\n\n");
    printf("---------------------------------------------------------------------\n");
    printf("Procedura instalacji i kalibracji socket_sever\n");
    printf("0. Skonfigurować interfejsy PI'a:\n");
    printf("   -włączyć I2C\n");
    printf("   -włączyć SPI\n");
    printf("   -włączyć SSH\n");
    printf("   -ustawić lan i stałe IP, np:192.168.2.201\n");
    printf("1. Wgrać na /hom/pi/lambda program: socket_server\n");
    printf("2. Wgrać na /hom/pi/lambda/config plik konfiguracji: config\n");
    printf("   Plik config zawira 6 tagów głównych <socketIP>, <socketPort>, <ADS0>, <ADS1>, <ADS2>, <PT1000>\n");
    printf("   <socketIP> ma być takie jak ustawiliśmy dla PI'a i zgodne z konfiguracją na PC\n");
    printf("   <socketPort> ma być zgodne z programem na PC, najlepiej: 5000\n");
    printf("   <PT1000> ma być takie jak w pliku wzorcowym, nie zmieniać\n");
    printf("   <ADS?>:\n");
    printf("     <ScanMode> ma być: 0x08\n");
    printf("     <Gain> ma być: 1 (dopóki nie zmienimy płytek przyłączeniowych)\n");
    printf("     <DataRate> ma być: 0x33 (to można jeszcze dobrać doświadczalnie)\n");
    printf("     <Vref> jeśli stosujemy płytkę Vref,ma być: 4.096\n");
    printf("            bez płytki należy zmierzyć napięcie pomiędzy VCC a GND i wpisac tu zmierzoną wartość\n");
    printf("            powinno wyjść około 3.300, jeśli będzie około 5V, to znaczy, że mamy źle ustawione zworki Vref,VCC na ADS'ie\n");
    printf("     <R> ma być 1.0 (dopóki nie zmienimy płytek przyłączeniowych z rezystorami 1kOhm)\n");
    printf("     <T0> zależnie od ADS: dla ADS0 ma być: 0 dla ADS1 i ADS2 ma być 10 \n");
    printf("     <T1> zależnie od ADS: dla ADS0 ma być: 10 dla ADS1 i ADS2 ma być 20 \n");
    printf("     ..pozostałe parametry konfiguracji obliczą się same podczas kalibracji \n");
    printf("3. Założyć katalog: /var/log/socket_server\n");
    printf("   Zmienić uprawnienia dla tego katlogu - zapisywanie dla wszystkich\n");
    printf("4. Uruchomić kalibrację \n");
    printf("   a. włożyć płytki kalibracyjne zamiast płytek przyłączeniowych\n");
    printf("   b. uruchomić socket_server z parametrem kalibracja:\n");
    printf("      sudo /home/pi/lambda/socket_server kalibracja\n");
    printf("   c. odczekać aż program wyświetli komunikat: Koniec kalibracji\n");
    printf("   d. zakończyć program przez: ctrl+C \n");
    printf("   e. wyjąć płytki kalibracyjne i włożyć płytki przyłączeniowe\n");
    printf("5. Sprawdzić czy wszystko działa: /home/pi/lambda/socket_server ADS1 4 4 10\n");
    printf("   program będzie wyświetlał odczyty z podanego ADS'a i kanału \n");
    printf("   zakończyć  program przez ctrl+C i sprawdzić czy zrobił sie plik w katalogu /var/log/socket_server \n");
    printf("6. Dodać socket_server do pliku /etc/rc.local\n");
    printf("   Przed przed linijką: exit 0\n");
    printf("   należy wstawić linijki:\n");
    printf("   cd /home/pi/lambda\n");
    printf("   sudo ./socket_server \n");
    printf("7. Zrebootować PI'a \n");
    printf("8. sprawdzić czy jest komunikacja z poziomu PC\n");

}
//------------------------------------------------------------------
void dispPodsumowanie(){
  if(logLevel>=1){
    string s="Stop.\n";
    s+="Socket Frames: "                  +to_string(socketFrames)+"\n";
    s+="Socket CRC errors: "              +to_string(errCRC)+"\n";
    s+="Socket ID Device errors: "        +to_string(errDev)+"\n";
    s+="Socket PI Frames:        "        +to_string(devPI)+"/"+to_string(errPI)+"\n";
    s+="Socket X3Motor:          "        +to_string(errX3Motor)+"/"+to_string(devX3Motor)+"\n";
    s+="Socket X3PWM:            "        +to_string(errX3PWM)+"/"+to_string(devX3PWM)+"\n";
    s+="Socket X3 Adress Device Error: "  +to_string(errX3)+"\n";
    s+="Socket ADS_0 waiting:    "        +to_string(errADS_0)+"/"+to_string(devADS_0)+"\n";
    s+="Socket ADS_1 waiting:    "        +to_string(errADS_1)+"/"+to_string(devADS_1)+"\n";
    s+="Socket ADS_2 waiting:    "        +to_string(errADS_2)+"/"+to_string(devADS_2)+"\n";
    s+="Socket ADS Address Device Error: "+to_string(errADS)+"\n";
    s+="\n";
    s+="Not ready ADS_0: "+to_string(ErrorsADS_0)+"\n";
    s+="Not ready ADS_1: "+to_string(ErrorsADS_1)+"\n";
    s+="Not ready ADS_2: "+to_string(ErrorsADS_2)+"\n";
    s+="Errors X3Motor: " +to_string(ErrorsX3Motor)+"\n";
    s+="Errors X3PWM: "   +to_string(ErrorsX3PWM)+"\n";
    s+="Loop Index: "     +to_string(loopIndex)+"\n";
    Rap(s);
  }
}
//-----------------------------------------------------------
void dispADS_Reg(atSPI_ADS1256 *ads) {
  if(logLevel>=1){
    uint32_t ofc0=ads->readReg(REG_OFC0);
    uint32_t ofc1=ads->readReg(REG_OFC1);
    uint32_t ofc2=ads->readReg(REG_OFC2);
    int32_t ofc = ofc0<<0 | ofc1<<8 | ofc2<<16;
    if((ofc2>7)) ofc =ofc | (0xFF<<24);

    uint32_t fsc0=ads->readReg(REG_FSC0);
    uint32_t fsc1=ads->readReg(REG_FSC1);
    uint32_t fsc2=ads->readReg(REG_FSC2);
    int32_t fsc = fsc0<<0 | fsc1<<8 | fsc2<<16;

    string sAdcon="";
    uint8_t adcon=ads->readReg(REG_ADCON);
    adcon= adcon & 0b00000111;
    if(adcon==1) sAdcon="2";
    else if(adcon==2) sAdcon="4";
    else if(adcon==3) sAdcon="8";
    else if(adcon==4) sAdcon="16";
    else if(adcon==5) sAdcon="32";
    else if(adcon==4) sAdcon="64";

    string s=" Rejestry "+ads->getName()+"("+to_string(ads->getId())+")("+to_string(ads->getRegStatus())+")";
    s+="\n stat: "+charToBit(ads->readReg(REG_STATUS));
    s+="\n  mux: "+charToBit(ads->readReg(REG_MUX));
    s+="\nadcon: "+charToBit(adcon)+"     --"+sAdcon;
    s+="\ndrate: "+charToBit(ads->readReg(REG_DRATE));
    s+="\n   io: "+charToBit(ads->readReg(REG_IO));
    s+="\n  ofc: "+int32ToBit(ofc) +" "+ to_string(ofc) ;
    s+="\n  fsc: "+int32ToBit(fsc) +" "+ to_string(fsc) ;
    s+="\n";
    Rap( s);
  }
}
//--------------------------------
void  Handler(int signo){
    dispPodsumowanie();
    delete atSS;
    delete dbrSS;
    delete dbwSS;
    delete ati2c;
    delete X3Motor;
    delete spi;
    delete ads1256_0;
    delete ads1256_1;
    delete ads1256_2;
    exit(0);
}

//-------------------------------------------------------------------------------
void dispADS(atSPI_ADS1256 *ads){
    if(logLevel>=5 ){
        if(ads->getName()!=logDev) return;
        for(int i=logCH0; i<=logCH1;i++){
            Rap(
                ads->getName()+";  " +
                to_string(i)+"; ----   " +
                to_string(ads->getADC_N(i))+";  "+
                to_string(ads->getADC_valueSr(i))+";  U:"+
                to_string(ads->getADC_U_uV(i))+";  LU:"+
                to_string(ads->getADC_LU_uV(i))+";  R:"+
                to_string(ads->getADC_R_KOhm(i))+";  RL:"+
                to_string(ads->getADC_LR_KOhm(i))+";  T:"+
                to_string(ads->getADC_T_mC(i))+";  LT:"+
                to_string(ads->getADC_LT_mC(i))+";  "
            );
        }
    }
}
//-------------------------------------------------------------------------------
void dispX3( atI2C_dev * d ){
    if(d->getName()==logDev && logLevel>=5 ){
        Rap("X3: "+d->getDbw()->charToHex(  )+">> [<<"+d->getDbr()->charToHex(  )+"]");
    }
}

//--------------------------------------------------------------------------------
// return 1 oznacza błąd
int GoADS( atSPI_ADS1256 *ads ){
 if(ads->scan()){
     if(ads->getBuforyN()>0){
         ads->next();
         ads->setOffsetL(ads->getADC_buforSr(0));
         ads->setOffsetM(ads->getADC_buforSr(1)); //podstawienie jest na wszystkich ADS'ach, ale chwilowo korzystamy z tego tylko na ADS2
         dispADS(ads);
     }
     return 0;
 }
 return 1;
}

//--------------------------------------------------------------------------------
int GoPodsumowanie( ){
    time(&czas);
    czas_info=localtime(&czas);
    if(dzien!=czas_info->tm_mday){
        dzien=czas_info->tm_mday;
        dispPodsumowanie();
    }
    return 0;
}
//--------------------------------------------------------------------------------
int GoX3( atI2C_X3 * d ){
    if(d->getName()==logDev && logDev=="MOTOR"  && !d->SendIsHot()){
        atDataBuffer db=atDataBuffer( I2C_PI_ID, AT_SOCKET_FRAME_LEN,AT_MODBUS_REQUNAG);
        db.buforSetNag(AT_MODBUS_ID_X3,AT_MODBUS_READ,AT_REGADRES_MOTOR1,0,0);
        db.buforAddCRC();
        d->setSendFrom(&db);
    }

    if(!d->go()){
        if(logLevel>=5)  Rap("X3 Error: "+d->getDbw()->charToHex(  )+">> [<<"+d->getDbr()->charToHex(  )+"]");
        return 1;
    }
    dispX3( d );
    return 0;
}
//--------------------------------------------------------------------------------
int serverGoADS1256_mC( atSPI_ADS1256 *ads ){
    dbwSS->buforSetNag(dbrSS);  //to jest ramka Resp a więc powinna mieć długość nagłówka AT_MODBUS_RESPNAG
    for(int i=0; i<ads->getLen();i++){
        dbwSS->buforAddInt32(ads->getADC_LT_mC(i));
    }
    dbwSS->buforAddCRC();
    return 0;
}
//--------------------------------------------------------------------------------
int serverGoADS1256_uV( atSPI_ADS1256 *ads ){
    dbwSS->buforSetNag(dbrSS);  //to jest ramka Resp a więc powinna mieć długośćnagłówka AT_MODBUS_RESPNAG
    for(int i=0; i<ads->getLen();i++){
         dbwSS->buforAddInt32(ads->getADC_LU_uV(i));
    }
    dbwSS->buforAddCRC();
    return 0;
}
//--------------------------------------------------------------------------------
//ads2 ma wzmocnienie 2, może więc obsługiwać temperatury
//na pierwszych 6 kanałach idzie temperatura a na 6,7 mikrowolty z czujników
int serverGoADS1256_mC_uV( atSPI_ADS1256 *ads ){
    dbwSS->buforSetNag(dbrSS);  //to jest ramka Resp a więc powinna mieć długośćnagłówka AT_MODBUS_RESPNAG
    for(int i=0; i<4;i++){
         dbwSS->buforAddInt32(ads->getADC_MT_mC(i));
    }
    for(int i=6; i<ads->getLen();i++){
         dbwSS->buforAddInt32(ads->getADC_U_uV(i));
    }
    for(int i=6; i<ads->getLen();i++){
         dbwSS->buforAddInt32(ads->getADC_MU_uV(i));
    }
    dbwSS->buforAddCRC();
    return 0;
}
//--------------------------------------------------------------------------------
int serverGoPI(){
    dbwSS->buforSetNag(dbrSS);
    dbwSS->buforAddReg32(ErrorsADS_0);
    dbwSS->buforAddReg32(ErrorsADS_1);
    dbwSS->buforAddReg32(ErrorsADS_2);
    dbwSS->buforAddReg32(ErrorsX3Motor);
    dbwSS->buforAddReg32(ErrorsX3PWM);
    dbwSS->buforAddCRC();
    return 0;
}
//--------------------------------------------------------------------------------
//to jest funkcja obsługująca komunikację SOCKET, wołana jest ze środka jako ProcedData
//analizuje zawartość ramki dbr i ustawia wartośc dbw do wysłania
int serverGoFun( atDataBuffer *dbr, atDataBuffer *dbw ){

    socketFrames++;

    if( !dbr->checkCRC() ){ dbw->buforSetNag(AT_MODBUS_ID_PI,0x80,0,0, AT_ERROR_MODBUS_CRC ); dbw->buforAddCRC(); errCRC++; }
    else if(  dbr->checkId(AT_MODBUS_ID_PI) ) {  errPI+=serverGoPI();   devPI++;}
    else if(  dbr->checkId(AT_MODBUS_ID_X3)){
        if(      dbr->checkDeviceAddress(AT_REGADRES_MOTOR1) ) {
            devX3Motor++;
            X3Motor->setSendFrom(dbr);
            int err=!X3Motor->setReceInto(dbw); errX3Motor+=err;
        }
        else if( dbr->checkDeviceAddress(AT_REGADRES_PWM1) )   {
            devX3PWM++;
            X3PWM->setSendFrom(dbr);
            int err=!X3PWM->setReceInto(dbw); errX3PWM+=err;
        }
        else{dbw->setError( dbr, ATERROR_X3ADDRESSDEVICE ); errX3++;}
    }
    else if(  dbr->checkId(AT_MODBUS_ID_ADS1256) ){
        if(      dbr->checkDeviceAddress(0)){  errADS_0+=serverGoADS1256_mC(ads1256_0); devADS_0++;}
        else if( dbr->checkDeviceAddress(1)){  errADS_1+=serverGoADS1256_mC(ads1256_1); devADS_1++;}
        else if( dbr->checkDeviceAddress(2)){  errADS_2+=serverGoADS1256_mC_uV(ads1256_2); devADS_2++;}
        else{dbw->setError( dbr, ATERROR_ADS1256ADDRESSDEVICE ); errADS++;}
    }
    else {  dbw->buforSetNag(AT_MODBUS_ID_PI,0x80,0,0, AT_ERROR_MODBUS_ID_DEVICE ); dbw->buforAddCRC();   errDev++; }

    if( dbr->getDataLen() !=0 ){
        if(logLevel>=4)Rap("Socket: ["+dbr->charToHex(  )+">>]  <<"+dbw->charToHex(  )+"\n");
        dbr->Clear();
     }

    return 0;

}
//--------------------------------------------------------------
void ResetErrors(){
    devADS_0=0;devADS_1=0;devADS_2=0;devX3Motor=0;devX3PWM=0;;devPI=0;devError=0;
    errDev=0;errADS=0;errADS_0=0;errADS_1=0;errADS_2=0;errX3Motor=0;errX3PWM=0;errPI=0;errCRC=0,errX3=0;
    ErrorsADS_0=0;ErrorsADS_1=0;ErrorsADS_2=0; ErrorsX3Motor=0;ErrorsX3PWM=0;
    socketFrames=0;loopIndex=0;
}
//-------------------------------------------------------------------------
void initSocket(){
    dbrSS = new atDataBuffer( AT_MODBUS_ID_PI, AT_SOCKET_FRAME_LEN ,AT_MODBUS_RESPNAG);
    dbwSS = new atDataBuffer( AT_MODBUS_ID_PI, AT_SOCKET_FRAME_LEN ,AT_MODBUS_REQUNAG);
    atSS  = new atSocketServer( dbrSS, dbwSS, &serverGoFun,AT_SOCKET_PORTNO );
}
//-------------------------------------------------------------------
int initX3(){
    ati2c  = new atI2C(I2C_PI_ID);
    ati2c->confRap(FileRap);

    X3Motor   = new atI2C_X3("MOTOR",AT_MODBUS_ID_X3,AT_REGADRES_MOTOR1);
    X3Motor->confRap(FileRap);
    X3Motor->SetFrameLen(AT_MOTOR_OUTBUFLEN);
    X3PWM     = new atI2C_X3("PWM",AT_MODBUS_ID_X3,AT_REGADRES_PWM1);
    X3PWM->confRap(FileRap);
    X3PWM->SetFrameLen(AT_PWM_OUTBUFLEN);
    ati2c->addDev( (atI2C_dev*)X3Motor );
    ati2c->addDev( (atI2C_dev*)X3PWM );
    ati2c->Init();

    if(ati2c->IsError()){ Rap( ati2c->getErrorOpis() );  return 1;  }
    return 0;
}
//-------------------------------------------------------------------
int initADS(){

    spi = new atSPI(ADS1256_DEV00);
    spi->confRap(FileRap,logLevel);

    ads1256_0  = new atSPI_ADS1256( ADS1256_PINCE_15 ,"ADS0");
    ads1256_1  = new atSPI_ADS1256( ADS1256_PINCE_16 ,"ADS1");
    ads1256_2  = new atSPI_ADS1256( ADS1256_PINCE_18 ,"ADS2");
    spi->addDev( (atSPI_dev*)ads1256_0 );
    spi->addDev( (atSPI_dev*)ads1256_1 );
    spi->addDev( (atSPI_dev*)ads1256_2 );


    ads1256_0->setParams(cnf);
    ads1256_1->setParams(cnf);
    ads1256_2->setParams(cnf);


    spi->Init();

    if(spi->getError()!=0){ Rap( spi->getErrorOpis() );  return 1;  }
/*    if( spi->getError() ){
        string e="spi init Error: 0x"+IntToHex(spi->getError());
        if(ads1256_0->getError()) e+=" (ADS0: 0x"+IntToHex(ads1256_0->getError())+")";
        else if(ads1256_1->getError()) e+=" (ADS1: 0x"+IntToHex(ads1256_1->getError())+")";
        else if(ads1256_2->getError()) e+=" (ADS2: 0x"+IntToHex(ads1256_2->getError())+")";
        Rap(e);
        return 1;
    }*/

    dispADS_Reg(ads1256_0);
    dispADS_Reg(ads1256_1);
    dispADS_Reg(ads1256_2);

    return 0;
}
//-------------------------------------------------------------------------------------
void initVars(){
    logLevel=AT_LOG_ERRORS;
    time(&czas);
    czas_info=localtime(&czas);
    dzien=czas_info->tm_mday;
    FileRap= new atFileLog("/var/log/socket_server","socket_server",".log",logLevel);
    Rap("Socket_server, port:"+to_string(AT_SOCKET_PORTNO)+" Start.");
    cnf=new at_Konfiguracja("config/config");

    logCH0=0;logCH1=0;
    logDev="";
    kal=new kalibracja_ads(cnf);
    //--------------------------------------------------------

    ResetErrors();

}


//------------------------------------------------------------//
//                   M A I N                                  //
//------------------------------------------------------------//
int main(int argc, char *argv[]) {

    delay_ns(1000000000);
    signal(SIGINT, Handler);
    initVars();
    //--------------------------------------------------------
    if (argc>1 ){
        string arg=argv[1];
        if(arg=="ADS0" || arg=="ADS1" || arg=="ADS2"|| arg=="MOTOR"|| arg=="PWM"|| arg=="debug"){  logLevel=5; logDev=arg;  }
        else if(arg=="kalibracja") { logLevel=1; kal->start(); Rap("Kalibracja ADS1256");  cout <<"\n\nProszę włożyć płytki kalibracyjne.\n\n\n"; }
        else{ dispHelp();  return 0;  }
    }
    if (argc>2 ){  logCH0=std::atoi(argv[2]); logCH1=logCH0;  logLevel=1;    }
    if (argc>3 ){  logCH1=std::atoi(argv[3]); logLevel=1;   }
    if (argc>4 ){  logLevel=std::atoi(argv[4]);    }
    Rap("logLevel..:"+to_string(logLevel));
    //-------------------------------------------------------------------
    initSocket();
    if(initADS() && logDev=="") return 1;
    if(initX3() && logDev=="") return 1;

    kal->setDevs(ads1256_0,ads1256_1,ads1256_2);
    //--------------------------------------------------------------------
    while(1)    {
        GoPodsumowanie();
        atSS->go();
        ErrorsX3Motor+=GoX3( X3Motor );
        ErrorsX3PWM  +=GoX3( X3PWM );
        ErrorsADS_0  +=GoADS( ads1256_0 );
        ErrorsADS_1  +=GoADS( ads1256_1 );
        ErrorsADS_2  +=GoADS( ads1256_2 );
        if(kal->go()) break;
        loopIndex++;
    }
    delete FileRap;
    //pozostałe delete'y
    return 0;
}
