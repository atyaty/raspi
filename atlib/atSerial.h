#ifndef ATSERIAL_H
#define ATSERIAL_H

#include <sys/ioctl.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

//#include <wiringPi.h>
//#include <wiringSerial.h>
#include <atDataBuffer.h>

//------------------------------------------
using namespace std;

//------------------------------------------
class atSerial {
private:
    int fd;
    long int timeout;

public:
    //atSerial( long int t );
    atSerial( const char* d, const int b, long int t );
    void puts(string s);
    void puts(atDataBuffer *d);
    string gets();
    void gets(atDataBuffer *d);
    void gets1(atDataBuffer *d);

};

#endif // ATSERIAL_H
