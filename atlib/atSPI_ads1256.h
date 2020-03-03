/*at test*/

#ifndef ATSPI_ADS1256_H_
#define ATSPI_ADS1256_H_


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <atSPI_dev.h>
#include <atFunctions.h>
#include <atErrors.h>
#include <at_Konfiguracja.h>
#include <atStoper.h>

//#include <sstream>
using namespace std;


//---------------------------------------------------------------

#define SPI_DELAY_READREG    2000                //nanosekundy!
#define SPI_DELAY_CFG     1000000                //=1ms
#define SPI_DELAY_READDATA  10000
#define SPI_DELAY_WRITECMD   5000
#define SPI_DELAY_READCHANN 25000

/*
#define SPI_DELAY_READREG   1000000                //nanosekundy!
#define SPI_DELAY_CFG       1000000                //=1ms
#define SPI_DELAY_READDATA  1000000
#define SPI_DELAY_WRITECMD  1000000
#define SPI_DELAY_READCHANN 1000000
*/
#define ADS1256_DEV00 "/dev/spidev0.0"
#define ADS1256_DEV01 "/dev/spidev0.1"
#define ADS1256_DEV10 "/dev/spidev1.0"

#define ADS1256_PINCE_15 22 //RPI_BPLUS_GPIO_J8_15  //GPIO 22, pin:15
#define ADS1256_PINCE_16 23 //RPI_BPLUS_GPIO_J8_16  //GPIO 23, pin:16
#define ADS1256_PINCE_18 24 //RPI_BPLUS_GPIO_J8_18  //GPIO 24, pin:18

//#define ADS1256_DZIELNIK 1670000
#define ADS1256_DZIELNIK      0x7fffff
#define ADS1256_DZIELNIK_uV   8.388607  //to jest: ADS1256_DZIELNIK/1000000
#define ADS1256_MAXBUFFER     8388607   //maksymalna wartośc bufora
#define ADS1256_MAXGND2       20000     //dopuszczale wskazanie GND przy wzmocnieniu 2
#define ADS1256_VREF_INTERNAL 5.0
// charakterystyka PT100
//#define PT1000_R1 1.0779
//#define PT1000_T1 20.0
//#define PT1000_R2 1.1554
//#define PT1000_T2 40.0

#define PT1000_r20 0.9213
#define PT1000_r10 0.9607
#define PT1000_R0  1.0000
#define PT1000_R10 1.0390
#define PT1000_R20 1.0779
#define PT1000_R30 1.1167
#define PT1000_R40 1.1554

#define PT1000_t20 -20.
#define PT1000_t10 -10.
#define PT1000_T0  0.
#define PT1000_T10 10.
#define PT1000_T20 20.
#define PT1000_T30 30.
#define PT1000_T40 40.

