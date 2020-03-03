#ifndef ATI2C_X3_H
#define ATI2C_X3_H

#include <atI2C_dev.h>
#include <at_X3.h>
#include <atErrors.h>

//-------------------------------------------------
class atI2C_X3 : public atI2C_dev {
private:
    uint8_t addrDev;  //bajt 2, adres urządzenia
    uint8_t idModbusDev;    // !! bajt 0, adres urządzenia, to powinien być ten sam parametr co idDev, wywalić ten parametr i używać idDev
    uint8_t frameModbusLen; // !! prawidłowa długość ramki powrotnej (wywalić jak nauczymy się odczytywać długoðć ramki z X3)
    bool sendIsHot;
    bool receIsHot;
public:
    atI2C_X3( string n, uint8_t d, uint8_t a);
    ~atI2C_X3(){delete dbr; delete dbw;}
    void SetFrameLen(uint8_t l){frameModbusLen=l;}
    bool Init();
    bool go();
    bool checkFrame(atDataBuffer* d );

   // bool checkReceId(){return dbr->checkId();}
   // bool checkReceDeviceAddress(){return dbr->checkDeviceAddress();}
    bool checkReceFrame(){return checkFrame(dbr);}
    bool checkSendFrame(){return checkFrame(dbw);}
    bool setSendFrom(atDataBuffer* d);//{dbw->setFrame(d); }
    bool setReceInto(atDataBuffer* d);//{d->setFrame(dbr);}
    bool SendIsHot(){ return sendIsHot; }
    void setSendIsHot(){  sendIsHot=true; }
    uint8_t getAddrDev(){return addrDev;}
};

#endif // ATI2C_X3_H
