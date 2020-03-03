#include <atStoper.h>

/*
//-------------------------------------------------
bool atStoper::waiting(){
    struct timespec t;
    clock_gettime(CLOCK_REALTIME,&t);
    long int n = t.tv_nsec-t0.tv_nsec+(t.tv_sec-t0.tv_sec)*1000000000;
    return n<timeout;

}*/
/*
//-------------------------------------------------
long int atStoper::time(){
    struct timespec t;
    clock_gettime(CLOCK_REALTIME,&t);
    long int n = t.tv_nsec-t0.tv_nsec+(t.tv_sec-t0.tv_sec)*1000000000;
    return n;

}*/
//-------------------------------------------------
uint64_t atStoper::time(){
    struct timespec t;
    clock_gettime(CLOCK_REALTIME,&t);
    uint64_t n = (t.tv_sec-t0.tv_sec);
    n*=1000000000;
    n+=t.tv_nsec-t0.tv_nsec;
//    uint64_t n = t.tv_nsec-t0.tv_nsec+(t.tv_sec-t0.tv_sec)*1000000000;
    return n;

}
