#ifndef ATSTOPER_H
#define ATSTOPER_H

#include <sstream>
#include <iostream>

using namespace std;

class atStoper {
private:
    struct timespec t0;
    uint64_t timeout;

public:
    atStoper(uint64_t t){start(t);}
    atStoper(){}
    bool waiting(){ return time()<timeout;}
    void start(uint64_t t)    {timeout=t;clock_gettime(CLOCK_REALTIME,&t0);}
    void start_us(long int t) {timeout=t;timeout*=1000; clock_gettime(CLOCK_REALTIME,&t0);}
    void start_ms(long int t) {timeout=t;timeout*=1000000;clock_gettime(CLOCK_REALTIME,&t0);}
    void start_s(long int t)  {timeout=t;timeout*=1000000000;clock_gettime(CLOCK_REALTIME,&t0);}
    void start_min(long int t){timeout=t;timeout*=60000000000;clock_gettime(CLOCK_REALTIME,&t0);}
    void stop()               {timeout=0;clock_gettime(CLOCK_REALTIME,&t0);}
    uint64_t time();
    uint64_t time_ns(){return time();}
    uint64_t time_us(){return time()/1000;}
    uint64_t time_ms(){return time()/1000000;}
    uint64_t time_s() {return time()/1000000000;}
    uint64_t time_out() {return timeout;}

};

#endif // ATSTOPER_H
