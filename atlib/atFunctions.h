#ifndef ATFUNCTIONS_H
#define ATFUNCTIONS_H

#include <sstream>
#include <iostream>

using namespace std;

string charToBit(uint8_t s);
string int16ToBit(uint16_t s);
string int32ToBit(uint32_t s);
string IntToHex(int f);
void delay_ns(long int timeout);
void delay_us(long int timeout);
void delay_ms(long int timeout);
void delay_s(long int timeout);
string Time_to_string(struct tm* ti);

#endif // ATFUNCTIONS_H
