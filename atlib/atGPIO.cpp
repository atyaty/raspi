#include "atGPIO.h"

//-------------------------------------------------
    atGPIO::atGPIO(){}
//-------------------------------------------------
atGPIO::~atGPIO(){  }
//-------------------------------------------------
bool atGPIO::Init() {
    if( !bcm2835_init() )    { Error.SetError(ATERROR_GPIO_OPEN); Rap("Błąd: "+Error.Opis(),AT_LOG_ERRORS );  return false; }
}
//-------------------------------------------------
void atGPIO::SetOutput(uint8_t gpio) {
    bcm2835_gpio_fsel( gpio, BCM2835_GPIO_FSEL_OUTP);
}
//-------------------------------------------------
void atGPIO::SetInput(uint8_t gpio) {
    bcm2835_gpio_fsel( gpio, BCM2835_GPIO_FSEL_INPT);
}
//-------------------------------------------------
void atGPIO::writeGPIO(uint8_t gpio,bool v) {
    bcm2835_gpio_write(gpio,v);

}
//-------------------------------------------------
//void atGPIO::delay_ns(long int timeout){
//    struct timespec t,t0;
//    clock_gettime(CLOCK_REALTIME,&t0);
//    long int n,i=0;
//    while ( true  )   {
//        clock_gettime(CLOCK_REALTIME,&t);
//        n = t.tv_nsec-t0.tv_nsec+(t.tv_sec-t0.tv_sec)*1000000000;
//        i++;
//        if(n>timeout) break;
//    }
//}