//----------------------------------------------------------------------
typedef enum {
	ADS1256_GAIN_1			= (0),	/* GAIN   1 */
	ADS1256_GAIN_2			= (1),	/*GAIN   2 */
	ADS1256_GAIN_4			= (2),	/*GAIN   4 */
	ADS1256_GAIN_8			= (3),	/*GAIN   8 */
	ADS1256_GAIN_16			= (4),	/* GAIN  16 */
	ADS1256_GAIN_32			= (5),	/*GAIN    32 */
	ADS1256_GAIN_64			= (6),	/*GAIN    64 */
} ADS1256_GAIN_E;
//----------------------------------------------------------------------
typedef enum {
    ADS1256_30000SPS = 0xF0,		   //11110000 = 30,000SPS (default)
    ADS1256_15000SPS = 0xE0,           //11100000 = 15,000SPS
    ADS1256_7500SPS  = 0xD0,           //11010000 = 7,500SPS
    ADS1256_3750SPS  = 0xC0,           //11000000 = 3,750SPS
    ADS1256_2000SPS  = 0xB0,           //10110000 = 2,000SPS
    ADS1256_1000SPS  = 0xA1,           //10100001 = 1,000SPS
    ADS1256_500SPS   = 0x92,           //10010010 = 500SPS
    ADS1256_100SPS   = 0x82,           //10000010 = 100SPS
    ADS1256_60SPS    = 0x72,           //01110010 = 60SPS
    ADS1256_50SPS    = 0x63,           //01100011 = 50SPS
    ADS1256_30SPS    = 0x53,           //01010011 = 30SPS
    ADS1256_25SPS    = 0x43,           //01000011 = 25SPS
    ADS1256_15SPS    = 0x33,           //00110011 = 15SPS
    ADS1256_10SPS    = 0x20,           //00100011 = 10SPS
    ADS1256_5SPS     = 0x13,           //00010011 = 5SPS
    ADS1256_2d5SPS   = 0x03            //00000011 = 2.5SPS
} ADS1256_DRATE_E;
//--------------------------------------------------------------------
enum {
    REG_STATUS = 0,	 // x1H
    REG_MUX    = 1,  // 01H
    REG_ADCON  = 2,  // 20H
    REG_DRATE  = 3,  // F0H
    REG_IO     = 4,  // E0H
    REG_OFC0   = 5,  // xxH
    REG_OFC1   = 6,  // xxH
    REG_OFC2   = 7,  // xxH
    REG_FSC0   = 8,  // xxH
    REG_FSC1   = 9,  // xxH
	REG_FSC2   = 10, // xxH
};
//------------------------------------------------------------------------
enum {
        CMD_WAKEUP  = 0x00, // Completes SYNC and Exits Standby Mode 0000  0000 (00h)
        CMD_RDATA   = 0x01, // Read Data 0000  0001 (01h)
        CMD_RDATAC  = 0x03, // Read Data Continuously 0000   0011 (03h)
        CMD_SDATAC  = 0x0F, // Stop Read Data Continuously 0000   1111 (0Fh)
        CMD_RREG    = 0x10, // Read from REG rrr 0001 rrrr (1xh)
        CMD_WREG    = 0x50, // Write to REG rrr 0101 rrrr (5xh)
        CMD_SELFCAL = 0xF0, // Offset and Gain Self-Calibration 1111    0000 (F0h)
        CMD_SELFOCAL= 0xF1, // Offset Self-Calibration 1111    0001 (F1h)
        CMD_SELFGCAL= 0xF2, // Gain Self-Calibration 1111    0010 (F2h)
        CMD_SYSOCAL = 0xF3, // System Offset Calibration 1111   0011 (F3h)
        CMD_SYSGCAL = 0xF4, // System Gain Calibration 1111    0100 (F4h)
        CMD_SYNC    = 0xFC, // Synchronize the A/D Conversion 1111   1100 (FCh)
        CMD_STANDBY = 0xFD, // Begin Standby Mode 1111   1101 (FDh)
        CMD_RESET   = 0xFE, // Reset to Power-Up Values 1111   1110 (FEh)
};
//------------------------------------------------------------------------
typedef enum {
        SCANMODE_DIFFERENTIAL  = 0x04,
        SCANMODE_UNIPOLAR      = 0x08,
        SCANMODE_NEGATIVE      = 0x18
} ADS1256_SCANMODE;

//------------------------------------------------------------------------
class atBufor {
private:
    int32_t value;     //aktualna wartość
    int32_t valueSr;   //ostatnia średnia
    int64_t sum;       //suma aktalna
    uint16_t N;        //ilośc pomiarów w sumie
    uint16_t not_drdy; //ilośc przedwczesnych prób odczytu
    uint8_t nMax;      //zabezpieczenie przed przepełnieniem (maksymalne N)

    int Gain;             //wzmocnienie       (int)
    double vRef;          //napięcie referencyjne             [V]
    double R;             //opornik dzielnika                 [KOhm]
    double aPT1000;       //nachylenie charakterystyki PT1000 [KOhm/C]
    double T0,R0;         //punkt charakterystyki             [C,kOhm]
    
