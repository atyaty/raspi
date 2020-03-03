#include "atSerial.h"

//----------------------------------------------------------------------
atSerial::atSerial( const char* d, const int b, long int t ){
    timeout =t;
    fd = open (d, O_RDWR);

    //if ( fd>=0 && wiringPiSetup() == -1)  { serialClose(fd); fd = -1; }
}

//-------------------------------------------------------------------------
//void atSerial::puts(string s){

  //  for (unsigned i = 0 ; i<s.length() ; i++ ) {
    //     write(fd, s[i]) ;
    //}
    //serialPutchar (fd,'\n');

//}
//-------------------------------------------------------------------------
void atSerial::puts(atDataBuffer *d){
    int ret = write(fd,d->getFrame(),d->getDataLen());

}
//-----------------------------------------------------------------------
//string atSerial::gets(){
  //  string s;
    //while ( serialDataAvail(fd) )   { s+=(serialGetchar (fd));    }
    //return s;
//}
//-----------------------------------------------------------------------
void atSerial::gets(atDataBuffer *d){
    int ret = read(fd,d->getFrame(),d->getFrameLen());
    if(ret<0) d->setDataLen(0);
    else d->setDataLen(ret);
}

