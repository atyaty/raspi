#ifndef ATSOCKETCLIENT_H
#define ATSOCKETCLIENT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <atDataBuffer.h>

using namespace std;
class atSocketClient
{
private:
    uint16_t portno;
    char hostname[256];
    string sError;

    int sockfd;
    struct sockaddr_in serv_addr;

    atDataBuffer *dbr;
    atDataBuffer *dbw;


public:
    atSocketClient(atDataBuffer *dr,atDataBuffer *dw,std::string h, uint16_t p);
    ~atSocketClient();
    void go();
    bool isError() {return sError!="";}
    string Error() {return sError;}
};

#endif // ATSOCKETCLIENT_H
