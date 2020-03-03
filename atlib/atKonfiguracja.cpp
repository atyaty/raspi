/*
by AT
*/
#include "atKonfiguracja.h"

#include <QFileDialog>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDir>
#include <QStringList>
#include <QMessageBox>


//--------------------------------------------------------
//Metody do operacji na pliku
//--------------------------------------------------------
bool atKonfiguracja::Ini(QString Pp,QString Ff,QString Ee){
   atFile::Ini(Pp,Ff,Ee);
   PobierzPlik();
   return !Filetext.isEmpty();
}



//---------------------------------------------------------------------
QString atKonfiguracja::getTag(QString s,QString tag){
    return getPole(s,"<"+tag+">","</"+tag+">");
}

//---------------------------------------------------------------------
QString atKonfiguracja::Odczytaj(QString Text,QString NazwaPola)
{
  QString SubText;
  int pos = NazwaPola.indexOf(".");
  if(pos >= 0 ){
      SubText=getTag(Text,NazwaPola.mid(0,pos));
      return Odczytaj(SubText,NazwaPola.mid(pos+1));
  }
  return getTag(Text,NazwaPola);
}
//---------------------------------------------------------------------
double atKonfiguracja::OdczytajFloat(QString Text,QString NazwaPola)
{
  ResetError();
  QString sub_text=Odczytaj(Text,NazwaPola);
  bool ok;
  double ret_war = sub_text.toFloat(&ok);
  if(!ok) Error=atKonfiguracja_ERROR_double_format;
  return ret_war;
}
//---------------------------------------------------------------------
int atKonfiguracja::OdczytajInt(QString Text,QString NazwaPola)
{
    ResetError();
    QString sub_text=Odczytaj(Text,NazwaPola);
    bool ok;
    int ret_war = sub_text.toInt(&ok);
    if(!ok) Error=atKonfiguracja_ERROR_int_format;
    return ret_war;
}
//---------------------------------------------------------------------
double atKonfiguracja::OdczytajFloat(QString Text,QString NazwaPola,int n)
{
  ResetError();
  QString sub_text=OdczytajSubField(Text,NazwaPola,n);
  bool ok;
  double ret_war = sub_text.toFloat(&ok);
  if(!ok) Error=atKonfiguracja_ERROR_double_format;
  return ret_war;
}
//---------------------------------------------------------------------
int atKonfiguracja::OdczytajInt(QString Text,QString NazwaPola,int n)
{
    ResetError();
    QString sub_text=OdczytajSubField(Text,NazwaPola,n);
    bool ok;
    int ret_war = sub_text.toInt(&ok);
    if(!ok) Error=atKonfiguracja_ERROR_int_format;
    return ret_war;
}
//---------------------------------------------------------------------




//---------------------------------------------------------------------
//XML
//---------------------------------------------------------------------
QString atKonfiguracja::setPole(QString Tekst,QString beg, QString end, QString Value ){
    QString sub_text = "";
    int pos0= Tekst.indexOf(beg);
    int pos1= Tekst.indexOf(end);
    if(pos0 >= 0 && pos1>pos0){
        return Tekst.mid(0,pos0+beg.length()) + Value + Tekst.mid(pos1);
    }
    else{
        return Tekst+"\n"+beg+Value+end;
    }
}
//---------------------------------------------------------------------
QString atKonfiguracja::setTag(QString Tekst,QString NazwaPola,QString Value){
    return setPole(Tekst,"<"+NazwaPola+">","</"+NazwaPola+">",Value);
}
//---------------------------------------------------------------------
QString atKonfiguracja::Zapisz(QString Text,QString NazwaPola,QString Value){
  int pos = NazwaPola.indexOf(".");
  if( pos >= 0 ){
      QString N1 = NazwaPola.mid(0,pos);
      QString SubText=getTag(Text,N1);
      QString N2 = NazwaPola.mid(pos+1);
      return setTag(Text,N1,Zapisz(SubText,N2,Value));
  }
  return setTag(Text,NazwaPola,Value);
}
//---------------------------------------------------------------------
QString atKonfiguracja::Zapisz(QString Text,QString NazwaPola,double Value){
  return Zapisz(Text,NazwaPola,QString::number(Value));
}
//---------------------------------------------------------------------
QString atKonfiguracja::Zapisz(QString Text,QString NazwaPola,int Value){
  return Zapisz(Text,NazwaPola,QString::number(Value));
}




//---------------------------------------------------------------------
//funkcje do zliczania pól
//---------------------------------------------------------------------
int atKonfiguracja::PoliczFields(QString Tekst){
  QString NazwaPola=getPole(Tekst,"<",">");
  if(NazwaPola=="") return 0;
  return 1 + PoliczFields( getAfter(Tekst,"</"+NazwaPola+">") );
}
//---------------------------------------------------------------------
//odczytaj pole o podanym numerze
QString atKonfiguracja::OdczytajField(QString Tekst,int n){
  QString NazwaPola=getPole(Tekst,"<",">");
  if(NazwaPola=="") return "";
  if( n>0) return OdczytajField(getAfter(Tekst,"</"+NazwaPola+">"),n-1);
  return getTag(Tekst,NazwaPola);
}
//---------------------------------------------------------------------
//odczytaj nazwę pola o podanym numerze
QString atKonfiguracja::OdczytajFieldNaz(QString Tekst,int n){
  QString NazwaPola=getPole(Tekst,"<",">");
  if(NazwaPola=="") return "";
  if( getAfter(Tekst,"<"+NazwaPola+">").indexOf("</"+NazwaPola+">")<0 ) return "";
  if( n>0) return OdczytajFieldNaz(getAfter(Tekst,"</"+NazwaPola+">"),n-1);
  else return NazwaPola;
}




