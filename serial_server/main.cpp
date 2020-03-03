
/*
komunikacja UART bez użucia bibliotek QT
model: klien-swever (to jest server)
odpowiedź zależy od zapytania przychodzącego od klienta i zależy od tego pytania
*/
#include <iostream>

#include <atSerial.h>
#include <atGetch.h>
#include <atDataBuffer.h>
#include <sstream>
#include <at_X3.h>
#include <at_X3Error.h>
#include <time.h>
#include <bcm2835.h>


using namespace std;

//---------------------------------------------------------
string charToHex(uint8_t* f,unsigned len){
    stringstream ss;
    for(unsigned i=0;i<len;i++){
     ss << std::hex << (unsigned int) f[i];
     ss << " ";
    }
    return ss.str();

}
//---------------------------------------------------------
string charToText(uint8_t* f,unsigned len){
    string c;
    for(unsigned i=0;i<len;i++){
     if(f[i]>=32 && f[i]<=126) c+=f[i];
     else c+=" ["+to_string(f[i])+"] " ;

    }
    return c;

}


//-----------------------------------------------------------
int main(int argc, char *argv[])
{

    cout << "serial listener started..." << endl;


    atGetch *g= new atGetch(1);
    atSerial* ats = new atSerial( AT_SERIAL_TTY, AT_SERIAL_BAUD, AT_SERIAL_TIMEOUT );
    atDataBuffer *dbr =new atDataBuffer(AT_MODBUS_ID_PI, AT_MODBUS_FRAME_LEN,AT_MODBUS_RESPNAG);
    atDataBuffer *dbw =new atDataBuffer(AT_MODBUS_ID_PI, AT_MODBUS_FRAME_LEN,AT_MODBUS_REQUNAG);

    struct timespec t,t0;
    clock_gettime(CLOCK_REALTIME,&t0);

    string s="";
    while( true ){

        //--------------------------------------
        //obsługa komunikacji szeregowej
        ats->gets(dbr);
        if( dbr->getDataLen() ){
            clock_gettime(CLOCK_REALTIME,&t);

            cout << "\nDataLen: ";
            cout << dbr->getDataLen();
            cout << "\ntime s: ";
            cout << t.tv_sec-t0.tv_sec;
            cout << "ns: ";
            cout << t.tv_nsec-t0.tv_nsec;
            cout << " read (HEX): ";
            cout << charToHex( dbr->getFrame(), dbr->getDataLen() );
            cout << "\n";
            cout << charToText( dbr->getFrame(), dbr->getDataLen() );

            t0.tv_sec=t.tv_sec;
            t0.tv_nsec=t.tv_nsec;

            if(!dbr->checkId(AT_REGADRES_SMM_KAPPA)){
                cout << " złe id";
            }
            else if( !dbr->checkCRC() ){
                dbw->setError( dbr, AT_ERROR_MODBUS_CRC );
                cout << " złe crc";
            }
            else{
              //  unsigned lenBajt = dbr->getReg(AT_MODBUS_FRAME_DATALEN);
                //if(dbr->checkFunction(AT_SMM_WRITE)){
                    dbw->buforSetNag( dbr );
                    dbw->buforAddReg(0);
                 //   for( unsigned i=0; i<lenBajt; i++ ){ dbw->buforAddByte( dbr->getByte(AT_MODBUS_REQUNAG+i)+1 );}
                    dbw->buforAddCRC();
                    cout << "    ok! ";


                //}
                //else{
                  //  dbw->setError(dbr, AT_ERROR_SMM_FUNCTION);
                    //cout << " zła funkcja ";
               // }
            }
            ats->puts(dbw);

            cout << "  dbw: (";
            cout << dbw->getDataLen() ;
            cout << ") ";
            cout << charToHex( dbw->getFrame(), dbw->getDataLen() );
            cout << "\n";

            dbr->Clear();
            dbw->Clear();
        }

        //--------------------------------------
        //obsługa klawiszy
        char c=g->get_char();
        if(c!=0){
          s+=c;
          if(c=='\n'){
            if(s=="exit\n"){  break;  }
            s="";
          }
        }
        //---------------------------------------
    }
    cout << "serial listener stpped..\n" ;
    delete g;
    delete ats;
    delete dbr;
    delete dbw;

    return 0;
}
