#include "atSocketClient.h"

//--------------------------------------------------
atSocketClient::atSocketClient(atDataBuffer *dr,atDataBuffer *dw, std::string h, uint16_t p)
{
    dbr=dr;
    dbw=dw;

    portno = p;
    strcpy(hostname,h.c_str());



    struct hostent *server = gethostbyname(hostname);
    if (server == NULL) { sError="Hostname error"; return; }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(  (char *)server->h_addr,   (char *)&serv_addr.sin_addr.s_addr,   server->h_length   );
    serv_addr.sin_port = htons(portno);



}
//--------------------------------------------------
atSocketClient::~atSocketClient() {    close(sockfd);   }//UWAGA możliwe użycie nie zainicjowanej zmiennej sockfd

//--------------------------------------------------
void atSocketClient::go() {

    if(sError!="") return;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)  sError="Socket error";

    if( sError=="" ){
        n = connect( sockfd,  (struct sockaddr *) &serv_addr,  sizeof(serv_addr)  );
        if( n< 0  ){ sError="Connect error:-pt"; return; }

    }

    if( sError=="" ){
        n = write(sockfd,dbw->getFrame(),dbw->getDataLen());
        if (n < 0){ sError="Write error"; return; }
    }


    if( sError=="" ){
        n = read(sockfd,dbr->getFrame(),dbr->getFrameLen());
        if (n < 0)   sError="Read error";
        dbr->setDataLen(n);
    }

    close(sockfd);

}
