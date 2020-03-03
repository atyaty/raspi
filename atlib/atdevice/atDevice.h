/*
by AT
*/

#ifndef atDeviceH
#define atDeviceH


#include <QVector>
#include <QString>

#include "atError.h"

//------------------------------------------------------------
class atDevice;
typedef atDevice atDevice;

class atWejscie;
typedef atWejscie atWejscie;


typedef void (* atEveFun) (void);


//------------------------------------------------------------
class atDevice {

protected:
 QString sNazwa;
 QString Id;
 atDevice *ParentDev;            //device nadrzędny. Nasz Device należy do tego ParentDev i jest umieszczony w jego tablicy aParts
 QVector <atDevice *> aParts;    //składowe
 QVector <atDevice *> aWy;       //składowe - powtórzony zerowy szereg partsów
 QVector <atWejscie *> aWe;      // .. wskaźnik tablicy wejść tego urządzenia
 double Wyjscie;                 //wyjście
 bool enabled;
 atError *Error;
 int ParentPartNo;               //numer w tablicy podzespołów rodzica do której przynależy nasz objekt
 int ParentSzereg;               //szereg w którym występuje nasz dev u swojego rodzica

 void SetError(int e, QString opis){ if(Error==NULL)  Error=new atError(e,"dev:"+Id+"\n"+opis);  }
 void SetParams();

 bool connect(atDevice *d, unsigned nWy,unsigned nWe); //podłącza do naszego wejścia "nWe" wyjście "nWy" z urządzenia "d"
 bool connect(atDevice *d, QString idWy,unsigned nWe){ return connect(d,d->WyNo(idWy),nWe); }
 bool connect(atDevice *d, unsigned nWy,QString idWe){ return connect(d,nWy,WeNo(idWe)); }
 bool connect(atDevice *d, QString idWy,QString idWe){ return connect(d,d->WyNo(idWy),WeNo(idWe)); }
 bool connect(atDevice *d, unsigned nWe){ return connect(d,0,nWe);}
 bool connect(atDevice *d, QString idWe){ return connect(d,0,WeNo(idWe)); }

 void setPart( atDevice *d){  aParts.push_back(d);   d->ParentDev=this;    d->ParentPartNo=aParts.size()-1;  d->ParentSzereg=0; }
 void setWy( atDevice *d);

 //events
 bool EveStatus;
 atEveFun EveOnUp; //przejście wyjścia(!) przez 0 w górę
 atEveFun EveOnDn; // w dół
 void GoEve();  //uruchamiacz zdarzeń


public:
 atDevice(QString s,QString id);
 atDevice(int n, QString s,QString id);
// atDevice(QString s,QString id){ SetParams(); SetNazwa(s,id); setWy(this); }
// atDevice(int n, QString s,QString id){ SetParams(); SetNazwa(s,id); setWy(this); addWe(n); }
 ~atDevice();

 void SetId(QString id){Id=id;}
 void SetNazwa(QString s){sNazwa=s;}
 void SetNazwa(QString s,QString id){sNazwa=s;Id=id;}
 QString GetNazwa(){ return sNazwa;}
 QString GetId() { return Id; }

 atDevice *GetParent(){return ParentDev;}
 int GetParentPartNo(){return ParentPartNo;}
 int GetParentSzereg(){return ParentSzereg;}
 void SetParentSzereg(int n);
 int GetSzeregi();                                     //ilość naszych szeregów
 atDevice *GetBrother(atDevice *d);


 double GetWy() {  return Wyjscie; }
// double GetWy(int i){ atDevice *p=Part(i);  return p->GetWy(); }
// double GetWy(QString id){  atDevice *p=Part(id);  return p->GetWy(); }

 atError *GetError() { return Error; }
 int GetErrorCode(){ return (Error!=NULL) ? Error->GetCode() : 0;}
 QString GetErrorOpis(){ return (Error!=NULL) ? Error->Opis() : "";}
 bool IsError(){return Error!=NULL;}
 void SetWyjscie(double w){ Wyjscie=w;}
 bool Enabled(){return enabled;}
 void SetEnabled(bool e){enabled=e;}

 int PartNo(QString id);
 int PartSize() {return aParts.size();}
 int lastPartNo() {return aParts.size()-1;}
 bool PartEmpty(int i){ return i<0 || i>=aParts.size() || aParts.at(i)==NULL; }
 bool PartEmpty(QString id){return PartNo(id)<0;}
 atDevice *Part(int i) { if(i>=0 && i<aParts.size()) return aParts.at(i); return NULL; }
 atDevice *Part(QString id) { return Part(PartNo(id)); }
 atDevice *lastPart() {if(aParts.size()==0)return NULL; else return aParts.at(aParts.size()-1);}

 int WyNo(QString id);
 int WySize() { return aWy.size(); }
 bool WyEmpty(int i){ return i<0 || i>=aWy.size() || aWy.at(i)==NULL; }
 bool WyEmpty(QString id){ return WyNo(id)<0; }
 atDevice *Wy(int i) { if(i>=0 && i<aWy.size()) return aWy.at(i); return NULL; }

 int WeNo(QString id);
 int WeSize() { return aWe.size(); }
 bool WeEmpty(int i){ return i<0 || i>=aWe.size() || aWe.at(i)==NULL; }
 bool WeEmpty(QString id){ return WeNo(id)<0; }

 atDevice *WeExternal(int i);
 atDevice *WeExternal(QString id){ return WeExternal(WeNo(id)); }
 bool WeExternalJestWolny(int i);
 bool WeExternalJestWolny(QString id){ return WeExternalJestWolny(WeNo(id)); }

