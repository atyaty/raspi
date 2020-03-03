/*
by AT
*/

#ifndef atDeviceBasicH
#define atDeviceBasicH


#include "atDevice.h"
#include "atError.h"


//----------------------------------------------------------
//powtarza na wyjściu wartość z we0 o ile we1!=0
class atDeviceSemafor : public atDevice {
 public:
  atDeviceSemafor(QString s,QString id):atDevice(2,s,id){}
  double GoFun();
};

//----------------------------------------------------------
//powtarza na wyjściu wartość z we*
//SetFun wskazuje z którego wejścia podawac wartośc
class atDeviceSem : public atDevice {
 private:
  int we_nr;
 public:
  atDeviceSem(int n,QString s,QString id):atDevice(n,s,id){we_nr=0;}
  double GoFun();
  void SetFun(int i);

};

//----------------------------------------------------------
class atDeviceTara : public atDevice {
 private:
  double tara;
 public:
  atDeviceTara(QString s,QString id);
  double GoFun();
  void SetFun(int i);
};



//----------------------------------------------------------
//na wyjsciu ustawiana jest wartośc zmiennej do której wskaźnik mamy w Buf
class atDeviceInput : public atDevice {
 private:
  double *Buf;
 public:
  atDeviceInput(double *b,QString s,QString id):atDevice(s,id){ Buf=b; Go(); }
  double GoFun() {return Enabled()?*Buf:0.;}
};
/*
//----------------------------------------------------------
class atDeviceCalc : public atDevice {
 private:
  atTrace *Tr;
 public:
  atDeviceCalc(QString tr,QString s,QString id);
  ~atDeviceCalc();
  double GoFun();
  atTrace *Trace(){return Tr;}
  void SetDev();
};

//----------------------------------------------------------
class atDeviceTTrace : public atDevice {
 private:
  atStoper *STO;
  atTrace *Tr;
  unsigned mtp; //mnożnik czasu
  double start;
  double period;
  bool en;  //enabled
  bool onePeriod;
  void SetDevs(atStoper *sto,QString tr,unsigned m,bool op);
 public:
  atDeviceTTrace(atStoper *sto,QString tr,unsigned m,bool op,QString s) {SetDevs(sto,tr,m,op);SetNazwa(s);}
  atDeviceTTrace(atStoper *sto,QString tr,unsigned m,QString s){SetDevs(sto,tr,m,false);SetNazwa(s);}
  atDeviceTTrace(atStoper *sto,QString tr,unsigned m)         {SetDevs(sto,tr,m,false);}
  atDeviceTTrace(atStoper *sto,QString tr,unsigned m,bool op) {SetDevs(sto,tr,m,op);}
  atDeviceTTrace(atStoper *sto,QString tr,QString s)           {SetDevs(sto,tr,1,false);SetNazwa(s);}
  atDeviceTTrace(atStoper *sto,QString tr)                    {SetDevs(sto,tr,1,false);}
  ~atDeviceTTrace(){delete Tr;}
  double GoFun();
  void ExtFun(int i);

};


//----------------------------------------------------------
//  nieprzetestowana...
class atDeviceMathSrednia : public atDevice {
 private:
  double sum,sred;
  int N;
  int Nmax;
  bool en,ready;
 public:
  atDeviceMathSrednia(int nmax,QString s,QString id)  {IniWe(1); en=false; ready=false; Nmax=nmax; sred=0.;sum=0.;N=0;SetNazwa(s,id);}
  double GoFun();
  void ExtFun(int i);
  bool Ready(){return ready;}
  double TakeRes() {ready=false; return sred; }
};
//----------------------------------------------------------
//
class atDeviceMathMax : public atDevice {
 private:
  double max;
  bool started;
 public:
  atDeviceMathMax(QString s,QString id) : atDevice(1,s,id) {max=0;started=false;}
  double GoFun();
  void ExtFun(int i);
};
//----------------------------------------------------------
//
class atDeviceMathMin : public atDevice {
 private:
  double min;
  bool started;
 public:
  atDeviceMathMin(QString s,QString id) : atDevice(1,s,id){min=0;started=false;}
  double GoFun();
  void ExtFun(int i);
};


*/

#endif
