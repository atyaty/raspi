/*
by AT
*/
#include "atDevice.h"


//--------------------------------------------------------
atDevice::atDevice(QString s,QString id){    SetParams();    SetNazwa(s,id);    setWy(this);}
//--------------------------------------------------------
atDevice::atDevice(int n, QString s,QString id){    SetParams();    SetNazwa(s,id);    setWy(this);    addWe(n);}
//--------------------------------------------------------
atDevice::~atDevice(){
    if(Error!=NULL) delete Error;
    for(int i=0; i<aWe.size();i++)   if(!WeEmpty(i))   delete aWe.at(i);
}
//--------------------------------------------------------
void atDevice::setWy( atDevice *d){    aWy.push_back(d); }

//--------------------------------------------------------
void atDevice::SetParams(){
    aParts.resize(0);
    Error=NULL;
    enabled=false;
    Wyjscie=0;
    ParentDev=NULL;
    ParentPartNo=-1;
    ParentSzereg=-1;
    EveOnUp=NULL;
    EveOnDn=NULL;

}
//--------------------------------------------------------
//jak sobie przestawiamy szereg to przestawiamy także wszystkim podłączonym do naszych wejśc
void atDevice::SetParentSzereg(int n){
    if(n>ParentSzereg){
        ParentSzereg=n;
        for(int i=0;i<WeSize();i++){
            if(aWe.at(i)->devExternal()!=NULL){
                atDevice *dP=aWe.at(i)->devExternal()->GetGrandParent(ParentDev);//patrz opis GetGrandParent
                if(dP!=NULL) dP->SetParentSzereg(ParentSzereg+1);
            }
        }
    }
}
//--------------------------------------------------------
int atDevice::GetSzeregi(){
    int szeregi=0;
    for(int i=0;i<PartSize();i++)  if( aParts.at(i)->GetParentSzereg()>szeregi) szeregi=aParts.at(i)->GetParentSzereg();
    return szeregi+1;
}
//--------------------------------------------------------
//zwraca numer składowej o identyfikatorze id
int atDevice::PartNo(QString id){
  for( int i=0; i<aParts.size(); i++ ){ if( aParts.at(i)!=NULL && aParts.at(i)->GetId()==id ) return i;  }
  return -1;
}
//--------------------------------------------------------
//zwraca numer wejścia
int atDevice::WyNo(QString id){
  for( int i=0; i<aWy.size(); i++ ){ if( aWy.at(i)!=NULL && aWy.at(i)->GetId()==id ) return i; }
  return -1;
}
//--------------------------------------------------------
//zwraca numer wejścia
int atDevice::WeNo(QString id){
  for( int i=0; i<aWe.size(); i++ ){ if( aWe.at(i)!=NULL && aWe.at(i)->GetId()==id ) return i; }
  return -1;
}
//---------------------------------------------------------
atDevice *atDevice::WeExternal(int i) {if(!WeEmpty(i)) return aWe.at(i)->devExternal();  else return NULL;}
//---------------------------------------------------------
bool atDevice::WeExternalJestWolny(int i){if(!WeEmpty(i)) return aWe.at(i)->devExternal()==NULL; else return false; }
//---------------------------------------------------------
atDevice *atDevice::WeInternal(int i) {if(!WeEmpty(i)) return aWe.at(i)->devInternal();  else return NULL;}
//---------------------------------------------------------
bool atDevice::WeInternalJestWolny(int i){ if(!WeEmpty(i)) return aWe.at(i)->devInternal()==NULL; else return false; }

