#include "atI2C.h"

//-----------------------------------------------------------
atI2C::atI2C(int id){ idDev=id;   FileRap=NULL;   }
//-----------------------------------------------------------
bool atI2C::Init(){
    if(!bcm2835_init()){      Error.SetError(ATERROR_I2C_INIT,"I2C Init"); return false;}
    if(!bcm2835_i2c_begin()){ Error.SetError(ATERROR_I2C_BEGIN,">I2C Begin"); return false;}

    for(unsigned i=0; i<devs.size(); i++){
        devs.at(i)->Init();
        if(devs.at(i)->getError()){  Error.SetError(ATERROR_I2C_DEV_OPEN, "I2C Device");    }
    }
    return Error.IsEmpty();
}
//-----------------------------------------------------------
void atI2C::addDev(atI2C_dev* d){
    devs.push_back(d);
    d->SetupDev( this );
}
//-----------------------------------------------------------
void atI2C::go() {
   for(unsigned i=0; i<devs.size(); i++){ devs.at(i)->go(); }
}
//-----------------------------------------------------------
void atI2C::go(unsigned i) {    devs.at(i)->go();  }
//-----------------------------------------------------------
string atI2C::getErrorOpis(){
    string s="";
    if(!Error.IsEmpty()) s+=Error.Opis();
    for(unsigned i=0; i<devs.size(); i++){
      if(devs.at(i)->getError()){  s+=" ("+devs.at(i)->getName()+": "+devs.at(i)->getErrorOpis()+")";    }
    }
    return s;
}
//--------------------------------------------------------------------------
int atI2C::setParams(at_Konfiguracja* cnf){
//    LevelRap=cnf->getInt("LevelRap");    if(cnf->getError()){ LevelRap=0;}
    return 0;
}



