/*
by AT
*/
#include "atFile.h"

#include <QFileDialog>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDir>
#include <QStringList>
#include <QMessageBox>
#include <iostream>


//--------------------------------------------------------
//Metody do operacji na pliku
//--------------------------------------------------------
// inicjuje nazwę pliku
void atFile::Ini(QString P,QString F,QString E){
    Filename = F;
    Fileext = E;
    Filepath = P;
    Filetext = "";
    FileAct=FindName(Filepath,Filename,Fileext);
}
//--------------------------------------------------------
// wczytuje do bufora z pliku o wskazanej nazwie
QString atFile::PobierzPlik(QString FN){
    QFile inputFile(FN);
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    Filetext = in.readAll();
    inputFile.close();
    return Filetext;
}
//--------------------------------------------------------
//wczytuje do bufora z plku o nawie podanej w parametrach inicjacji
QString atFile::PobierzPlik(){
    QFileInfo fi(Filepath,Filename+Fileext);
    return PobierzPlik(fi.absoluteFilePath());
}
//--------------------------------------------------------
//zapisuje plik pod wskazaną nazwą
void atFile::ZapiszPlik(QString FN){
    QFile inputFile(FN);
    if (!inputFile.open(QIODevice::Append | QIODevice::Text)) return;
    QTextStream out(&inputFile);
    out << Filetext;
    inputFile.close();
}
//--------------------------------------------------------
//zapisuje plik pod nazwą rozszerzoną
void atFile::ZapiszPlik(){
    QFileInfo fi(Filepath,FileAct+Fileext);
    ZapiszPlik(fi.absoluteFilePath());
    Filetext = "";
}
//--------------------------------------------------------
//zapisuje plik pod nazwą parametrową w trybie "bak"
void atFile::ZapiszPlikBak(QString Ext){
  QFileInfo fib(Filepath,Filename+Ext);
  QFile fB(fib.absoluteFilePath());
  if(  fB.exists() )fB.remove();

  QFileInfo fi(Filepath,Filename+Fileext);
  QFile f(fi.absoluteFilePath());
  if(!f.exists() ||  f.rename(fib.absoluteFilePath()) ) ZapiszPlik(fi.absoluteFilePath());
}
//--------------------------------------------------------
//znajduje nową nazwę dla pliku
QString atFile::FindName(QString P,QString F,QString Ext){
   QString FN;
   QFile f;
   int i=1;
   do{
    FN = F+"_"+QString::number(i++);
    QFileInfo fi(P,FN+Ext);
    f.setFileName(fi.absoluteFilePath());
   } while ( f.exists() );
   return FN;
}
//---------------------------------------------------------------------
// prymitywna funkcja czytania
QString atFile::getAfter(QString s,QString tag){
    int i=s.indexOf(tag);
    if(i<0){ return "";}
    return s.mid(i+tag.length());
}
//---------------------------------------------------------------------
// prymitywna funkcja czytania
QString atFile::getBefore(QString s,QString tag){
    int i=s.indexOf(tag);
    if(i<0){ return "";}
    return s.mid(0,i);
}
//---------------------------------------------------------------------
// prymitywna funkcja czytania
QString atFile::getPole(QString s,QString beg, QString end){
    int i=s.indexOf(beg);
    if(i<0){ return "";}
    s=s.mid(i+beg.length());
    i=s.indexOf(end);
    if(i<0){ return "";}
    return s.mid(0,i);
}