 atDevice *WeInternal(int i);
 atDevice *WeInternal(QString id){ return WeInternal(WeNo(id)); }
 bool WeInternalJestWolny(int i);
 bool WeInternalJestWolny(QString id){ return WeInternalJestWolny(WeNo(id)); }

 bool addPart( atDevice *d){ setPart(d); setWy(d); return true; }
 bool addPart(atDevice *d, unsigned nWy, unsigned nP, unsigned nWe);
 bool addPart(atDevice *d, unsigned nWy, unsigned nP, QString idWe);
 bool addPart(atDevice *d, unsigned nWy, QString idP, unsigned nWe){  return addPart(d,nWy,PartNo(idP),nWe); }
 bool addPart(atDevice *d, unsigned nWy, QString idP, QString idWe){  return addPart(d,nWy,PartNo(idP),idWe); }
 bool addPart(atDevice *d, QString idWy, QString idP, unsigned nWe){  return addPart(d,d->WyNo(idWy),PartNo(idP),nWe); }
 bool addPart(atDevice *d, QString idWy, QString idP, QString idWe){  return addPart(d,d->WyNo(idWy),PartNo(idP),idWe); }
 bool addPart(atDevice *d, QString idWy, unsigned nP, unsigned nWe){  return addPart(d,d->WyNo(idWy),nP,nWe); }
 bool addPart(atDevice *d, QString idWy, unsigned nP, QString idWe){  return addPart(d,d->WyNo(idWy),nP,idWe); }
 bool addPart(atDevice *d, QString idP, unsigned nWe){  return addPart(d,0,PartNo(idP),nWe); }
 bool addPart(atDevice *d, QString idP, QString idWe){  return addPart(d,0,PartNo(idP),idWe); }
 bool addPart(atDevice *d, unsigned nP, unsigned nWe){  return addPart(d,0,nP,nWe); }
 bool addPart(atDevice *d, unsigned nP, QString idWe){  return addPart(d,0,nP,idWe); }

 bool addWe( int n);
 bool addWe( );
 bool addWe( QString s, QString id);
 bool addWe( QString s, QString id, unsigned nP, unsigned nW);
 bool addWe( QString s, QString id, unsigned nP, QString idW);
 bool addWe( QString s, QString id, QString idP, unsigned nW){return addWe(s,id,PartNo(idP),nW); }
 bool addWe( QString s, QString id, QString idP, QString idW){return addWe(s,id,PartNo(idP),idW); }

 bool addConnection( unsigned nD0, unsigned nWy, unsigned nD1, unsigned nWe);
 bool addConnection( unsigned nD0, QString idWy, unsigned nD1, unsigned nWe);
 bool addConnection( unsigned nD0, unsigned nWy, unsigned nD1, QString idWe);
 bool addConnection( unsigned nD0, QString idWy, unsigned nD1, QString idWe);
 bool addConnection( QString idD0, unsigned nWy, QString idD1, unsigned nWe){return addConnection(PartNo(idD0),nWy,PartNo(idD1),nWe);}
 bool addConnection( QString idD0, QString idWy, QString idD1, unsigned nWe){return addConnection(PartNo(idD0),idWy,PartNo(idD1),nWe);}
 bool addConnection( QString idD0, unsigned nWy, QString idD1, QString idWe){return addConnection(PartNo(idD0),nWy,PartNo(idD1),idWe);}
 bool addConnection( QString idD0, QString idWy, QString idD1, QString idWe){return addConnection(PartNo(idD0),idWy,PartNo(idD1),idWe);}

 void GoParts();
 void ExtFunParts(int i);
 double Go(){  Wyjscie=GoFun();  GoEve(); return Wyjscie;}
 virtual double GoFun();
 virtual void ExtFun(int i);
 virtual void SetFun(int i);
 virtual QString GetOpis() {return "";}

 friend class atWejscie;

 void SetEveOnUp(atEveFun f){EveOnUp=f;}
 void SetEveOnDn(atEveFun f){EveOnDn=f;}

};





//------------------------------------------------------------
// atWejscie zawiera dwa wskaźniki deviców:
// dInternal - wskaźnik wewnętrznego urządzenia do którego podłączone jest wejście
// dExternal - wskaźnik zewnętrznego urządzenia podłączonego do naszego wejscia
// nInternalWe - numer wejścia urządzenia wewn. do którego jesteśmy podłączeni
//
class atWejscie {
private:
    QString Id;
    QString sNazwa;
    atDevice *dExternal;
    atDevice *dInternal;
    int nInternalWe;        //wejście w Internalu do którego my mamy być podłączeni
public:
    atWejscie(QString s,QString id){Id=id;sNazwa=s;dInternal=NULL;nInternalWe=0;dExternal=NULL;}
    atWejscie(QString s,QString id,atDevice *d, int n){Id=id;sNazwa=s;dInternal=d;nInternalWe=n;dExternal=NULL;}
    atWejscie(atDevice *d, int n){Id="wejscie_"+d->GetId();sNazwa=d->GetNazwa();dInternal=d;nInternalWe=n;dExternal=NULL;}
    ~atWejscie(){}

    atDevice *devExternal(){ return dExternal; }
    atDevice *devInternal(){ return dInternal; }

    void setDevExternal(atDevice *d){dExternal=d; if(dInternal!=NULL) dInternal->connect(d,nInternalWe);}
    QString GetId(){return Id;}
    QString GetNazwa(){return sNazwa;}
    double GetWy(){return devExternal()->GetWy();}
};


#endif
