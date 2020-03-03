
#include "atDevicePanel.h"

//===========================================================================
//---------------------------------------------------------------------------
atQLabel::atQLabel(QWidget *parent) :    QLabel(parent){}
void atQLabel::mousePressEvent(QMouseEvent *ev){    emit Mouse_Pressed(); }
//===========================================================================
//---------------------------------------------------------------------------
atQLineEdit::atQLineEdit(QWidget *parent) :    QLineEdit(parent){}
void atQLineEdit::mousePressEvent(QMouseEvent *ev){    emit Mouse_Pressed(); }

//===========================================================================
//---------------------------------------------------------------------------
 atDevicePanel::atDevicePanel(QWidget *w)	: QFrame(w) {

 dev=NULL;
 ImageBack=NULL;
 LabelName=NULL;
 EditErr=NULL;
 EditWy=NULL;
 Wejscia=NULL;

 WyWidth=20;
 ErrWidth=20;
 PartsWidth=100;
 NameWidth=100;
 MargWidth=2;
 SpaceWidth=20;

 WyHeight=20;
 PartsHeight=30;
 MargHeight=2;

 WyDec=-1;

 NameVisible=false;
 WyVisible=false;
 ErrVisible=false;
 WeVisible=false;
 PartsVisible=false;


 ColorDeviceEnabled="color:black;";
 ColorDeviceNotEnabled="color:gray;";
 ColorErrorOn="background:red;color:white";
 ColorErrorOff="background:"+palette().color(QWidget::backgroundRole()).name()+";color:black";
 ColorWyOn="background:white;";
 ColorWyOff="background:"+palette().color(QWidget::backgroundRole()).name()+";";
 ColorWeOn="background:green;color:black";
 ColorWeOff="background:green;color:lightGray";

 setFrameStyle(QFrame::Panel);

 form=NULL;




 //LabelName->mouseReleaseEvent();
 //FPanel = new TPanel(this);
 //FPanel->Parent=Parent;
 //FPanel->Top=0;
 //FPanel->Left=0;
 //FPanel->Width=200;
 //FPanel->Height=200;
// FPanel->
// FPanel->Canvas->LineTo(255, 82);

/*
TControl* pControl = Screen->ActiveControl;
TRect Rect;
Rect.left =  1;
Rect.top =  1;
Rect.right = 100;
Rect.bottom = 100;
DrawFocusRect( pCanvas->Handle, &Rect );
delete pCanvas;


 FCanvas = new TControlCanvas;
 FCanvas->Control = FPanel;

 //debug
 Graphics::TBitmap *bmp = new Graphics::TBitmap;
 bmp->LoadFromFile("test.bmp");
 FCanvas->Brush->Bitmap = bmp;
 FCanvas->FillRect(Rect(0, 0, 200, 200));
 delete bmp;
 //
   */

}


//---------------------------------------------------------------------------
atDevicePanel::~atDevicePanel(void){
 if(ImageBack!=NULL) delete ImageBack;
 if(LabelName!=NULL) delete LabelName;
 if(EditErr!=NULL) delete EditErr;
 if(EditWy!=NULL) delete EditWy;
 if(Wejscia!=NULL) delete Wejscia;

 while(Szeregi.size()>0){delete Szeregi.at(Szeregi.size()-1); Szeregi.pop_back();}
 while(Kladki.size()>0){delete Kladki.at(Kladki.size()-1); Kladki.pop_back();}
 if(form!=NULL){ form->ClosePanel(); delete form; }
}

