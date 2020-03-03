#include "atSPI.h"


//-------------------------------------------------
atSPI::atSPI(string s){    sDev=s;    Error=0; FileRap=NULL; }
//-------------------------------------------------
atSPI::~atSPI(){    bcm2835_close();  }
//-------------------------------------------------
void atSPI::addDev(atSPI_dev* d){   devs.push_back(d);    d->SetupDev( this );     }
//-------------------------------------------------
bool atSPI::Init() {
    //--------------------------------------
     if( !bcm2835_init() )    { Error=ATERROR_SPI_OPEN;   return false; }
     if( !bcm2835_spi_begin()){ Error=ATERROR_SPI_BEGIN;  return false; }

     bcm2835_gpio_fsel( DEV_RST_PIN, BCM2835_GPIO_FSEL_OUTP);
     bcm2835_gpio_write(DEV_RST_PIN,1);
     bcm2835_delay(200);
     bcm2835_gpio_write(DEV_RST_PIN,0);
     bcm2835_delay(200);
     bcm2835_gpio_write(DEV_RST_PIN,1);
     delay_ns(1000000000); //po resecie
                                                                   //poniższe parametry wyglądają jakby miały byc dla spi_dev.. przenieść ewentualnie
     bcm2835_spi_setBitOrder( BCM2835_SPI_BIT_ORDER_MSBFIRST);     //High first transmission
     bcm2835_spi_setDataMode( BCM2835_SPI_MODE1);                  //spi mode 0
     bcm2835_spi_setClockDivider(ClockDivider);                    //Frequency
//     bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_2048);  //Frequency

     for(unsigned i=0; i<devs.size(); i++){
         devs.at(i)->Init();
         if(devs.at(i)->getError()){  Error=ATERROR_SPI_DEV_OPEN;    }
     }
     delay_ns(1000000000); //po zmianie konfiguracji poszczególnych ADS'ów
     return Error==0;
}
//-------------------------------------------------
void atSPI::go() {  for(unsigned i=0; i<devs.size(); i++){devs.at(i)->go();}  }
//-------------------------------------------------
void atSPI::go( unsigned i ) {  devs.at(i)->go();  }
//-----------------------------------------------------------
string atSPI::getErrorOpis(){
    string s="";
    if(Error) s+="SPI Init error: 0x"+IntToHex(Error);
    for(unsigned i=0; i<devs.size(); i++){
      if(devs.at(i)->getError()){  s+=" ("+devs.at(i)->getName()+": 0x"+IntToHex(devs.at(i)->getError())+")";    }
    }
    return s;
}
//--------------------------------------------------------------------------
int atSPI::setParams(at_Konfiguracja* cnf){
    uint16_t cd=cnf->getInt(sDev+".ClockDivider");
    if(cnf->getError()){ cd=2048;}
    Rap("SPI.ClockDivider: "+to_string(cd)+"\n",0);
    ClockDivider=(bcm2835SPIClockDivider)cd;
    return 0;
}