//---------------------------------------------------------
//podłączamy wyjście nWy urządzenia "d" do naszego wejścia "nWe"
//
bool atDevice::connect(atDevice *d, unsigned nWy, unsigned nWe) {
    if( d==this ) {  SetError(ERROR_atDevice_connect_Self,"" );  return false; }
    if( ParentDev==NULL ) {  SetError(ERROR_atDevice_connect_ParentEmpty,"" );  return false; }
    if( d->WyEmpty(nWy) ){  SetError(ERROR_atDevice_connect_Wy,""); return false; }
    if( WeEmpty(nWe)) {  SetError(ERROR_atDevice_connect_WeEmpty,"We: "+QString::number(nWe) );  return false; }
    if( !WeExternalJestWolny(nWe)) {  SetError(ERROR_atDevice_connect_Reconnect,"Dev: "+d->GetId()+", We: "+QString::number(nWe) );  return false; }
    aWe.at(nWe)->setDevExternal(d->Wy(nWy));
    atDevice *dP=d->GetGrandParent(ParentDev);
    if(dP!=NULL) dP->SetParentSzereg(ParentSzereg+1); //w szregu ustawiamy obiekt dP a nie d !!! (chociaż czasami dP==d)
    return true;
}
//--------------------------------------------------------
//metoda dodaje składowe podłączane urzadzenia
//składowa po dodaniu łączona jest z odpowiednim istniejącym juz urządzeniem
//nWy - wyjście urządzenia d
//nWe - wejście podzespołu nP
bool atDevice::addPart(atDevice *d,unsigned nWy, unsigned nP, unsigned nWe){
    if(  PartEmpty(nP) ){	SetError(ERROR_atDevice_addParts_n,""); return false; }
    setPart(d);
    aParts.at(nP)->connect(d,nWy,nWe);
    return true;
}
//--------------------------------------------------------
//metoda dodaje składowe podłączane urzadzenia
//składowa po dodaniu łączona jest z odpowiednim istniejącym juz urządzeniem
bool atDevice::addPart(atDevice *d, unsigned nWy, unsigned nP, QString idWe){
    if( PartEmpty(nP) ){	SetError(ERROR_atDevice_addParts_id,""); return false; }
    setPart(d);
    aParts.at(nP)->connect(d,nWy,idWe);
    return true;
}

//---------------------------------------------------------
// dodanie n wejsc niepodłączonych
bool atDevice::addWe(int n) {for(int i=0;i<n;i++) addWe();   return true; }
//---------------------------------------------------------
// dodanie wejscie niepodłączone wewnętrznie
bool atDevice::addWe() {QString s="wejscie_"+QString::number(aWe.size()); aWe.push_back( new atWejscie( s,s ));    return true; }
//---------------------------------------------------------
// dodanie wejscie niepodłączone wewnętrznie
bool atDevice::addWe(QString s, QString id ) { aWe.push_back( new atWejscie( s,id ));    return true; }
//---------------------------------------------------------
// dodanie wejscia i podłączenie go do wejścia nW składowej nP
bool atDevice::addWe(QString s, QString id, unsigned nP, unsigned nW) {
    if( PartEmpty(nP) ){	SetError(ERROR_atDevice_addWe_n_Part,""); return false; }
    if( !Part(nP)->WeExternalJestWolny(nW) ){	SetError(ERROR_atDevice_addWe_n_We,""); return false; }
    aWe.push_back( new atWejscie( s,id, Part(nP), nW ));
    return true;
}
//---------------------------------------------------------
// dodanie wejscia i podłączenie go do wejścia nW składowej nP
bool atDevice::addWe(QString s, QString id, unsigned nP, QString idW) {
    if( PartEmpty(nP) ){	SetError(ERROR_atDevice_addWe_id_Part,""); return false; }
    if( !Part(nP)->WeExternalJestWolny(idW) ){	SetError(ERROR_atDevice_addWe_id_We,""); return false; }
    aWe.push_back( new atWejscie( s,id, Part(nP), Part(nP)->WeNo(idW) ));
    return true;
}


//--------------------------------------------------------
//metoda dodaje połaczenie składowej nD0 do wejscia nW składowej nD1
bool atDevice::addConnection(unsigned nD0, unsigned nWy, unsigned nD1, unsigned nWe){
 if( Part(nD0)==NULL || Part(nD1)==NULL || nD0<=nD1 ){ SetError(ERROR_atDevice_addConnection_nn,"");  return false;}
 return Part(nD1)->connect( Part(nD0), nWy, nWe);
}
//--------------------------------------------------------
//metoda dodaje połaczenie składowej nD0 do wejscia nW składowej nD1
bool atDevice::addConnection(unsigned nD0, QString idWy, unsigned nD1, unsigned nWe){
 if( Part(nD0)==NULL || Part(nD1)==NULL || nD0<=nD1 ){ SetError(ERROR_atDevice_addConnection_in,"");  return false;}
 return Part(nD1)->connect( Part(nD0), Part(nD0)->WyNo(idWy) , nWe);
}
//--------------------------------------------------------
//metoda dodaje połaczenie składowej nD0 do wejscia nW składowej nD1
bool atDevice::addConnection(unsigned nD0, unsigned nWy, unsigned nD1, QString idWe){
 if( Part(nD0)==NULL || Part(nD1)==NULL || nD0<=nD1 ){ SetError(ERROR_atDevice_addConnection_ni,"");  return false;}
 return Part(nD1)->connect( Part(nD0), nWy, Part(nD1)->WeNo(idWe));
}
//--------------------------------------------------------
//metoda dodaje połaczenie składowej nD0 do wejscia nW składowej nD1
bool atDevice::addConnection(unsigned nD0, QString idWy, unsigned nD1, QString idWe){
 if( Part(nD0)==NULL || Part(nD1)==NULL || nD0<=nD1 ){ SetError(ERROR_atDevice_addConnection_ii,"");  return false;}
 return Part(nD1)->connect( Part(nD0), Part(nD0)->WyNo(idWy), Part(nD1)->WeNo(idWe));
}


