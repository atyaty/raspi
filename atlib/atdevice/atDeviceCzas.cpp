/*
by AT
*/
#include "atDeviceCzas.h"

//==============================================================
//--------------------------------------------------------------
atDeviceCzas::atDeviceCzas(atKonfiguracja *cnf,int n, bool r,QString s,QString id ):atDeviceConf(cnf,id,s,id){
  if(n>=1) addWe("Period","period");
  if(n>=2) addWe("Start/Stop","start");
  if(n>=3) addWe("Pause/continue","pause");
  period=0;
  repeat=r;
  for(int i=0;i<Len;i++){  if(naz[i]=="period"){period=war[i];}   }
  CZAS=new atCzas();
  addPart(new atDeviceInput(&Tim,"Czas od startu","_TIME"));
  addPart(new atDeviceInput(&Rest,"Czas do końca","_REST"));
}
//---------------------------------------------------------------
atDeviceCzas::~atDeviceCzas(){
 delete CZAS;
 for(int i=0; i<aParts.size(); i++){ if(aParts.at(i)!=NULL){ delete aParts.at(i); aParts.replace(i,NULL); }}
}
/*/fat/cpp-qt/qt/atlib/atdevice/atDeviceCzas.cpp:22: warning: deleting object of polymorphic class type 'atDevice' which has non-virtual destructor might cause undefined behaviour [-Wdelete-non-virtual-dtor]
  for(int i=0; i<aParts.size(); i++){ if(aParts.at(i)!=NULL){ delete aParts.at(i); aParts.replace(i,NULL); }}
                                                                                ^*/
//---------------------------------------------------------------
double atDeviceCzas::GoFun(){
  if(!Enabled()) return 0;
  if(aWe.size()>0 && aWe.at(0)!=NULL){ period = aWe.at(0)->GetWy(); }
  if(aWe.size()>1 && aWe.at(1)!=NULL){
      if(CZAS->Going() && aWe.at(1)->GetWy()<=0 && LastWe==1) CZAS->Stop();
      if(!CZAS->Going() && aWe.at(1)->GetWy()>0 && LastWe==0) CZAS->Start();
      LastWe=(aWe.at(1)->GetWy()>0)?1:0;
      int a=5;
  }
  if(aWe.size()>2 && aWe.at(2)!=NULL){
      if(!CZAS->Pausing()&&aWe.at(2)->GetWy()>0) CZAS->Pause();
      if(CZAS->Pausing()&&aWe.at(2)->GetWy()<=0) CZAS->Continue();
  }
  if(!CZAS->Going() && repeat){ CZAS->Start(); }
  if(CZAS->Going() && (period<=0 || CZAS->Seconds()<=period) ){
      Tim=CZAS->Seconds();
      Rest=(period>0)?(period-Tim):0;
      GoParts();
      return 1.;
  }
  CZAS->Stop();
  return 0.;
}
//---------------------------------------------------------------
void atDeviceCzas::ExtFun(int i){
  atDeviceConf::ExtFun(i);
  if(i<=0){CZAS->Stop();}
  if(aWe.size()>1 && aWe.at(1)!=NULL){ LastWe=1; }
}
//---------------------------------------------------------------
void atDeviceCzas::SetFun(int i){
 if(i<=0){CZAS->Stop();}
 else if(i==1&&Enabled()){CZAS->Start(); }
 else if(i==2&&Enabled()){CZAS->Pause();}
 else if(i==3&&Enabled()){CZAS->Continue();}
}


