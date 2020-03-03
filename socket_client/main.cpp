#include <iostream>
#include <stdio.h>
#include <atSocketClient.h>
#include <atDataBuffer.h>
#include <cfg.h>
#include <at_X3.h>
#include <at_X3Error.h>

//--------------------------------------------------------
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
//--------------------------------------------------------
int main(int argc, char *argv[])  {
    char buf[256];
    char hostname[256];
    strcpy(hostname,"192.168.2.201");
    cout<<"Tu socket client:\n";

    atDataBuffer* dbr     = new atDataBuffer( AT_MODBUS_ID, AT_MODBUS_FRAME_LEN );
    atDataBuffer* dbw     = new atDataBuffer( AT_MODBUS_ID, AT_MODBUS_FRAME_LEN );
    atSocketClient * atSC = new atSocketClient( dbr,dbw,hostname,5000);
    if( atSC->isError() ){ cout<<atSC->Error(); cout<<"\n"; }

    while( !atSC->isError() )  {

        //--------------------------czytamy z klawiatury tekst do wysłania
        bzero(buf,256);
        fgets(buf,255,stdin);
        if( !strcmp(buf,"bye\n") ){cout<< "..no to bye\n"; break; }
        //--------------------------wstawiamy tekst do ramki
        dbw->buforSetNag(AT_SMM_WRITE);
        dbw->buforAddReg(0);
        dbw->buforAddReg(strlen(buf));
        dbw->buforAddStr(buf);
        dbw->buforAddCRC();
        //--------------------------wysyłamy i odbieramy zwrotkę
        atSC->go();
        //---------------------------wypisujemy zwrotkę na ekran

        cout << dbr->charToHex();
        cout <<"\n";

    }
    if(atSC->isError()){ cout<<atSC->Error(); cout<<"\n";  }

    delete dbr;
    delete dbw;
    delete atSC;

    return 0;
}
