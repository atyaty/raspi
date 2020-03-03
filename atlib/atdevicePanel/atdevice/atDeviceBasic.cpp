/*
by AT
*/
#include "atDeviceBasic.h"

//==============================================================
double atDeviceSemafor::GoFun(){    //powtarza na wyjściu wartość z we0 o ile we1!=0
  if( aWe.size()<2 ){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  if( aWe.at(0)!=NULL && aWe.at(1)!=NULL && aWe.at(1)->GetWy()!=0. )  return aWe.at(0)->GetWy();
  return 0.;
}

//==============================================================
double atDeviceSem::GoFun(){    //powtarza na wyjściu wartość z we0 o ile we1!=0
  if( aWe.at(we_nr)!=NULL ) return aWe.at(we_nr)->GetWy();
  else SetError( ERROR_atDevice_WeNotConected ,"");
  return 0.;
}
//---------------------------------------------------------------
void atDeviceSem::SetFun(int j){
 if( j<0 || aWe.size()<=j ){ SetError(ERROR_atDevice_ZaMaloWejsc,""); }
 else { we_nr=j; Go();}
}


//==============================================================
//
atDeviceTara::atDeviceTara(QString s,QString id):atDevice(1,s,id){ tara=0.0;}
double atDeviceTara::GoFun(){ return (aWe.at(0)!=NULL) ?  aWe.at(0)->GetWy()-tara : 0.0;}
void atDeviceTara::SetFun(int i){ tara=(i==1 && aWe.at(0)!=NULL)? aWe.at(0)->GetWy() :0.0; Go();}

/*

//==============================================================
//
atDeviceCalc::atDeviceCalc(QString tr,QString s,QString id):atDevice(s,id){
 Tr=new atTrace(tr);
 EditWidth=50;
 EditWeWidth=50;
 IniWe(1);
 SetDev();
// SetNazwa(s,id);
}
//---------------------------------------------------------------
void atDeviceCalc::SetDev(){
 Tr->Wczytaj();
 if(Tr->GetN()<=0){
        QString s="atDeviceCalc, Empty trace: "; s.append(Tr->GetFileName()); s.append(" N="); s.append(IntToStr(Tr->GetN()).c_str());
	SetError(atDevice_ERROR_Trace_Empty,NULL,s);
 }
 Go();
}
//---------------------------------------------------------------
atDeviceCalc::~atDeviceCalc(){delete Tr;atDevice::~atDevice();}
//---------------------------------------------------------------
double atDeviceCalc::GoFun(){ return (Tr->GetN()>0 && aWe.at(0)!=NULL) ? Tr->GetY( aWe.at(0)->GetWy() ) : 0.0;}


//==============================================================
double atDeviceMathSrednia::GoFun(){
 if( en && aWe.at(0)==NULL){
  sum += aWe.at(0)->GetWy();
  N++;
  if(N==Nmax){ sred=sum/N; ready=true; sum=0.; N=0;}
 }
 return sred;
}
//---------------------------------------------------------------
void atDeviceMathSrednia::ExtFun(int i){ en=(i==0?false:true); ready=false; sred=0.; sum=0.; N=0; }
//==============================================================
// znajduje maksymalną z wejścia
double atDeviceMathMax::GoFun(){
 if( Enabled() && aWe.at(0)!=NULL ){
  if(!started) {max=aWe.at(0)->GetWy(); started=true;}
  else if( aWe.at(0)->GetWy()>max ) max=aWe.at(0)->GetWy();
  return max;
 }
 return 0;
}
void atDeviceMathMax::ExtFun(int i){ atDevice::ExtFun(i); started=false; max=0; }
//==============================================================
// znajduje maksymalną z wejścia
double atDeviceMathMin::GoFun(){
 if( Enabled() && aWe.at(0)!=NULL ){
  if(!started) {min=aWe.at(0)->GetWy(); started=true;}
  else if( aWe.at(0)->GetWy()<min ) min=aWe.at(0)->GetWy();
  return min;
 }
 return 0;
}
void atDeviceMathMin::ExtFun(int i){ atDevice::ExtFun(i); started=false; min=0; }


*/
