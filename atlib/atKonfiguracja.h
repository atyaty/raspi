/*
by AT
*/

#ifndef atKonfiguracja_H
#define atKonfiguracja_H

#include <QString>
#include "atFile.h"

#define atKonfiguracja_ERROR_pole_not_found    1
#define atKonfiguracja_ERROR_pole_corupted     2
#define atKonfiguracja_ERROR_double_format     3
#define atKonfiguracja_ERROR_int_format        4

class atKonfiguracja :public atFile {
private:

   //XML-odczyt
   QString getTag(QString Text,QString NazwaPola);
   QString Odczytaj(QString Text,QString NazwaPola);
   double OdczytajFloat(QString Text,QString NazwaPola);
   double OdczytajFloat(QString Text,QString NazwaPola,int n);
   int OdczytajInt(QString Text,QString NazwaPola);
   int OdczytajInt(QString Text,QString NazwaPola,int n);

   int PoliczFields(QString Tekst);
   QString OdczytajField(QString Tekst,int n); //odczytaj n-te pole
   QString OdczytajFieldNaz(QString Tekst,int n);
   int PoliczSubFields(QString Tekst,QString NazwaPola){  return PoliczFields( getTag(Tekst,NazwaPola) ); }
   QString OdczytajSubField(QString Tekst,QString NazwaPola,int n){  return OdczytajField(getTag(Tekst,NazwaPola),n); }
   QString OdczytajSubFieldNaz(QString Tekst,QString NazwaPola,int n){  return OdczytajFieldNaz(getTag(Tekst,NazwaPola),n); }

   //XML-zapis
   QString setPole(QString Text,QString beg,QString end,QString Value);
   QString setTag(QString Text,QString NazwaPola,QString Value);
   QString Zapisz(QString Text,QString NazwaPola,QString Value);
   QString Zapisz(QString Text,QString NazwaPola,double Value);
   QString Zapisz(QString Text,QString NazwaPola,int Value);

   int Error;


public:
   atKonfiguracja(QString Pp,QString Ff,QString Ee){ Ini(Pp,Ff,Ee);}
   atKonfiguracja(QString Pp,QString Ff){ Ini(Pp,Ff,"");}
   atKonfiguracja(QString Ff){ Ini("",Ff,"");}

   bool Ini(QString Pp,QString Ff,QString Ee);

   //XML-odczyt
   QString getTag(QString NazwaPola)       {return Odczytaj(Filetext, NazwaPola);}
   double getFloat(QString NazwaPola)      {return OdczytajFloat(Filetext, NazwaPola);}
   double getFloat(QString NazwaPola,int n){return OdczytajFloat(Filetext, NazwaPola,n);}
   int getInt(QString NazwaPola)           {return OdczytajInt(Filetext, NazwaPola);}
   int getInt(QString NazwaPola, int n)    {return OdczytajInt(Filetext, NazwaPola,n);}

   int getPola(QString pole)               {return PoliczSubFields(Filetext,pole);}
   int getPola()                           {return PoliczFields(Filetext);}
   QString getTag(QString pole,int n)   {return OdczytajSubField(Filetext,pole,n);}
   QString getTag(int n)                {return OdczytajField(Filetext,n);}
   QString getTagNaz(QString pole,int n){return OdczytajSubFieldNaz(Filetext,pole,n);}
   QString getTagNaz(int n)             {return OdczytajFieldNaz(Filetext,n);}

   //XML-zapis
   void setTag(QString NazwaPola,QString Value){Filetext = Zapisz(Filetext,NazwaPola,Value);}
   void setFloat(QString NazwaPola,double Value) {Filetext = Zapisz(Filetext,NazwaPola,Value);}
   void setInt(QString NazwaPola,int Value)    {Filetext = Zapisz(Filetext,NazwaPola,Value);}

   int getError(){return Error;}
   void ResetError(){Error=0;}

};

#endif