//---------------------------------------------------------------------------
void  atDevicePanel::showBack(){
/*
    // if(NameVisible){
	 if(ImageBack==NULL){  ImageBack= new TImage(this); }
	 ImageBack->Parent = this;
	 ImageBack->Align = alClient;
// }*/
}
//---------------------------------------------------------------------------
void  atDevicePanel::showName(){
 if(NameVisible){
     if(LabelName==NULL){  LabelName= new atQLabel(this); }
     LabelName->setText(dev!=NULL ? dev->GetNazwa() : "no device" );
     LabelName->setStyleSheet( (dev!=NULL && dev->Enabled()) ? ColorDeviceEnabled : ColorDeviceNotEnabled );
     LabelName->setAlignment(Qt::AlignHCenter);
     LabelName->setVisible(NameVisible);
     LabelName->resize(NameWidth,WyHeight);
     connect( LabelName, SIGNAL(Mouse_Pressed()), this, SLOT(ShowForm()) );

 }
}
//---------------------------------------------------------------------------
void  atDevicePanel::showErr(){
 if(ErrVisible){
     if(EditErr==NULL) EditErr= new atQLineEdit(this);
     EditErr->setReadOnly(true);
     EditErr->setStyleSheet( (dev==NULL || dev->IsError()) ?  ColorErrorOn : ColorErrorOff );
     EditErr->setVisible(ErrVisible);
     EditErr->resize(ErrWidth,WyHeight);
     connect( EditErr, SIGNAL(Mouse_Pressed()), this, SLOT(ShowErr()) );
 }
}
//---------------------------------------------------------------------------
void  atDevicePanel::showWy(){
 if(WyVisible){
     if(EditWy==NULL) EditWy= new QLineEdit(this);
     EditWy->setReadOnly(true);
     EditWy->setStyleSheet( ColorWyOff );
     EditWy->setVisible(WyVisible);
     EditWy->resize(WyWidth,WyHeight);
 }
}
//---------------------------------------------------------------------------
void  atDevicePanel::showParts(){
 if(PartsVisible){
     for(int i=0; i<dev->GetSzeregi();i++){ Szeregi.push_back(new atSzereg(i,this,MargHeight,MargWidth)); }
     for(int i=0; i<dev->PartSize(); i++){
           atDevicePanel *dp=new atDevicePanel(this);
           dp->SetDev(dev->Part(i));
           dp->SetWy(WyWidth,WyHeight);
           dp->SetName(NameWidth);
           dp->SetErr(ErrWidth);
           dp->Show();
           Szereg(dp->GetParentSzereg())->addPanel( dp );
     }
 }
}
//---------------------------------------------------------------------------
void  atDevicePanel::showWe(){
 if(WeVisible){
     Wejscia= new atSzereg(dev->GetSzeregi()+1,this,MargHeight,MargWidth);
     for(int i=0; i<dev->WeSize(); i++){
         atDevicePanel *dp=new atDevicePanel(this);
         dp->SetDev(dev->WeExternal(i));
         dp->SetWy(WyWidth,WyHeight);
         dp->SetName(NameWidth,ColorWeOn,ColorWeOff );
         dp->setStyleSheet( ColorWeOn );
         dp->Show();
         Wejscia->addPanel( dp );
     }
 }
}
//---------------------------------------------------------------------------
// główna pętla duktów
// jedziemy po panelach wejść oraz po panelach szeregów większych niż 0
// dla każdego panelu jedziemy po jego wyjściach
// dla każdego wyjścia odpalamy dodajemy dukt
void atDevicePanel::showDukt() {
  if(Wejscia!=NULL){
    for(int i=0; i<Wejscia->GetPanelSize(); i++){
      Szereg(SzeregiSize()-1)->addDukt( Wejscia->GetPanel(i), 0 ); //tu tylko wyjście 0
    }
  }
  for(int i=SzeregiSize()-1; i>0; i--){
      for(int j=0;j<Szereg(i)->GetPanelSize();j++){
          for(int k=0;k<Szereg(i)->GetPanel(j)->WySize();k++){
            Szereg(i-1)->addDukt( Szereg(i)->GetPanel(j), k );
          }
      }
  }
}
//---------------------------------------------------------------------------
void atDevicePanel::moveWe() {
  if(Wejscia==NULL) return;
  if(SzeregiSize() ||  Wejscia->GetPanelSize()>1)  Wejscia->moveSzereg(wyHeight()+MargHeight,MargWidth,MargHeight,MargWidth);
  else Wejscia->moveSzereg(MargHeight,MargWidth,MargHeight,MargWidth);
}
//---------------------------------------------------------------------------
void atDevicePanel::moveParts() {
  for(int i=0; i<SzeregiSize(); i++){
      Szereg(i)->moveSzereg(wyHeight()+MargHeight,MargWidth+wejsciaWidth()+szeregiWidth(i+1),MargHeight,MargWidth);
  }
}
//---------------------------------------------------------------------------
void atDevicePanel::moveKladki() {
   for(int i=0;i<KladkiSize();i++){
       Kladka(i)->resize(Kladka(i)->setRight()-Kladka(i)->setLeft(),MargHeight);
       Kladka(i)->move(Kladka(i)->setLeft(), wyHeight()+partsHeight()+MargHeight*i  );
     }
}
//---------------------------------------------------------------------------
int atDevicePanel::szeregWidth(int i) {   return Szereg(i)->width()+MargWidth; }
//---------------------------------------------------------------------------
int atDevicePanel::duktyWidth(int i) {   return Szereg(i)->DuktyWidth()+MargWidth; }
//---------------------------------------------------------------------------
//szerokość szeregów od sz do kńca
//szereg 0 jest to ten z prawej strony czyli szereg wyjść
int atDevicePanel::szeregiWidth(int sz) {
    int w=0;
    for(int i=sz;i<SzeregiSize();i++){ w+=szeregWidth(i)+MargWidth+duktyWidth(i);    }
//    if(sz>=0&&sz<SzeregiSize())w+=Szereg(sz-1)->DuktyWidth();
    return w;
}