    int32_t Offset_m,Offset_p,Offset_l;        //wewnętrzne procedury kalibracji nie działają tak jak sie spodziewam. dlatego robię własne
                                               //na tych zmiennych są odczyty zebrane przy
                                               //m- zwarciu do masy
                                               //p- zwarciu do Vref
                                               //l- z dzielnika 1/2 czyli przy napięciu Vref/2
    int32_t Offset_L; //jeszcze jedno obejście. Do dynamicznego wyliczania offsetu z dzielnika ustawionego na jednym z kanałów
    int32_t Offset_M; //na kolejnym kanale dynamiczne zero
public:
    atBufor(uint8_t m){reset(); nMax=m; Offset_m=0;Offset_p=ADS1256_DZIELNIK; Offset_l=ADS1256_DZIELNIK/2;}
    atBufor(){reset(); nMax=100; Offset_m=0;Offset_p=ADS1256_DZIELNIK; Offset_l=ADS1256_DZIELNIK/2;}
    void reset(){value=0;valueSr=0;sum=0;N=0;not_drdy=0;}
    void add(int32_t v){value=v; sum+=v; N++; if(N>nMax)next();}
    void NDrdy(){not_drdy++;}
    void next(){if(N){valueSr=sum/N; sum=0; N=0; not_drdy=0; }}

    void setParams(double v, double r,double a,double t0,double r0){vRef=v;R=r;aPT1000=a;T0=t0;R0=r0;}
    void setGain(ADS1256_GAIN_E g){if(g==1){Gain=2;}else if(g==2){Gain=4;}else if(g==3){Gain=8;}else if(g==4){Gain=16;}else if(g==5){Gain=32;}else if(g==6){Gain=64;} else{Gain=1;}}
    void setVref(double v){vRef=v;}
    void setOffsets(int32_t m,int32_t p,int32_t l){Offset_m=m,Offset_p=p,Offset_l=l;}
    void setOffsetL(int32_t l){Offset_L=l;}
    void setOffsetM(int32_t l){Offset_M=l;}

    int32_t getBufor()  {return value;}
    int32_t getBuforSr(){return valueSr;}
    int32_t getSum()    {return sum;}
    int32_t getValue()  {return value-Offset_m;}
    int32_t getValueSr(){return valueSr-Offset_m;}
    uint16_t getN()     {return N;}
    uint16_t getNDrdy() {return not_drdy;}
    int32_t getOffsetL(){return Offset_L;}
    int32_t getOffsetM(){return Offset_M;}

    double getUref_V()  {return vRef;}
    double getPT100_a() {return aPT1000;}

    //tu zmienić ..według naszej kalibracji
    //double getU_V()     {return ((double)getValueSr())*ADS1256_VREF_INTERNAL/(Gain*ADS1256_DZIELNIK);}
    //double getU_uV()    {return ((double)getValueSr())*ADS1256_VREF_INTERNAL/(Gain*ADS1256_DZIELNIK_uV);}
    //double getR_KOhm()  {return (vRef/getU_V())*R - R;}
    //double getT_C()     {return T0 + (getR_KOhm()-R0)/aPT1000;}
    //int32_t getT_mC()   {return (int32_t)(getT_C()*1000);}

    double getBuf()     {return ((double)getValueSr())/((Offset_l-Offset_m)*2);} //*2 jest dlatgo że Offset_l to napięcie z dzielnika, pełne napięcie jest 2 razy wieksze
    double getU_V()     {return vRef*getBuf();}
    double getR_KOhm()  {return (R/getBuf()) - R; }
    double getT_C()     {return T0 + (getR_KOhm()-R0)/aPT1000;}
    int32_t getU_uV()   {return (int32_t)(getU_V()*1000000);}
    int32_t getU_mV()   {return (int32_t)(getU_V()*1000);}
    int32_t getT_mC()   {return (int32_t)(getT_C()*1000);}

    //zestaw alternatywnych funkcji do dynamicznego offsetu
    double getLBuf()     {return ((double)getValueSr())/((Offset_L-Offset_m)*2);} //*2 jest dlatgo że Offset_l to napięcie z dzielnika, pełne napięcie jest 2 razy wieksze
    double getLU_V()     {return vRef*getLBuf();}
    double getLR_KOhm()  {return (R/getLBuf()) - R; }
    double getLT_C()     {return T0 + (getLR_KOhm()-R0)/aPT1000;}
    int32_t getLU_uV()   {return (int32_t)(getLU_V()*1000000);}
    int32_t getLU_mV()   {return (int32_t)(getLU_V()*1000);}
    int32_t getLT_mC()   {return (int32_t)(getLT_C()*1000);}

