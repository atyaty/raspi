#ifndef ATSOCKETSERVER_H
#define ATSOCKETSERVER_H


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <sys/ioctl.h>

#include <atDataBuffer.h>

#define AT_SOCKET_FRAME_LEN 256

using namespace std;

//---------------------------------------------------
typedef int ( * goData )( atDataBuffer *dbr,atDataBuffer *dbw );
//---------------------------------------------------
class atSocketServer {
private:
    struct sockaddr_in serv_addr;
    int listenfd, max_sd ;
    fd_set master_set, working_set;
    struct timeval timeout;
    bool end_server;

    uint16_t portno;

    atDataBuffer *dbr;
    atDataBuffer *dbw;
    goData goDataFun;

    string sError;
//    int sError;

public:
    atSocketServer(atDataBuffer *dr,atDataBuffer *dw, goData gd, uint16_t p);
    ~atSocketServer();
    bool go();
    void closeConnection(int i);
    void closeAll();
    void goAaccept();
    void goReadWrite(int i);
};

#endif // ATSOCKETSERVER_H
