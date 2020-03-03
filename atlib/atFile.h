/*
by AT
*/

#ifndef atFile_H
#define atFile_H

#include <QString>

class atFile {
protected:
    QString Filename;    //nazwa pliku
    QString Fileext;     //rozszerzenie
    QString Filepath;    //ścieżka
    QString FileAct;     //nazwa pliku właściwa

    QString Filetext;    //zawartosc pliku

    QString getAfter(QString Text,QString tag); //funkcja czytająca z pliku
    QString getBefore(QString Text,QString tag); //funkcja czytająca z pliku
    QString getPole(QString Text,QString beg,QString end); //funkcja czytająca z pliku


public:
    atFile(){}
    atFile(QString P, QString F, QString E){Ini(P,F,E);}
    void Ini(QString P, QString F, QString E);

    QString PobierzPlik();
    QString PobierzPlik( QString FN);

    void ZapiszPlik();
    void ZapiszPlik(QString Ff);
    void ZapiszPlikBak(QString Ext);

    QString FindName(QString P,QString Ff,QString Ext);

    QString FileNameFull(){return Filename+Fileext;}
    QString FileNameBase(){return Filename;}
    QString FileNameAct(){return FileAct;}
    QString FileNameActFull(){return FileAct+Fileext;}
    QString FileExt(){return Fileext;}
    QString FilePath(){return Filepath;}
    QString FileText(){return Filetext;}

    void Append(QString s){Filetext+=s;}
    void AppendLn(QString s){Filetext+=s+"\n";}
    bool isEmpty(){return Filetext.isEmpty();}
    void Clear(){Filetext="";}

    QString getAfter(QString tag) {return getAfter(Filetext,tag);}
    QString getBefore(QString tag) {return getBefore(Filetext,tag);}
    QString getPole(QString b,QString e)  {return getPole(Filetext, b, e );}



};

#endif
