#ifndef ATFILESCAD_H
#define ATFILESCAD_H

#include <QString>
#include "atFile.h"
#include <QVector>
#include "atConvert.h"


//----------------------------------------------------------------
//errors:
#define ERROR_NAWIAS_ONE "nawiasOne: bład otwarcia "
#define ERROR_NAWIAS_ONE_END "nawiasOne: bład zamknięcia "
#define ERROR_NAWIAS_REQ "nawias: bład otwarcia "
#define ERROR_NAWIAS_REQ_END "nawias: bład zamknięcia "
#define ERROR_SEPARATOR_REQ "separator: błąd otwarcia"
#define ERROR_SEPARATOR_REQ_END "separator: błąd zamknięcia"
#define ERROR_PROC_NOTIS_BLACK "proc: Powinien być znak"
#define ERROR_PROC_OPERATOR_NEEDED "proc: oczekiwany operator"
#define ERROR_PROCUSE_OPENBRACKET "procUse: oczekiwany nawias otwierający <"
#define ERROR_PROCINCLUDE_OPENBRACKET "procUse: oczekiwany nawias otwierający <"
#define ERROR_PROC_COMMAND "proc: nierozpoznane słowo"
#define ERROR_PROCGOMODULE_SEMICOLON "procGoModule: oczekiwany średnik"
#define ERROR_PROC_UNEXPECTED_END "proc: niespodziewany koniec pliku"
#define ERROR_ZMIENNA_SYNTAX "zmienna: błąd składni "
#define ERROR_PROCVALUE_NOTIS_BLACK "procVaue Powinien być znak lub operator"
#define ERROR_EQUSIGN_ONE "procZmienna: oczekiwany ="
#define ERROR_PROCVALUE_SYNTAX "procValue: złą składnia lub nie ma zmiennej"
#define ERROR_PROCVALUE_DEVBYZERO "procValue: dzielenie przez zero"
#define ERROR_PROCVALUE_OPERATOR "procValue: zły operator"
#define ERROR_PROCFUNKCJA_BRAK "procFunkcja: nie ma funkcji "
#define ERROR_INCLUDE "procInclude: błąd w pliku "
#define ERROR_USE "procUse: błąd w pliku "
#define ERROR_PROCVALUE_ONE "procValue_One: oczekiwana wartosć "
#define ERROR_PROCVALUE_GETVALUE "getValue: "
#define ERROR_PROCBODY_NOTIS_BLACK "procBody: not is black"
#define ERROR_PROCBODY_UNEXPECTED_END "procBody: niespodziewany okniec"
#define ERROR_PROCBODY_DKONCZYCPROGRAM "procBody: niedokończony program.."
#define ERROR_PROCBODY_OPERATOR_NEEDED "procBody: optrzebny operator"
#define ERROR_PROCBODYMODULE_SEMICOLON "procBodyModule: spodziewany średnik"
#define ERROR_PROCBODYMODULE_EQUSIGN "procBodyZmienna: sodziewane podstawienie"

//----------------------------------------------------------------------
class atScadModule;
class atConvert;

//----------------------------------------------------------------------
class atFileScad {
private:
    QString Filetext;
    QString path;
    QString file;  //plik w którym operujemy

    atConvert* convert;
    atScadModule* module;
    bool tryb_inc;  //true=include, false=use=tylko odczytuje moduły
    int position,line,linepos; //pozycja kursora dla pętli przeszukujących.

    QString error;
    int eposition,eline,elinepos; //parametry błędu.


public:
    atFileScad() ;
    atFileScad(QString t,QString p, atConvert* c,bool i,QString f) ;
    atFileScad(atScadModule* m,QString p, atConvert* c,bool i,QString f);
    atFileScad(atScadModule* m,QString t,QString p, atConvert* c,bool i,QString f);
    void reset(){    eposition=0;eline=0;elinepos=0; position=0; line=0; linepos=0;}

    void setError(QString e){ error=e;eposition=position;eline=line;elinepos=linepos; }
    QString getError(){return QString::number(eline)+"/"+QString::number(elinepos)+"/"+QString::number(eposition)+":"+error;}
    bool ok(){return error=="";}

    bool isWhite();
    bool isComment();
    bool isBlack();
    bool isOperator();

    void nextPosition();
    void goToNotWhite();

    QString nawias(QChar c1,QChar c2);
    QString separator(QChar c2) ;
    QString goToNotBlack();
    QString slowoGet();

    void go();

    bool procMain();
    bool procUse();
    bool procInclude();
    bool procModule();

    bool procGoModule(QString s);
    bool procGoTranslate(QString s);
    bool procGoRotate(QString s);
    bool procGoZmienna(QString s);

    double procValue(QChar cEnd);
    double procValueOne();
    double procValueMno();
    double procFunkcja(QString f, double v);
    double getValue(QString s);

    bool procBody( );
    bool procBodyModule( QString s );
    bool procBodyZmienna(QString s);
};

#endif // ATFILESCAD_H
