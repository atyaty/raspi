//---------------------------------------------------------------------------

#ifndef atDevicePanelH
#define atDevicePanelH

//---------------------------------------------------------------------------
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QImage>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

#include "atDevice.h"
#include "atDeviceForm.h"

class atDeviceForm;
typedef atDeviceForm atDeviceForm;

class atDukt;
typedef atDukt atDukt;

class atKladka;
typedef atKladka atKladka;

class atSzereg;
typedef atSzereg atSzereg;


//---------------------------------------------------------------------------
class atQLabel : public QLabel{
    Q_OBJECT
public:
    explicit atQLabel(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *ev);
signals:
    void Mouse_Pressed();
public slots:
};
//---------------------------------------------------------------------------
class atQLineEdit : public QLineEdit{
    Q_OBJECT
public:
    explicit atQLineEdit(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *ev);
signals:
    void Mouse_Pressed();
public slots:
};

//---------------------------------------------------------------------------
class atDevicePanel : public QFrame {

    Q_OBJECT

private:

private slots:
  void ShowForm();
  void ShowErr();


protected:

 atDevice *dev;

 bool NameVisible;  //widocznosc elementów
 bool WyVisible;
 bool ErrVisible;
 bool WeVisible;
 bool PartsVisible;

 int WyDec;         //miejsca dziesiętne Wy

 QString ColorDeviceEnabled,ColorDeviceNotEnabled;
 QString ColorErrorOn,ColorErrorOff;
 QString ColorWyOn,ColorWyOff;
 QString ColorWeOn,ColorWeOff;

 int WyWidth;       //szerokosc pola Wyjscia
 int ErrWidth;      //szerokosc pola error
 int PartsWidth;    //szerokosc panelu - Podzespołu lub We
 int NameWidth;     //szerokosc opisu
 int MargWidth;     //szerokosc marginesu
 int SpaceWidth;    //szerokosc odstępów między partsami

 int nameWidth()   {return NameVisible?(NameWidth+MargWidth):0;}
 int wyWidth()     {return WyVisible?(WyWidth+MargWidth):0;}
 int errWidth()    {return ErrVisible?(ErrWidth+MargWidth):0;}
 int wejsciaWidth();
 int szeregiWidth();
 int szeregiWidth(int sz);
 int szeregWidth(int i);
 int duktyWidth(int i);

 int WyHeight;      //wysokosc pola Wyjscia/Nazwy/Error
 int PartsHeight;   //wysokosc podzespołu/We
 int MargHeight;    //wysokosc marginesu

 int wyHeight()     {return (WyVisible || ErrVisible || NameVisible)?(WyHeight+MargHeight):0;}
 int wejsciaHeight();
 int partsHeight();
 int szeregHeight(int i);
 int kladkiHeight() {return (Kladki.size()+1)*MargWidth; }

 QImage *ImageBack;
 atQLabel *LabelName;
 QLineEdit *EditWy;
 atQLineEdit *EditErr;
 QVector <atSzereg *> Szeregi;
 QVector <atKladka *> Kladki;
 atSzereg *Wejscia;

 atDeviceForm *form;

 void  showBack();
 void  showName();
 void  showErr();
 void  showWy();
 void  showWe();
 void  showParts();
 void  showDukt();

 void  moveWe();
 void  moveParts();
 void  moveKladki();

public:
     atDevicePanel(QWidget *w=0);
     ~atDevicePanel(void);

     void paintEvent(QPaintEvent*);

     void  SetDev(atDevice *d){dev=d;}
     void  SetName(int w,int h){ NameVisible=true;NameWidth=w;WyHeight=h;}
     void  SetName(int w,QString cON, QString cOFF){   NameVisible=true;    NameWidth=w; ColorDeviceEnabled=cON;ColorDeviceNotEnabled=cOFF;   }
     void  SetName(int w){   NameVisible=true;    NameWidth=w;    }
     void  SetErr(int w,int h){ ErrVisible=true;ErrWidth=w;WyHeight=h;}
     void  SetErr(int w){ErrVisible=true;ErrWidth=w;}
     void  SetWy(int w,int h,int d){WyVisible=true;WyWidth=w;WyHeight=h;WyDec=d;}
     void  SetWy(int w,int h){WyVisible=true;WyWidth=w;WyHeight=h;}
     void  SetWy(int w){WyVisible=true;WyWidth=w;}
     void  SetWe(int w,int h){WeVisible=true;PartsWidth=w;PartsHeight=h;}
     void  SetWe(){WeVisible=true;}
     void  SetParts(int w,int h){PartsVisible=true;PartsWidth=w;PartsHeight=h;}
     void  SetParts(){PartsVisible=true;WyVisible=false;}

     void  Show();
     void  Disp();
     void  CloseForm();

     atDevice * Dev(){return dev;}
     int GetParentSzereg(){return dev->GetParentSzereg();}

     atDevice * Wy(int i){return dev->Wy(i);}
     int WySize(){return dev->WySize();}

     atDevice * We(int i){return dev->WeExternal(i);}
     int WeSize(){return dev->WeSize();}

