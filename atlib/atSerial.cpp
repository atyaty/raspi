#include "atSerial.h"

//----------------------------------------------------------------------
atSerial::atSerial(atDataBuffer *dr,atDataBuffer *dw, std::string h ){
   dbr=dr;
   dbw=dw;
   fd = open (h.c_str(), O_RDWR);

}
//--------------------------------------------------
void atSerial::go() {

    //if(sError!="") return;
   int n;

    //  sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //if (sockfd < 0)  sError="Socket error";
    if(fd==-1) return ;
    n = write(fd,dbw->getFrame(),dbw->getDataLen());
    if (n < 0){
      //sError="Write error";
      return;
    }
    dbr->Clear();
    n = read(fd,dbr->getFrame(),dbr->getFrameLen());
    if (n < 0){
      //sError="Read error";
      return ;
    }
    dbr->setDataLen(n);
}

//-------------------------------------------------------------------------
