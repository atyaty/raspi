/*
by AT
*/
#include "atCzas.h"

//----------------------------------------
void atCzas::Reset( qint64 t){
 tStoperStart=QDateTime::currentDateTime();
 tStoperStart.addMSecs(-t);
 going=false;
 tPause=0;
 pause=false;
}


