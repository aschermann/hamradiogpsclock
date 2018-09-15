
#include "debug.h"
#include "src/TinyGPS++.h" 
#include "gpstask.h"
#include "rtctask.h"  // needed for updating the rtc
#include "displaytask.h"


bool updateDisplayTime = false;
bool updateDisplayGps = false;

// GPS data

bool locationValid = false;
double lat = 0.0;
double lon = 0.0;

bool satInfoValid = false;
int nrOfSats = 0;
int hdop = 0;

bool dateValid = false;
int mday = 0;
int month = 0;
int year = 0;

bool timeValid = false;
int hour = 0;
int min = 0;
int sec = 0;

double prevLat = 0.0;
double prevLon = 0.0;

int prevNrOfSats = 0;
int prevHdop = 0;
  
int prevSec = 0; 

// The TinyGPS++ object
TinyGPSPlus gps;

HardwareSerial  *gpsSerial = NULL;

void gpsTaskInit(HardwareSerial  *serial)
{
   gpsSerial = serial;
   // set to youre GPS port speed.
   gpsSerial->begin(9600);    
}

void gpsTask(unsigned long now)
{
  bool gpsupdate = false;

  while (gpsSerial->available() > 0)
  {
    gpsupdate = gps.encode(gpsSerial->read());

    if (gpsupdate)
    { 
      locationValid = gps.location.isValid();
      if (locationValid)
      {
        lat =   gps.location.lat();
        lon =   gps.location.lng();  
      }
      else
      {
        lat = 0.0;
        lon = 0.0;
      } 
      
      satInfoValid = gps.satellites.isValid();
      if (satInfoValid)
      {
        nrOfSats = gps.satellites.value(); 
      }
      else
      {
        nrOfSats = 0;
      }
      hdop = gps.hdop.hdop(); 
  
      dateValid = gps.date.isValid();
      if (dateValid)
      {
        mday = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();
      }
      else
      {
        mday = 0;
        month = 0;
        year = 0;
      } 
      timeValid = gps.time.isValid();
      if (timeValid)
      {
        hour = gps.time.hour();
        min = gps.time.minute();
        sec = gps.time.second();
      }
      else
      {
        hour = 0;
        min = 0;
        sec = 0;
      }
    }
  }
  if (gpsupdate)
  {
    if (sec != prevSec) 
    {
      updateDisplayTime = true;
      prevSec = sec;

      // every hour at 42 minutes update the RTC or when the system is cold booted (and RTC battery is dead)
      if ((min == 42 && sec == 0) || rtcyear == 1900)
      {
        updateRtc(year, month, mday, hour, min, sec);
      }
    }
    if (lat != prevLat || lon != prevLon || nrOfSats != prevNrOfSats || hdop != prevHdop)
    { 
      updateDisplayGps = true;
      prevLat = lat;
      prevLon = lon;
      prevNrOfSats = nrOfSats;
      prevHdop = hdop;
    }
  } 
}
 

