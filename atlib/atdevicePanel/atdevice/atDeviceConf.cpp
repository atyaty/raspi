/*
by AT
*/

#include "atDeviceConf.h"

//==============================================================
//
atDeviceConf::~atDeviceConf() {
  if(war!=NULL) delete [] war;
  if(naz!=NULL) delete []naz;
  for(int i=0; i<aParts.size(); i++){ if(aParts.at(i)!=NULL){ delete aParts.at(i); aParts.replace(i,NULL); }}
}
//--------------------------------------------------------------
void atDeviceConf::SetDevs(atKonfiguracja *cnf,QString pole){
  N=0;
  CNF=cnf;
  Pole=pole;
  war=NULL;
  naz=NULL;
  Len=CNF->getPola( Pole ); //ilośc pozycji podrzędnych w polu
  if(Len>0){
    war = new double[Len];
    naz = new QString[Len];
    for(int i=0;i<Len;i++){
       naz[i]=CNF->getTagNaz(Pole,i);
       war[i]=CNF->getFloat(Pole,i);
    }
  }
  nN=0.; nLen=Len;
  addPart(new atDeviceInput(&nN,"N","N"));
  addPart(new atDeviceInput(&nLen,"Len","Len"));
}
//---------------------------------------------------------------
// z konfiguracji wczytujemy serię wartości
// N oznacza która wartośc wyświetlamy
// wczytane wartości liczymy od 0 !!!
double atDeviceConf::GoFun(){
  if(!Enabled()){ return 0.;}
  GoParts();
  if( N>=0 && N<Len ){return war[N];}
  SetError(ERROR_atDeviceConf_N,"N="+QString::number(N)+", Len="+QString::number(Len));
  return 0.;
}
//---------------------------------------------------------------
void atDeviceConf::SetFun(int i){
 if(i<0){N=0;}
 else if(i>=Len){ N=Len-1;}
 else {N=i;}
 nN=N;
 GoParts();
 Go();
}
//---------------------------------------------------------------



