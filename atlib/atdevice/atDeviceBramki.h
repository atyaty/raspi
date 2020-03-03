/*
by AT
*/

#ifndef atDeviceBramkiH
#define atDeviceBramkiH

#include "atDevice.h"
#include <QtGlobal>


//----------------------------------------------------------
//
class atDeviceOnOff : public atDevice {
 public:
  atDeviceOnOff(QString s, QString id):atDevice(s,id){}
  double GoFun();
  void SetFun(int i);
};


//----------------------------------------------------------
//bramka AND
class atDeviceAND : public atDevice {
 public:
    atDeviceAND(int n,QString s,QString id):atDevice(n,s,id){}
  double GoFun();
};
//----------------------------------------------------------
// bramka OR
class atDeviceOR : public atDevice {
 public:
  atDeviceOR(int n,QString s,QString id):atDevice(n,s,id){}
  double GoFun();
};
//----------------------------------------------------------
class atDeviceNOR : public atDevice {
 public:
  atDeviceNOR(int n,QString s,QString id):atDevice(n,s,id){}
  double GoFun();
};
//----------------------------------------------------------
class atDeviceNAND : public atDevice {
 public:
  atDeviceNAND(int n,QString s,QString id):atDevice(n,s,id){}
  double GoFun();
};
//----------------------------------------------------------
class atDeviceTrue : public atDevice {
 public:
  atDeviceTrue(QString s,QString id):atDevice(s,id) {Go();}
  double GoFun();
};
//----------------------------------------------------------
//Zawsze zwraca 1 ..zawsze
class atDeviceFalse : public atDevice {
 public:
  atDeviceFalse(QString s,QString id):atDevice(s,id) { Go(); }
  double GoFun();
};
//----------------------------------------------------------
//suma wejść jest wartościa wyjścia
class atDeviceSum : public atDevice {
 public:
    atDeviceSum(int n,QString s,QString id):atDevice(n,s,id){}
    double GoFun();
};
//----------------------------------------------------------
//średnia z wejść jest wartościa wyjścia
class atDeviceAvg : public atDeviceSum {
 public:
  atDeviceAvg(int n,QString s,QString id) : atDeviceSum(n,s,id) {}
  double GoFun(){return (aWe.size()<=0)?0:(atDeviceSum::GoFun()/aWe.size());}
};

//----------------------------------------------------------
//suma wejśc ze znakiem minus
class atDeviceInv : public atDevice {
 public:
  atDeviceInv(int n,QString s,QString id):atDevice(n,s,id){}
  double GoFun();
};
//----------------------------------------------------------
// różnica We0 i pozostałych
class atDeviceRoz : public atDevice {
 public:
  atDeviceRoz(int n,QString s,QString id):atDevice(n,s,id){}
  double GoFun();
};

//----------------------------------------------------------
//iloczyn wejść
class atDeviceMno : public atDevice {
 public:
  atDeviceMno(int n,QString s,QString id):atDevice(n,s,id)  {}
  double GoFun();
};
//----------------------------------------------------------
//odwrotnośc iloczynu wejsc
class atDeviceOdw : public atDevice {
 public:
  atDeviceOdw(int n,QString s,QString id):atDevice(n,s,id)  {}
  double GoFun();
};
//----------------------------------------------------------
// dzielenie We0 przez pozostałe
class atDeviceDzi : public atDevice {
 public:
  atDeviceDzi(int n,QString s,QString id):atDevice(n,s,id)  {}
  double GoFun();
};
//----------------------------------------------------------
// wartość bezwzględna
class atDeviceAbs : public atDevice {
 public:
  atDeviceAbs(QString s,QString id):atDevice(1,s,id)  {}
  double GoFun();
};



#endif
