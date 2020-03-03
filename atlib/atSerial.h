#ifndef ATSERIAL_H
#define ATSERIAL_H

#include <sys/ioctl.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include <atDataBuffer.h>
//------------------------------------------
using namespace std;

//------------------------------------------
class atSerial {
private:
    int fd;
    long int timeout;
    atDataBuffer *dbr;
    atDataBuffer *dbw;


public:
    atSerial( atDataBuffer *dr,atDataBuffer *dw, std::string h);
    void go();
};

#endif // ATSERIAL_H