//--------------------------------------------------------
//petla po składowych
//zaczynając od ostatniej, bo pierwsza jest najblizej wyjścia
void atDevice::GoParts(){
  bool err=false;
  QString s="";
  for( int i=aParts.size()-1; i>=0; i-- ){
   aParts[i]->Go();
   if(aParts[i]->GetError()!=NULL){err=true; s+="\r\n"+aParts[i]->GetError()->Opis(); }
  }
  if(err) SetError(ERROR_atDevice_GoParts,s);
}
//--------------------------------------------------------
//petla po składowych dla funkcji ExtFun
//zaczynając od ostatniej, bo pierwsza jest najblizej wyjścia
void atDevice::ExtFunParts(int j){
  bool err=false;
  QString s="";
  for( int i=aParts.size()-1; i>=0; i-- ){
   aParts[i]->ExtFun(j);
   if(aParts[i]->GetError()!=NULL){err=true; s+="\r\n"+aParts[i]->GetError()->Opis(); }
  }
  if(err)  SetError(ERROR_atDevice_ExtFunParts,s);
}
//--------------------------------------------------------
//standardowa funkcja wyjscia
//- jeśli są podzespoły to: na wyjściu jest powtórzona wartość wyjścia zerowego podzespołu
//- jeśli nie ma podzespołów to: na wyjściu jest wsawiana watość NAND z wejść [brak wejścia (czyli: aWe.at(i)==NULL)traktowany jest jak 0 na wejściu]
//- jeśli nie ma wejść to: na wyjsciu jest zero
//funkcja ta jest wirtualna i może byc podmieniona na dowolną inną
double atDevice::GoFun(){
   if( aParts.size()>0 ){	GoParts();	return aParts.at(0)->GetWy();   }
   for( int i=0; i<aWe.size(); i++ ) if( aWe.at(i)==NULL || aWe.at(i)->GetWy()==0.0) return 1.0;
   return 0.0;
}
//---------------------------------------------------------
//standardowa funkcja zewnętrzna
//uruchamia funkcje zewnętrzne w podzespołach (z parametrem takim jak sama dostala)
//jednocześnie przestawia znacznik enabled w zależnosci od parametru
void atDevice::ExtFun(int j){
 ExtFunParts(j);
 SetEnabled(j>0);
}
//---------------------------------------------------------
void atDevice::SetFun(int j){}
//---------------------------------------------------------
//funkcja szuka dziadka :)
//jako parametr przekazany jest wskaźnik dokumentu nadrzędnego d
//jeśli naszym rodzicem jest d to znaczy że my sami jesteśmy dziadkiem
//jeśli nie to sprawdzamy czy nasz rodzic nie jest dziadkiem rekurencyjnie
// w ten sposób jeśli któryś z naszych Grand-rodziców będzie miał d za rodzica, to znaczy że on jest dziadkiem
// innymi słowy: d jest punktem odniesienia a dziadkiem jest ten z naszej rodziny kto jest bezpośrednio pod punktem d
//ta funkcja jest uzywana do szeregowania Partsów
atDevice *atDevice::GetGrandParent( atDevice *d) {
  if(ParentDev==d) return this; //kolejność ifów jest ważna, daje nam to że dla d==NULL mamy dziadka a nie NULL
  if(ParentDev==NULL) return NULL;
  return ParentDev->GetGrandParent(d);
}

//--------------------------------------------------------
//uruchamiacz od zdarzeń
void atDevice::GoEve(){
	if(Wyjscie>0 && !EveStatus){ EveStatus=true;  if (EveOnUp!=NULL) EveOnUp();	}
	if(Wyjscie<=0 && EveStatus){ EveStatus=false; if( EveOnDn!=NULL) EveOnDn();	}
}



