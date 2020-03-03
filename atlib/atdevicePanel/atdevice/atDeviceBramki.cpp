/*
by AT
*/
#include "atDeviceBramki.h"



//==============================================================
//funkcje GoFun klas dziedziczonych
double atDeviceOnOff::GoFun(){ return Enabled()?1.0:0.0; }
//--------------------------------------------------------------
void atDeviceOnOff::SetFun(int i){
   if(i<=0) SetEnabled(false);
   else if(i==1) SetEnabled(true);
   else SetEnabled(!Enabled());
}
//==============================================================
double atDeviceAND::GoFun(){
  if(aWe.size()<=0){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  for( int i=0; i<aWe.size(); i++ ) if( aWe.at(i)==NULL || aWe.at(i)->GetWy()==0.0) return 0.0;
  return 1.0;
}
//==============================================================
double atDeviceOR::GoFun(){
  if(aWe.size()<=0){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  for( int i=0; i<aWe.size(); i++ ){ if( aWe.at(i)!=NULL && aWe.at(i)->GetWy()!=0.0)  return 1.0;  }
  return 0.0;
}
//==============================================================
double atDeviceNOR::GoFun(){
  if(aWe.size()<=0){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  for( int i=0; i<aWe.size(); i++ ) if( aWe.at(i)!=NULL &&  aWe.at(i)->GetWy()!=0.0) return 0.0;
  return 1.0;
}
//==============================================================
double atDeviceNAND::GoFun(){
  if(aWe.size()<=0){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  for( int i=0; i<aWe.size(); i++ ) if( aWe.at(i)==NULL || aWe.at(i)->GetWy()==0.0) return 1.0;
  return 0.0;
}
//==============================================================
double atDeviceTrue::GoFun(){ return 1.0; }
//==============================================================
double atDeviceFalse::GoFun(){ return 0.0; }
//==============================================================
double atDeviceSum::GoFun(){
  if(aWe.size()<=0){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  double sum=0.0;
  for( int i=0; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ) sum+=aWe.at(i)->GetWy();
	else SetError(ERROR_atDevice_WeNotConected,"");
  return sum;
}
//==============================================================
double atDeviceInv::GoFun(){
  if(aWe.size()<=0){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  double sum=0.0;
  for( int i=0; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ) sum+=aWe.at(i)->GetWy();
	else SetError(ERROR_atDevice_WeNotConected,"");
  return -sum;
}
//==============================================================
double atDeviceRoz::GoFun(){
  if(aWe.size()<=1){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  double sum=0.0;
  if( aWe.at(0)!=NULL ) sum+=aWe.at(0)->GetWy();
  else SetError(ERROR_atDevice_WeNotConected,"");
  for( int i=1; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ) sum-=aWe.at(i)->GetWy();
	else SetError(ERROR_atDevice_WeNotConected,"");
  return sum;
}
//==============================================================
double atDeviceMno::GoFun(){
  if(aWe.size()<=0){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  double mul=1.0;
  for( int i=0; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ) mul*=aWe.at(i)->GetWy();
	else SetError(ERROR_atDevice_WeNotConected,"");
  return mul;
}
//==============================================================
double atDeviceOdw::GoFun(){
  if(aWe.size()<=0){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  double mul=1.0;
  for( int i=0; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ) mul*=aWe.at(i)->GetWy();
	else SetError(ERROR_atDevice_WeNotConected,"");
  if(mul!=0.) return 1./mul;
  else return 0.;
}
//==============================================================
double atDeviceDzi::GoFun(){
  if(aWe.size()<=1){SetError(ERROR_atDevice_ZaMaloWejsc,""); return 0.;};
  double sum=0.0;
  if( aWe.at(0)!=NULL ) sum=aWe.at(0)->GetWy();
  else SetError(ERROR_atDevice_WeNotConected,"");
  for( int i=1; i<aWe.size(); i++ )
	if( aWe.at(i)!=NULL ){	 if( aWe.at(i)->GetWy()!=0.0) sum/=aWe.at(i)->GetWy();	 else sum=0.;	}
	else  SetError(ERROR_atDevice_WeNotConected,"");
  return sum;
}
//==============================================================
double atDeviceAbs::GoFun(){
  if( aWe.at(0)!=NULL ) return qAbs( aWe.at(0)->GetWy() );
  return 0.;
}