    //zestaw alternatywnych funkcji do dynamicznego offsetu
    int32_t getMValueSr(){return valueSr-Offset_M;}
    double getMBuf()     {return ((double)getMValueSr())/((Offset_L-Offset_M)*2);} //*2 jest dlatgo że Offset_l to napięcie z dzielnika, pełne napięcie jest 2 razy wieksze
    double getMU_V()     {return vRef*getMBuf();}
    double getMR_KOhm()  {return (R/getMBuf()) - R; }
    double getMT_C()     {return T0 + (getMR_KOhm()-R0)/aPT1000;}
    int32_t getMU_uV()   {return (int32_t)(getMU_V()*1000000);}
    int32_t getMT_mC()   {return (int32_t)(getMT_C()*1000);}



};

//-------------------------------------------------------------------
class atSPI_ADS1256 : public atSPI_dev{
private:
        ADS1256_GAIN_E  Gain;                       /* GAIN  */
        ADS1256_DRATE_E DataRate;                   /* DATA output  speed*/
        uint8_t id;                                 /* Id urzadzenia sprzętowe - odczytane*/
        uint8_t len;                                /* Ilość kanalow*/
        uint8_t ScanMode;                           /* ScanMode, (8=single, 4=dual, 0x18=single-negative).*/
        atBufor bufor[SCANMODE_UNIPOLAR];
        uint8_t Channel;                              /* The current channel*/

        uint8_t reg_status;

public:
        atSPI_ADS1256( uint8_t p, string N);

        void Config( ADS1256_SCANMODE _ucScanMode, ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate );
        bool Init(void);

        void writeReg( uint8_t reg,uint8_t val );
        void writeCmd( uint8_t _cmd );
        void writeCnf();
        //uint8_t getChannel( ){return Channel;}
        uint8_t readReg( uint8_t reg );
        int32_t readData(void);

        bool DRDY();
        void setChannel(uint8_t _ch);
        uint8_t scan(void);

        void initGain(ADS1256_GAIN_E g)     {Gain=g; writeCnf(); for(int ch=0;ch<len;ch++)bufor[ch].setGain(Gain);}
        void initDataRate(ADS1256_DRATE_E d){DataRate=d; writeCnf();}
        ADS1256_GAIN_E readGain()                      {return (ADS1256_GAIN_E)(readReg(REG_ADCON) & 0b00000111);}
        ADS1256_DRATE_E readDataRate()                  {return (ADS1256_DRATE_E)(readReg(REG_DRATE));}
        ADS1256_GAIN_E getGain(){return Gain;}
        ADS1256_DRATE_E getDataRate(){return DataRate;}
        bool GainOK(){return readGain()==Gain;}
        bool DataRateOK(){return readDataRate()==DataRate;}
        //------------------------------------------------------------------------
        void reset(){  for(uint8_t i=0; i<len; i++){ bufor[i].reset();}  }
        void next(){  for(uint8_t i=0; i<len; i++){ bufor[i].next();}  }