//---------------------------------------------------------------------------
//szerokość wszystkich szeregów
int atDevicePanel::szeregiWidth() { return szeregiWidth(0);}
//---------------------------------------------------------------------------
//szerokość wejsc
int atDevicePanel::wejsciaWidth() { return  WeVisible?Wejscia->width():0; }
//---------------------------------------------------------------------------
int atDevicePanel::partsHeight() {
    int h=0;
    for(int i=0;i<Szeregi.size();i++){ if(Szeregi.at(i)->height()>h) h=Szeregi.at(i)->height();    }
    return h+MargHeight;
}
//---------------------------------------------------------------------------
int atDevicePanel::wejsciaHeight() {return WeVisible?Wejscia->height():0;}

//---------------------------------------------------------------------------
//najpierw metody show.. ustalają rozmiary poszczególnych komponentów
//potem metody move przesuwają je w odpowiednie miejsca
void  atDevicePanel::Show() {

  showName();
  showWy();
  showErr();
  showParts();
  showWe();
  showDukt();
  //metody show ustaliły rozmiary poszczególnych elementów

  if(LabelName!=NULL){ LabelName->move(MargWidth+wejsciaWidth(), MargHeight );  }
  if(EditErr!=NULL){ EditErr->move(MargWidth+wejsciaWidth()+nameWidth(),MargHeight );    }
  if(EditWy!=NULL){ EditWy->move(MargWidth+wejsciaWidth()+nameWidth()+errWidth(),MargHeight );    }
  moveWe();
  moveParts();
  moveKladki();

  int w1 = MargWidth;
   w1+=wejsciaWidth();
   w1+=nameWidth();
   w1+=errWidth();
   w1+=wyWidth();
  int w2 = MargWidth;
   w2+=wejsciaWidth();
    w2+=szeregiWidth();
  int h1 = wyHeight();
   if(h1<wejsciaHeight()) h1=wejsciaHeight();//poprawić
  int h2 = partsHeight()  + kladkiHeight();
   if(h2<wejsciaHeight()) h2=wejsciaHeight();
  resize((w1>w2?w1:w2),h1+h2);
  //QSize sss=size();
  //QSize ssss=size();
}

