#ifndef ATI2C_H
#define ATI2C_H

#include <atI2C_dev.h>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <bcm2835.h>
#include <atError.h>
#include <atErrors.h>
#include <atFileLog.h>
#include <at_Konfiguracja.h>

using namespace std;

//---------------------------------------
class atI2C_dev;
//---------------------------------------
class atI2C {
private:
    int idDev;                 //identyfikator modułu I2C wewnątrz PI'a
    vector <atI2C_dev*> devs;  //tablica urządzeń
    atError Error;             //objekt błędu
    atFileLog *FileRap;        //obiekt loga
    void Rap(string s,int l){ if(FileRap!=NULL ) FileRap->Rap(s,l); }
public:
    atI2C(int id);
    void addDev(atI2C_dev* d);
    int setParams(at_Konfiguracja* cnf);
    bool Init();
    void go();
    void go(unsigned i);
    int getId(){ return idDev; }

    bool IsError(){ return !Error.IsEmpty(); }
    string getErrorOpis();

    void confRap(atFileLog *f){FileRap=f;}
};

#endif // ATI2C_H