        uint8_t  getId()                   {return id;}
        uint8_t  getRegStatus()            {return reg_status;}
        uint8_t  getLen()                  {return len;}
        atBufor  getADC_bufor_(uint8_t ch) {if(ch>=len){return 0;} return bufor[ch];}
        int32_t  getADC_bufor(uint8_t ch)  {if(ch>=len){return 0;} return bufor[ch].getBufor();}
        int32_t  getADC_buforSr(uint8_t ch){if(ch>=len){return 0;} return bufor[ch].getBuforSr();}
        int32_t  getADC_value(uint8_t ch)  {if(ch>=len){return 0;} return bufor[ch].getValue();}
        int32_t  getADC_valueSr(uint8_t ch){if(ch>=len){return 0;} return bufor[ch].getValueSr();}
        int32_t  getADC_Sum(uint8_t ch)    {if(ch>=len){return 0;} return bufor[ch].getSum();}
        uint16_t getADC_N(uint8_t ch)      {if(ch>=len){return 0;} return bufor[ch].getN();}
        uint16_t getADC_NDrdy(uint8_t ch)  {if(ch>=len){return 0;} return bufor[ch].getNDrdy();}
        uint16_t getBuforyN()              {uint16_t n=0xffff; for(uint8_t i=0; i<len; i++){ if(bufor[i].getN()<n) n=bufor[i].getN();} return n;}
        uint16_t getBuforyNSum( )          {uint16_t S=0; for(uint8_t i=0; i<len; i++){S+=bufor[i].getN();} return S;}
        int32_t  getBuforyU_uVSum( )       {double S=0; for(uint8_t i=0; i<len; i++){S+=bufor[i].getU_uV();} return S;}

        double getADC_U_V(uint8_t ch)      {if(ch>=len){return 0;} return bufor[ch].getU_V();}
        double getADC_R_KOhm(uint8_t ch)   {if(ch>=len){return 0;} return bufor[ch].getR_KOhm();}
        double getADC_T_C(uint8_t ch)      {if(ch>=len){return 0;} return bufor[ch].getT_C();}
        int32_t getADC_U_uV(uint8_t ch)    {if(ch>=len){return 0;} return bufor[ch].getU_uV();}
        int32_t getADC_T_mC(uint8_t ch)    {if(ch>=len){return 0;} return bufor[ch].getT_mC();}

        double getADC_LU_V(uint8_t ch)      {if(ch>=len){return 0;} return bufor[ch].getLU_V();}
        double getADC_LR_KOhm(uint8_t ch)   {if(ch>=len){return 0;} return bufor[ch].getLR_KOhm();}
        double getADC_LT_C(uint8_t ch)      {if(ch>=len){return 0;} return bufor[ch].getLT_C();}
        int32_t getADC_LU_uV(uint8_t ch)    {if(ch>=len){return 0;} return bufor[ch].getLU_uV();}
        int32_t getADC_LU_mV(uint8_t ch)    {if(ch>=len){return 0;} return bufor[ch].getLU_mV();}
        int32_t getADC_LT_mC(uint8_t ch)    {if(ch>=len){return 0;} return bufor[ch].getLT_mC();}

        double getADC_MU_V(uint8_t ch)      {if(ch>=len){return 0;} return bufor[ch].getMU_V();}
        double getADC_MR_KOhm(uint8_t ch)   {if(ch>=len){return 0;} return bufor[ch].getMR_KOhm();}
        double getADC_MT_C(uint8_t ch)      {if(ch>=len){return 0;} return bufor[ch].getMT_C();}
        int32_t getADC_MU_uV(uint8_t ch)    {if(ch>=len){return 0;} return bufor[ch].getMU_uV();}
        int32_t getADC_MT_mC(uint8_t ch)    {if(ch>=len){return 0;} return bufor[ch].getMT_mC();}

        double getADC_Uref_V(uint8_t ch)   {if(ch>=len){return 0;} return bufor[ch].getUref_V();}
        double getADC_PT100_a(uint8_t ch)  {if(ch>=len){return 0;} return bufor[ch].getPT100_a();}

        void setParams(uint8_t ch,double v, double r,double a,double t0,double r0){if(ch<len){bufor[ch].setParams(v,r,a,t0,r0);}}
        void setVref(double v)                          {for(int i=0;i<len;i++)bufor[i].setVref(v);}
        void setOffsets(int32_t m,int32_t p,int32_t l)  {for(int i=0;i<len;i++)bufor[i].setOffsets(m,p,l);      }
        void setOffsetL(int32_t l)                      {for(int i=0;i<len;i++)bufor[i].setOffsetL(l);      }
        void setOffsetM(int32_t l)                      {for(int i=0;i<len;i++)bufor[i].setOffsetM(l);      }
        int setParams(at_Konfiguracja* cnf);
        string OpisRejestrow();
} ;



#endif