//---------------------------------------------------------------------------
void atDevicePanel::Disp() {
 if(dev!=NULL) {
     if(LabelName!=NULL){
         LabelName->setStyleSheet(  dev->Enabled() ? ColorDeviceEnabled : ColorDeviceNotEnabled );
     }
     if(EditWy!=NULL){
         EditWy->setText(QString::number(dev->GetWy(),'g',(WyDec<0?6:WyDec) ));
         EditWy->setStyleSheet( (dev->GetWy()!=0) ? ColorWyOn : ColorWyOff );
     }
     if(EditErr!=NULL){
         EditErr->setText( dev->IsError() ? QString::number(dev->GetErrorCode()) :"" );
         EditErr->setStyleSheet( dev->IsError() ? ColorErrorOn : ColorErrorOff );
     }
     for(int i=0; i<SzeregiSize(); i++){   Szereg(i)->Disp();  }
     if(Wejscia!=NULL)Wejscia->Disp();
     if(form!=NULL) form->Disp();
 }
}


//---------------------------------------------------------------------------
void   atDevicePanel::paintEvent(QPaintEvent* p){
    QFrame::paintEvent(p);
    QPainter painter(this);
    QPen pen(QColor(0, 0, 0, 127));
    pen.setWidth(2);
    painter.setPen(pen);
    for(int i=0;i<SzeregiSize();i++){
        for(int j=0;j<Szereg(i)->DuktySize();j++){
            atDukt *d=Szereg(i)->Dukt(j);
            if( !d->Up() ) { //tylko dukty Dn
                pen.setColor(QColor(255, 0, 0, 127));
                painter.setPen(pen);
                painter.drawLine(d->PosLeft(),d->PosWy(),d->PosCenter(),d->PosWy()); //Wy
            }
            int pC=d->PosCenter();
            int pT=d->PosTop();
            int pB=d->PosBottom();
            painter.drawLine(d->PosCenter(),d->PosTop(),d->PosCenter(),d->PosBottom()); //Dukt
            for(int k=0;k<d->WeSize();k++){
                pen.setColor(QColor(0, 55, 55, 127));
                painter.setPen(pen);
                painter.drawLine(d->PosCenter(),d->PosWe(k),d->PosRight(k),d->PosWe(k)); //We
            }
        }
    }
    for(int i=0;i<KladkiSize();i++){
        pen.setColor(QColor(255, 0, 255, 127));
        painter.setPen(pen);

        painter.drawLine(Kladka(i)->PosLeft(),Kladka(i)->PosTop(),Kladka(i)->PosRight(),Kladka(i)->PosTop());
    }
}

//---------------------------------------------------------------------------
void   atDevicePanel::ShowForm(){ if(form==NULL) form=new atDeviceForm(NULL,dev);  form->Show();}
//---------------------------------------------------------------------------
void   atDevicePanel::ShowErr(){  if(dev->GetError()!=NULL) QMessageBox::warning(NULL,"Błąd!",dev->GetErrorOpis(),QMessageBox::Ok, QMessageBox::Ok); }
//---------------------------------------------------------------------------
void   atDevicePanel::CloseForm(){
  for(int i=0; i<SzeregiSize(); i++){  Szereg(i)->CloseForms();    }
  //for(int i=0; i<PartPanels.size(); i++){  PartPanels.at(i)->CloseForm();    }
  if(form!=NULL){      form->ClosePanel();      delete form;      form=NULL;   }
}
//---------------------------------------------------------------------------
int  atDevicePanel::PosWy(int i){  return MargHeight+i*5;}
//---------------------------------------------------------------------------
int  atDevicePanel::PosWe(int i){  return MargHeight+i*5;}
//---------------------------------------------------------------------------
void atDevicePanel::addKladka(atKladka *kl){  Kladki.push_back(kl);  kl->setNo(Kladki.size()-1); }

