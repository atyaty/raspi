/*
by AT
*/

#ifndef atErrorH
#define atErrorH


#include <string>
#include <atFunctions.h>

using namespace std;

//---------------------------------------------------------------------------
class atError{
 protected:
  int ErrCode;
  string ErrOpis;

public:
  atError()                     { ErrCode=0; ErrOpis="";}
  atError(int c, string e)      { ErrCode=c; ErrOpis=e;}
  void SetError(int c, string e){ ErrCode=c; ErrOpis=e;}
  void SetError(int c)          { ErrCode=c; ErrOpis="";}

  int GetCode(){return ErrCode;}
  string GetOpis(){return ErrOpis;}
  string Opis(){return "[0x"+IntToHex(ErrCode)+"] "+ErrOpis;}
  bool IsEmpty(){return ErrCode==0;}
};




#endif
