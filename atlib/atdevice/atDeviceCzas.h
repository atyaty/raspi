/*
by AT
*/

#ifndef atDeviceCzasH
#define atDeviceCzasH

#include "atDeviceConf.h"
//#include "atRegresja.h"


//----------------------------------------------------------
// licznik czasu
class atDeviceCzas : public atDeviceConf {
 protected:
  atCzas *CZAS;
  bool repeat;
  double period;
  double Tim;
  double Rest;
  int LastWe;
 public:
  atDeviceCzas(atKonfiguracja *cnf,int n,bool r,QString s,QString id);
  ~atDeviceCzas();
  double GoFun();
  void ExtFun(int i);
  void SetFun(int i);
};

/*
//----------------------------------------------------------
// nie przetestowane ..jak co to wywalić
//
// Bufor danych "Ciągniony" FIFO
// wtablicach gromadzimy pewien zakres odczytów z zakresu czasowego (oś x): period
// dane mogą być poddawane dalszejobróbce w klasach dziedziczonych
// - period odczytujemy z konfiguracji
// przepełniony bufor kasuje dane które weszły jako pierwsze
// na wyjściu ustawiana jest najstarsza wartość bufora
class atDeviceFIFO : public atDeviceConf {
 private:
   atCzas *CZAS;
   atRegresja *REGR;
   double period;       //długość bufora w czasie
   double bufflen;      //długość bufora
   vector <double> val; //wartości odczytane do usrednienia (oś y)
   vector <double> tim; //czas (oś x) dla kolejnych odczytów na podst CZAS

   //parametry wyjścia
   double nIlosc;
   double wsp_a;
   double wsp_b;
   double wsp_r;
   double sred_y;
   double sred_x;
   double odch_y;
   double odch_x;
   double point_x0;
   double point_x1;
   double point_y0;
   double point_y1;
   double sumy;
   double sumyy;
   void GetRegr();

 public:
  atDeviceFIFO(RealTimeLoop *rtl,Konfiguracja *cnf,QString s,QString id);
  ~atDeviceFIFO();
  double GoFun();
  void ExtFun(int i);
};


//----------------------------------------------------------
// średnia w czasie (odcinkowa)
// na wyjście podajemy uśrednione w czasie wejście
// z konfiguracji odczytujemy:
// - okres uśredniania
// - zwłokę
class atDeviceSr : public atDeviceConfs {
 private:
   atCzas *CZAS;
   atRegresja *REGR;
   bool periodic;
   double period;
   double zwloka;

   //parametry wyjścia
   double nIlosc;
   double wsp_a;
   double wsp_b;
   double wsp_r;
   double sred_y;
   double sred_x;
   double odch_y;
   double odch_x;
   double point_x0;
   double point_x1;
   double point_y0;
   double point_y1;
   double sumy;
   double sumyy;

   void GetRegr();

 public:
  atDeviceSr(RealTimeLoop *rtl,Konfiguracja *cnf,QString s,QString id);
  ~atDeviceSr() ;
  double GoFun();
  void ExtFun(int i);
};



//----------------------------------------------------------
// beeper
// period -  z konfiguracji
// freq,dur -  z konfiguracji
class atDeviceBeeper : public atDeviceConfs {
 protected:
  atCzas *CZAS;
  double period;
  double freq;
  double dur;
 public:
  atDeviceBeeper(Konfiguracja *cnf,RealTimeLoop *rtl,QString s,QString id);
  ~atDeviceBeeper();
  double GoFun();
  void ExtFun(int i);
};
*/

#endif
