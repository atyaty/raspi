#ifndef ATGPIO_H
#define ATGPIO_H

#include <iostream>
#include <fcntl.h>
#include <bcm2835.h>
#include <atError.h>
#include <atErrors.h>
#include <string>
#include <atFileLog.h>
#include <at_Konfiguracja.h>

#define DEV_RST_PIN     18 //uwaga powtórzone


//-------------------------------
using namespace std;
//---------------------------------------
class atGPIO {
private:
    //dorobić ewentualnie tablicę opisującą czy to jest We czy Wy : vector<uint8_t> gpios
    atError Error;                 //
    atFileLog *FileRap;          //
    void Rap(string s,int l){ if(FileRap!=nullptr ) FileRap->Rap(s,l); }
public:
    atGPIO();
    virtual~atGPIO();
    bool Init();
    void SetOutput(uint8_t gpio);
    void SetInput(uint8_t gpio);
    void writeGPIO(uint8_t gpio,bool value);
    bool readGPIO(uint8_t gpio);

    bool IsError(){ return !Error.IsEmpty(); }
    string getErrorOpis();
    void confRap(atFileLog *f){FileRap=f;}

};

#endif // ATGPIO_H
