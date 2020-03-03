/*
by AT
*/

#ifndef atDeviceCialoH
#define atDeviceCialoH


#include "atGeometria.h"
#include "atDevice.h"

class atDeviceCialo : public atDevice {
protected:
  atPoint *polozenie;
  atWektor *kierunek;
  atWektor *predkosc;
public:
  atDeviceCialo(QString s, QString id);
  double GoFun();
};

#endif // ATDEVICECIALO_H