     atSzereg * Szereg(int i){return Szeregi.at(i);}
     int SzeregiSize() {return Szeregi.size();}

     atKladka * Kladka(int i){return Kladki.at(i);}
     int KladkiSize() {return Kladki.size();}


     void addKladka(atKladka *kl);

     int totalHeight()  {return height()+2*frameWidth();}
     int totalWidth()  {return width()+2*frameWidth();}

     int PosWy(int i);
     int PosWe(int i);

};

//---------------------------------------------------------------------------
//objekt opisuje połaczenia między panelami
//panele ustawione są w szeregach, każdy panel może mieć wiele wyjść
//każde wyjście z szeregu tworzy nowy Dukt czyli połączenie do innych paneli
//Jeśli wyjści tworzące Dukt jest w szeregu n to Dukt należy do szeregu n-1
//Dukt przypisany jest do tego szeregu którego wejścia obsługuje
//Dukt reprezentuje na diagramie pionową nitkę wychodzącą z jakiegoś wyjścia
//Połączenia realizowane z panelami w danym szeregu realizowane są przez Dukty przynależące do tego szeregu
//Dukty danego wyjścia z różnych szeregów łączone sa Kładką

class atDukt {
private:
  atDevicePanel *dpWy;
  int nWy;
  QVector <atDevicePanel *> dpWe;
  QVector <int> nWe;
  atKladka *kl;
  bool up;          //wskazuje czy jet w dół czy w górę, true znaczy w górę
  int DuktPos;      //
  int DuktWidth;    //szerokość - odstępy Duktów
  int No;           //pozycja (kolejność w szeregu)
public:
  atDukt(atDevicePanel *d, int n, bool u,int pos,int mW){dpWy=d;nWy=n;kl=NULL;up=u;DuktPos=pos;DuktWidth=mW;}
  void addWe(atDevicePanel *d,int n){dpWe.append(d);nWe.append(n);}
  void setKl(atKladka *k){kl=k;}
  void setNo(int p){No=p;}
  bool Up() {return up;}

  atDevicePanel *We(int i){return dpWe.at(i);}
  atKladka *Kladka(){return kl;}
  int WeSize(){return dpWe.size();}

  int PosLeft(){return dpWy->pos().x()+ dpWy->width();}   //lewy skrajny punkt (Panel Wy)
  int PosCenter(){return  DuktPos; }                      //współrzędna pozioma Duktu
  int PosRight(int i){return  dpWe.at(i)->pos().x();}     //
  int PosWy(){ return dpWy->pos().y()+dpWy->PosWy(nWy); } //współrzędna pionowa Wyjścia
  int PosWe(int i){return dpWe.at(i)->pos().y()+dpWe.at(i)->PosWe(nWe.at(i));} //współrzędna pionowa wejścia
  int PosKl();                                            //pozycja kładki (pionowa współrzędna
  int PosTop();                                           //skrajny górny punkt duktu
  int PosBottom();                                        //skrajny dony punkt duktu

  void setPos(int p){DuktPos =p;}

  int Width(){return DuktWidth;}
};
//---------------------------------------------------------------------------
class atKladka : public QFrame{
private:
 QVector <atDukt *> du;
 int No; //kolejnosć kładek
public:
  atKladka(atDevicePanel *dp){dp->addKladka(this);}
  void addDukt(atDukt *d){du.append(d); d->setKl(this);}
  void setNo(int p){No=p;}
  int getNo(){return No;}
  int PosTop(){return pos().y();}
  int PosLeft(){return pos().x();}
  int PosRight(){return pos().x()+width();}
  int setLeft();
  int setRight();

  int DuktySize(){return du.size();}
  atDukt *Dukt(int i){return du.at(i);}

};
//---------------------------------------------------------------------------
//zawiera panele unieszczone jeden nad drugim na diagramie
//zawiera też dukty czyli połączenia wychodzące z tego szeregu
class atSzereg : public QSize{
private:
  QVector <atDukt *> du;
  QVector <atDevicePanel *> PPanels;
  int MargHeight;    //wysokosc marginesu
  int MargWidth;     //szerokość - odstępy Duktów
  int sz; //numer szeregu liczony od zera. zero - prawy skrajny szereg czyli szereg wyjść
  atDevicePanel *parentPanel;

public:
  atSzereg(int s,atDevicePanel *p,int mH,int mW):QSize(0,0){sz=s;MargHeight=mH;MargWidth=mW;parentPanel=p;}
  ~atSzereg(void);
  void addDukt(atDevicePanel *d, int n);
  void addDukt(atDukt *d){ du.append(d); d->setNo(du.size()-1); setWidth(width()+d->Width()); }
  void addPanel(atDevicePanel *d);
  atDevicePanel *GetPanel(int i){return PPanels.at(i);}
  int GetPanelSize(){return PPanels.size();}
  atSzereg * Szereg(int i){return parentPanel->Szereg(i);}

  int DuktyWidth();
  int DuktySize(){return du.size();}
  atDukt *Dukt(int i){return du.at(i);}

  void moveSzereg(int top, int left, int mh, int mw);
  void Disp();
  void CloseForms();

};
//------------------------------------------------------------------------------

#endif
