#include <HardwareSerial.h>

extern void gpsTask(unsigned long now);
extern void gpsTaskInit(HardwareSerial  *serial);

extern bool updateDisplayTime;
extern bool updateDisplayGps;

// GPS data

extern bool locationValid;
extern double lat;
extern double lon;

extern bool satInfoValid;
extern int nrOfSats;
extern int hdop;

extern bool dateValid;
extern int mday;
extern int month;
extern int year;

extern bool timeValid;
extern int hour;
extern int min;
extern int sec;
