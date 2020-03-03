#ifndef ATFILELOG_H
#define ATFILELOG_H

#include "at_File.h"
#include "atFunctions.h"

class atFileLog : public at_File {
private:
    int LogLevel;
public:
    atFileLog(int l):at_File(){LogLevel=l;}
    atFileLog(string P, string F, string E, int l):at_File(P,F,E){LogLevel=l;}
    void Rap(string s,int l){ if( l<=LogLevel ) Append(s); }
    void RapDate(string s,int l);

};

#endif // ATFILELOG_H