/*
//==============================================================
// FIFO - średnia ciągniona
//--------------------------------------------------------------
atDeviceFIFO::atDeviceFIFO(RealTimeLoop *rtl,Konfiguracja *cnf,QString s,QString id) :atDeviceConfs(cnf,id,s,id){
 EditWidth=50;
 EditWeWidth=50;
 period=0.; bufflen=0.;
 IniWe(1);

 for(int i=0;i<warLen;i++){
   if(naz[i]=="period"){	 period=war[i];  };
   if(naz[i]=="bufflen"){	 bufflen=war[i]; };
 }
 if( period<=0. && bufflen<=0.) SetError(atDevice_ERROR_FIFO,"");

 CZAS=new atCzas(rtl);
 REGR=new atRegresja();
 //Devices wyjścia
 addPart(new atDeviceInput(&nIlosc,"n","n"));
 addPart(new atDeviceInput(&wsp_a,"a","a"));
 addPart(new atDeviceInput(&wsp_b,"b","b"));
 addPart(new atDeviceInput(&wsp_r,"r","r"));
 addPart(new atDeviceInput(&sred_x,"srx","srx"));
 addPart(new atDeviceInput(&sred_y,"sry","sry"));
 addPart(new atDeviceInput(&odch_x,"odchx","odchx"));
 addPart(new atDeviceInput(&odch_y,"odchy","odchy"));
 addPart(new atDeviceInput(&point_x0,"x0","x0"));
 addPart(new atDeviceInput(&point_y0,"y0","y0"));
 addPart(new atDeviceInput(&point_x1,"x1","x1"));
 addPart(new atDeviceInput(&point_y1,"y1","y1"));
 addPart(new atDeviceInput(&sumy,"sumy","sumy"));
 addPart(new atDeviceInput(&sumyy,"sumyy","sumyy"));
}
//-----------------------------------------------------------------
atDeviceFIFO::~atDeviceFIFO(){
 delete CZAS;
 delete REGR;
 for(unsigned i=0; i<aParts.size(); i++) delete aParts.at(i);
}
//-----------------------------------------------------------------
double atDeviceFIFO::GoFun(){
 if(Enabled()){
   val.push_back(  (aWe.at(0)!=NULL) ? aWe.at(0)->GetWy() : 0.0  );
   tim.push_back(  CZAS->Time()  );
   REGR->AddXY(  tim.at(tim.size()-1), val.at(val.size()-1)  );
   while(tim.size()>1 && ( (tim.at(tim.size()-1)-tim.at(0))>period  ||  bufflen>0 && tim.size()>bufflen) ){
	 //bufor FIFO możemy sterować długością okresu i/lub długością bufora
	 //przekroczenie jednego z ww kryteriów powoduje likwidację początkowych danych
	 REGR->RemXY(  tim.at(0), val.at(0)  );
	 tim.erase(tim.begin()); val.erase(val.begin());
	 if(tim.size()>0) REGR->SetX0(tim.at(0));
   }
   GetRegr();
   return sred_y;
 }
 return 0.;
}
//-----------------------------------------------------------------
void atDeviceFIFO::ExtFun(int i){
 val.clear(); tim.clear(); REGR->Reset();
 if(i==0){ SetEnabled(false); ExtFunParts(i); CZAS->Stop();GetRegr();}
 else if(i==1) { SetEnabled(true); ExtFunParts(i); CZAS->Start();GetRegr();}
}
//-----------------------------------------------------------------
void atDeviceFIFO::GetRegr(){
	nIlosc=REGR->n();
	wsp_a=REGR->a();
	wsp_b=REGR->b();
	wsp_r=REGR->r();
	sred_y=REGR->sy();
	sred_x=REGR->sx();
	odch_y=REGR->oy();
	odch_x=REGR->ox();
	point_x0=REGR->x0();
	point_x1=REGR->x1();
	point_y0=REGR->y0();
	point_y1=REGR->y1();
	sumy=REGR->sumy();
	sumyy=REGR->sumyy();
	GoParts();
}

//==============================================================
// ŚREDNIA
//
//--------------------------------------------------------------
atDeviceSr::atDeviceSr(RealTimeLoop *rtl,Konfiguracja *cnf,QString s,QString id) :atDeviceConfs(cnf,id,s,id){
 EditWidth=50;
 EditWeWidth=50;
 zwloka=0.;
 period=0.;
 periodic=false;
 IniWe(1);
// unsigned err=0;
 for(int i=0;i<warLen;i++){
  if(naz[i]=="period"){period=war[i];periodic=true;};
  if(naz[i]=="zwloka"){zwloka=war[i];};
 }
 CZAS=new atCzas(rtl);
 REGR=new atRegresja();

 //Devices wyjścia
 addPart(new atDeviceInput(&nIlosc,"n","n"));
 addPart(new atDeviceInput(&wsp_a,"a","a"));
 addPart(new atDeviceInput(&wsp_b,"b","b"));
 addPart(new atDeviceInput(&wsp_r,"r","r"));
 addPart(new atDeviceInput(&sred_x,"srx","srx"));
 addPart(new atDeviceInput(&sred_y,"sry","sry"));
 addPart(new atDeviceInput(&odch_x,"odchx","odchx"));
 addPart(new atDeviceInput(&odch_y,"odchy","odchy"));
 addPart(new atDeviceInput(&point_x0,"x0","x0"));
 addPart(new atDeviceInput(&point_y0,"y0","y0"));
 addPart(new atDeviceInput(&point_x1,"x1","x1"));
 addPart(new atDeviceInput(&point_y1,"y1","y1"));
 addPart(new atDeviceInput(&sumy,"sumy","sumy"));
 addPart(new atDeviceInput(&sumyy,"sumyy","sumyy"));

}
//---------------------------------------------------------------
atDeviceSr::~atDeviceSr(){
 delete CZAS;
 delete REGR;
 for(unsigned i=0; i<aParts.size(); i++) delete aParts.at(i);
}
//-----------------------------------------------------------------
void atDeviceSr::GetRegr(){
	nIlosc=REGR->n();
	wsp_a=REGR->a();
	wsp_b=REGR->b();
	wsp_r=REGR->r();
	sred_y=REGR->sy();
	sred_x=REGR->sx();
	odch_y=REGR->oy();
	odch_x=REGR->ox();
	point_x0=REGR->x0();
	point_x1=REGR->x1();
	point_y0=REGR->y0();
	point_y1=REGR->y1();
	sumy=REGR->sumy();
	sumyy=REGR->sumyy();
	GoParts();
}
//-----------------------------------------------------------------
double atDeviceSr::GoFun(){
 if(Enabled()){
  if( CZAS->Time()>zwloka && (!periodic || CZAS->Time() < (zwloka+period)) ){
	REGR->AddXY(CZAS->Time(),(aWe.at(0)!=NULL) ? aWe.at(0)->GetWy() : 0.);
  }
  if( periodic && CZAS->Time() > (zwloka+period) ){
	GetRegr(); ExtFunParts(2);	CZAS->Start();	REGR->Reset();
  }
  return sred_y;
 }
 return 0.;
}
//-----------------------------------------------------------------
void atDeviceSr::ExtFun(int i){
 if(i==0){
  SetEnabled(false); CZAS->Stop(); REGR->Reset(); GetRegr(); ExtFunParts(i);}
 else if(i==1) {
  SetEnabled(true); CZAS->Start(); REGR->Reset(); GetRegr(); ExtFunParts(i);}
 else if(Enabled()){
  GetRegr();ExtFunParts(i);CZAS->Start();REGR->Reset(); SetWy(sred_y);
  //Uwaga na Devicy które działają w oparciu o ExtFun,
  //jeśli w ExtFun wykonujemy operacje które mają wpływać na Wyjśie Devica powinniśmy
  //ustwić je funkcją SetWy w funkcji ExFun..
 }
}



*/

/*
//==============================================================
//
atDeviceBeeper::atDeviceBeeper(Konfiguracja *cnf,RealTimeLoop *rtl,QString s,QString id ):atDeviceConfs(cnf,id,s,id){
  period=10.;  freq=1000.; dur=100.;
  for(int i=0;i<warLen;i++){
   if(naz[i]=="period") period=war[i];
   if(naz[i]=="freq")   freq=war[i];
   if(naz[i]=="dur")    dur=war[i];
  }
  CZAS=new atCzas(rtl);
}
//---------------------------------------------------------------
atDeviceBeeper::~atDeviceBeeper(){ delete CZAS;}
//---------------------------------------------------------------
double atDeviceBeeper::GoFun(){
 if( Enabled()){
  if( CZAS->Time()<=period)  return 1.;
  else { Beep(freq,dur); CZAS->Reset(); }
 }
 return 0.;
}
//---------------------------------------------------------------
void atDeviceBeeper::ExtFun(int i){
   if(i<=0) SetEnabled(false);
   else { SetEnabled(true); CZAS->Reset();}
}
//---------------------------------------------------------------


*/
