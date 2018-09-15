#include "debug.h"
#include <Wire.h>
#include "displaytask.h"
#include "src/ds3231.h"

bool updateDisplayRtc = false;

int rtchour = 0;
int rtcmin = 0;
int rtcsec = 0;

int rtcmday = 0;
int rtcmonth = 0;
int rtcyear = 0;

// Default : forced mode, standby time = 1000 ms
static unsigned long prev, interval = 1000;

void rtcTaskInit()
{
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
}

void rtcTask(unsigned long now)
{ 
  struct ts t;
  
  // only update time when 500mSec is passed
  if (now - prev > interval)  
  {
    prev = now;
    DS3231_get(&t);
    rtchour = t.hour;
    rtcmin = t.min;
    rtcsec = t.sec;
    
    rtcyear = t.year;
    rtcmonth = t.mon;
    rtcmday = t.mday;
 
    updateDisplayRtc = true; 
  }
}

void updateRtc(int year, int month, int mday, int hour, int min, int sec)
{
  
    debugPrint("updateRtc : ");
    debugPrint(year); 
    debugPrint(month);
    debugPrint(mday); 
    debugPrint(hour); 
    debugPrint(min); 
    debugPrintLn(sec);
    
    struct ts t;
    
    t.sec = sec;
    t.min = min;
    t.hour = hour;
    //t.wday  
    t.mday = mday;
    t.mon = month;
    t.year = year;
    DS3231_set(t);
    debugPrintLn("updateRtc : done");

}

