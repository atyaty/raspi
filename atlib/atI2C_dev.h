#ifndef ATI2C_DEV_H
#define ATI2C_DEV_H

#include <atDataBuffer.h>
//#include <atI2C.h>
#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
//#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <atFunctions.h>
#include <atErrors.h>
#include <atError.h>
#include <atFileLog.h>


//-------------------------------------------------------------
class atI2C;
typedef atI2C atI2C;
//-------------------------------------------------------------
class atI2C_dev{

protected:

    string Name;                                /* Nazwa nadana*/
    atI2C* dev;
    atDataBuffer* dbr;
    atDataBuffer* dbw;
    atError Error;
    atFileLog *FileRap;        //obiekt loga
    void Rap(string s,int l){ if(FileRap!=NULL ) FileRap->Rap(s,l); }
public:
    atI2C_dev(string n);
    atI2C_dev(string n,atDataBuffer*dr,atDataBuffer*dw);
    virtual ~atI2C_dev(){}
    void SetupDev( atI2C* d );
    void confBuffers(atDataBuffer*dr,atDataBuffer*dw){dbr=dr;dbw=dw;}
    atDataBuffer* getDbr(){ return dbr; }
    atDataBuffer* getDbw(){ return dbw; }
    //int getId(){return idDev;}
    string getName(){return Name;}

    void SetError(int c);
    void SetError(int c,string e);
    int getError(){ return Error.GetCode(); }
    string getErrorOpis(){ return Error.Opis(); }
    void confRap(atFileLog *f){FileRap=f;}


    virtual bool Init(){return true;} //komendy inicjujące konkretnego urządzenia
    virtual bool go();    //
    //virtual bool go_read();    //
    //virtual bool go_write();    //

};

#endif // ATI2C_DEV_H
