#ifndef NAV_H
#define NAV_H

#include <TinyGPS++.h>

class NEO6M
{
public:
    void gpsBegin();
    void gpsUpdate();
    void gpsPrintCoordinates();
    TinyGPSPlus gps;

private:
    bool isInitialized = false;
};

#endif