//===========================================================================
//---------------------------------------------------------------------------
int atKladka::setLeft(){
  int min=-1;
  for(int i;i<DuktySize();i++){ if(min==-1 || Dukt(i)->PosCenter()<min) min=Dukt(i)->PosCenter();}
  return min;
}
//---------------------------------------------------------------------------
int atKladka::setRight(){
  int max=-1;
  for(int i;i<DuktySize();i++){ if(max==-1 || Dukt(i)->PosCenter()>max) max=Dukt(i)->PosCenter();}
  return max;
}
//===========================================================================
//---------------------------------------------------------------------------
int atDukt::PosTop(){
  int min=-1;
  if(dpWy!=NULL && !up) min=PosWy();
  if(kl!=NULL) if(min==-1 || PosKl()<min) min=PosKl() ;
  for(int i=0;i<dpWe.size();i++){if(min==-1 || PosWe(i)<min) min=PosWe(i);}
  return min;
}
//---------------------------------------------------------------------------
int atDukt::PosBottom(){
  int max=-1;
  if(dpWy!=NULL && !up) max=PosWy();
  if(kl!=NULL) if(max==-1 || PosKl()>max) max=PosKl() ;
  for(int i=0;i<dpWe.size();i++){if(max==-1 || PosWe(i)>max) max=PosWe(i);}
  return max;
}
//---------------------------------------------------------------------------
int atDukt::PosKl(){ return kl->pos().y(); }

//===========================================================================
//---------------------------------------------------------------------------
atSzereg::~atSzereg(void){
  while(du.size()>0){delete du.at(du.size()-1); du.pop_back();}
}
//---------------------------------------------------------------------------
int atSzereg::DuktyWidth(){
  int szer=0;
  for(int i;i<DuktySize();i++) szer+=Dukt(i)->Width();
  return szer;
}
//---------------------------------------------------------------------------
void atSzereg::addPanel(atDevicePanel *d){
  PPanels.append(d);
  if(d->totalWidth()>width()) setWidth(d->totalWidth());
  setHeight(height()+d->totalHeight()+MargHeight);
}
//---------------------------------------------------------------------------
// dodaje dukt do naszego szeregu na podstawie wyjścia z panelu dp
// panel nalezy do szeregu poprzedniego (z definicji seregów) i ma wejścia do naszego szeregu
// jeśli panel ma tez wejścia do innych szeregów, to dodajemy do tych szeregów dukty połączone kładka z naszym
void atSzereg::addDukt(atDevicePanel *dp, int nWy){
  atDukt *d=new atDukt(dp, nWy, false,0,4);
  addDukt(d);
  //sprawdzenie połączeń w naszym szeregu
  for(int j=0; j<GetPanelSize(); j++){
      for(int k=0;k<GetPanel(j)->WeSize();k++){
          if(dp->Wy(nWy)==GetPanel(j)->We(k)) {d->addWe(GetPanel(j),k);}
      }
  }
  //sprawdzenie połączeń w innych szeregach
  atKladka *kl=NULL;
  for(int i=sz-1; i>=0; i--){
      atDukt *d1=NULL;
      for(int j=0; j<Szereg(i)->GetPanelSize(); j++){
          for(int k=0;k<Szereg(i)->GetPanel(j)->WeSize();k++){
              if(dp->Wy(nWy)==Szereg(i)->GetPanel(j)->We(k)) {

                  if(d1==NULL){ d1=new atDukt(dp,nWy,true,0,4); Szereg(i)->addDukt(d1);  }
                  d1->addWe(Szereg(i)->GetPanel(j),k);
                  if(kl==NULL){ kl=new atKladka(parentPanel); kl->addDukt(d);  }
                  kl->addDukt(d1);

              }
          }
      }
  }
}

//---------------------------------------------------------------------------
void atSzereg::moveSzereg(int top, int left, int mh, int mw) {
  int w=left;
  int h=top;
  for(int i=0; i<DuktySize(); i++){
      Dukt(i)->setPos(w);
      w+=Dukt(i)->Width()+mw;
  }
  for(int i=0; i<GetPanelSize(); i++){
        GetPanel(i)->move(w,h);
        h+=GetPanel(i)->totalHeight()+mh;
  }
}
//---------------------------------------------------------------------------
void atSzereg::Disp(){  for(int i=0; i<GetPanelSize(); i++) GetPanel(i)->Disp();  }
//---------------------------------------------------------------------------
void atSzereg::CloseForms(){  for(int i=0; i<GetPanelSize(); i++) GetPanel(i)->CloseForm();  }
