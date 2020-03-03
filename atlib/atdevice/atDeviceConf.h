/*
by AT
*/

#ifndef atDeviceConfH
#define atDeviceConfH


#include "atDevice.h"
#include "atDeviceBasic.h"
#include "atKonfiguracja.h"
#include "atError.h"
#include "atCzas.h"


//----------------------------------------------------------
// prymitywny odczyt wielu wartości z konfiguracji
//
class atDeviceConf : public atDevice {
 protected:
  atKonfiguracja *CNF;
  QString Pole;
  int N,Len; //element aktualny,ilośc elementów
  double * war;
  QString * naz;
  void SetDevs(atKonfiguracja *cnf,QString pole);
  double nN,nLen; //do inputów
 public:
  atDeviceConf(atKonfiguracja *cnf,QString pole,QString s,QString id):atDevice(s,id){SetDevs(cnf,pole);}
  atDeviceConf(atKonfiguracja *cnf,QString s,QString id):atDevice(s,id)             {SetDevs(cnf,id);}
  ~atDeviceConf();
  double GoFun();
  void SetFun(int i);

};



#endif
