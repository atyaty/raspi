#include <atFunctions.h>

//--------------------------------------------------------
string charToBit(uint8_t s){
    string sRet="";
    for(int j=0;j<8;j++){
            uint8_t c=  ((s & (1 << (7-j))) >> (7-j));
            sRet+=((char)(48+c));
    }
    return sRet;
}
//--------------------------------------------------------
string int32ToBit(uint32_t s){
    string sRet="";
    for(int j=0;j<32;j++){
            uint8_t c=  ((s & (1 << (31-j))) >> (31-j));
            sRet+=((char)(48+c));
    }
    return sRet;
}
//--------------------------------------------------------
string int16ToBit(uint16_t s){
    string sRet="";
    for(int j=0;j<16;j++){
            uint8_t c=  ((s & (1 << (15-j))) >> (15-j));
            sRet+=((char)(48+c));
    }
    return sRet;
}

//---------------------------------------------------------
string IntToHex(int f){

    stringstream ss;
    ss << std::hex << (unsigned int) f;
    return ss.str();

}
//-------------------------------------------------
void delay_ns(long int timeout){
    struct timespec t,t0;
    clock_gettime(CLOCK_REALTIME,&t0);
    long int n,i=0;
    while ( true  )   {
        clock_gettime(CLOCK_REALTIME,&t);
        n = t.tv_nsec-t0.tv_nsec+(t.tv_sec-t0.tv_sec)*1000000000;
        i++;
        if(n>timeout) break;
    }
}
void delay_us(long int timeout){ return delay_ns(timeout*1000);}
void delay_ms(long int timeout){ return delay_ns(timeout*1000000);}
void delay_s(long int timeout){ return delay_ns(timeout*1000000000);}

//-------------------------------------------------------------
string Time_to_string(struct tm* ti){
    return to_string(ti->tm_year+1900)+"."+to_string(ti->tm_mon+1)+"."+to_string(ti->tm_mday)+" "+
           to_string(ti->tm_hour)+":"+to_string(ti->tm_min)+":"+to_string(ti->tm_sec);
}
