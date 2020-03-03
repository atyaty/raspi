#include "atFileLog.h"




//------------------------------------------------------------
void atFileLog:: RapDate(string s, int l){
    time_t czas;
    struct tm* czas_info;
    time(&czas);
    czas_info=localtime(&czas);
    string s1=Time_to_string(czas_info)+":  "+s+"\n";
    Rap(s1,l);
}


