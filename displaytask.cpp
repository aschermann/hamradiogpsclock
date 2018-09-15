
#include "src/LiquidCrystal_I2C.h"

#include "debug.h"
#include "displaytask.h" 
#include "bme280task.h"
#include "gpstask.h"
#include "rtctask.h"

static char dgrChar = (char)0xdf; // 0xb2 => large older display.
static char antennachar = (char)0xdd; 

// Display object aanmaken, eventueel i2c adres aanpassen!
LiquidCrystal_I2C lcd(0x27,20,4);     // set the LCD address to 0x27 for a 16 chars and 2 line display 
 
void displayTaskInit()
{  
    lcd.begin(); // initialiseer dispay
    lcd.setCursor(0, 1); // first line
    lcd.print("   GPS Clock v1.0   ");
    //         12345678901234567890
    lcd.setCursor(0, 2); // first line
    lcd.print("       by PH2LB      ");
}

void calcLocator(char *dst, double lat, double lon) 
{
  int o1, o2, o3;
  int a1, a2, a3;
  double remainder;
  // longitude
  remainder = lon + 180.0;
  o1 = (int)(remainder / 20.0);
  remainder = remainder - (double)o1 * 20.0;
  o2 = (int)(remainder / 2.0);
  remainder = remainder - 2.0 * (double)o2;
  o3 = (int)(12.0 * remainder);

  // latitude
  remainder = lat + 90.0;
  a1 = (int)(remainder / 10.0);
  remainder = remainder - (double)a1 * 10.0;
  a2 = (int)(remainder);
  remainder = remainder - (double)a2;
  a3 = (int)(24.0 * remainder);
  dst[0] = (char)o1 + 'A';
  dst[1] = (char)a1 + 'A';
  dst[2] = (char)o2 + '0';
  dst[3] = (char)a2 + '0';
  dst[4] = (char)o3 + 'a';
  dst[5] = (char)a3 + 'a';
  dst[6] = (char)0; 
} 
 
void displayTime(int line, bool force)
{
  if (updateDisplayTime || updateDisplayRtc || force)
  {
    // first line is always time and date
    lcd.setCursor(0, line);  
  
    if (!dateValid)
    {
      // when gpsdate isn't valid take over the RTC values
      mday = rtcmday;
      month = rtcmonth;
      year = rtcyear;
    }
    
    if (!timeValid)
    {
      // when gpstime isn't valid take over the RTC values
      hour = rtchour;
      min = rtcmin;
      sec = rtcsec;
    } 
    
    if (mday < 10)
      lcd.print('0');
    lcd.print(mday,DEC);
    lcd.print('/');
    if (month < 10)
      lcd.print('0');
    lcd.print(month,DEC);
    lcd.print('/');
    lcd.print(year,DEC);
    
    lcd.print(' ');
    
    if (hour < 10)
      lcd.print('0');
    lcd.print(hour,DEC);
    lcd.print(':');
    if (min < 10)
      lcd.print('0');
    lcd.print(min,DEC);
    lcd.print(':');
    if (sec < 10)
      lcd.print('0');
    lcd.print(sec,DEC);
    if (!timeValid)  
      lcd.print('r');
    else
      lcd.print(' ');
       
    updateDisplayTime = false;
    updateDisplayRtc = false; 
  }
}

void displayLocatorAndSatInfo(int line, bool force)
{  
  if (updateDisplayGps || force)
  {
    lcd.setCursor(0, line); // second  line
    lcd.print("Loc: ");
    char buf[12];
    calcLocator(buf, lat, lon);
    lcd.print(buf);
    
    lcd.print(" Sat: ");
    if (nrOfSats < 10)
      lcd.print('0');
    lcd.print(nrOfSats);   
    lcd.print(' '); // fill line 
  }
}

void displayLatDgr(int line, bool force)
{
  if (updateDisplayGps || force)
  { 
    lcd.setCursor(0, line); 
   
    lcd.print("Lat: "); 
    
    // now some nice formating
    int l = (int)abs(lat);
    if (l < 100)
      lcd.print(' '); 
    if (l < 10)
      lcd.print(' '); 
    lcd.print(lat, 6);  
    lcd.print(dgrChar);  // dgr (223)
    
    if (lat < 0)
      lcd.print('E');
    else
      lcd.print('W');  
    lcd.print("   ");  // fill up to blank line
  }
}

void displayLatDgrDms(int line, bool force)
{
  if (updateDisplayGps || force)
  { 
    lcd.setCursor(0, line); 
   
    lcd.print("Lat: "); 
 
    float dms_sec = lat;
    int dms_deg = dms_sec;
    dms_sec -= dms_deg; // remove the degrees from the calculation
    dms_sec *= 60; // convert to minutes
    int dms_min = dms_sec;
    dms_sec -= dms_min; // remove the minuts from the calculation
    dms_sec *= 60; // convert to seconds 

    if (dms_deg < 100)
      lcd.print(' ');
    if (dms_deg < 10)
      lcd.print(' ');
    lcd.print(dms_deg);
    lcd.print(dgrChar);  // dgr (0xdf for japan)
    lcd.print(dms_min);
    lcd.print((char)0x27);
    lcd.print(dms_sec, 3);
    lcd.print((char)0x22);
  
    
    if (lat < 0)
      lcd.print('E');
    else
      lcd.print('W'); 
  }
}

void displayLonDgr(int line, bool force)
{
  if (updateDisplayGps || force)
  { 
    lcd.setCursor(0, line);  
     
    lcd.print("Lon: "); 

    // now some nice formating
    int l = (int)abs(lon);
    if (l < 100)
      lcd.print(' ');
    if (l < 10)
      lcd.print(' ');
    lcd.print(lon, 6);   
    lcd.print(dgrChar);  // dgr (0xdf for japan)
    if (lon >= 0)
      lcd.print('N');
    else
      lcd.print('S');
    lcd.print("   ");  // fill up to blank line
    updateDisplayGps = false;   
  }
}

void displayLonDgrDms(int line, bool force)
{
  if (updateDisplayGps || force)
  { 
    lcd.setCursor(0, line); 
   
    lcd.print("Lon: "); 
    float dms_sec = lon;
    int dms_deg = dms_sec;
    dms_sec -= dms_deg; // remove the degrees from the calculation
    dms_sec *= 60; // convert to minutes
    int dms_min = dms_sec;
    dms_sec -= dms_min; // remove the minuts from the calculation
    dms_sec *= 60; // convert to seconds 
    if (dms_deg < 100)
      lcd.print(' ');
    if (dms_deg < 10)
      lcd.print(' ');
    lcd.print(dms_deg);
    lcd.print(dgrChar);  // dgr (0xdf for japan)
    lcd.print(dms_min);
    lcd.print((char)0x27);
    lcd.print(dms_sec, 3);
    lcd.print((char)0x22);
    if (lon >= 0)
        lcd.print('N');
      else
        lcd.print('S');
    updateDisplayGps = false;   
  } 
}
 
void displayBme280TH(int line, bool force)
{
  if (updateDisplayBme280  || force)
  { 
    lcd.setCursor(0, line);   
    lcd.print("T:"); 
    if (temp > -10.0)
      lcd.print(' ');   // extra space when no -xx.x
    if (temp < 10.0 && temp >= 0.0 )
      lcd.print(' ');   // extra space for x.x
    lcd.print(temp, 1);
    lcd.print(dgrChar);  // dgr (0xdf for japan)
    lcd.print('C');
    lcd.print(' ');
    lcd.print("H: "); 
    lcd.print(humidity, 1) ;
    lcd.print('%');  // fill up to blank line  
    if (humidity < 100.0)
      lcd.print(' ');
    lcd.print(' ');
  }
}
 
void displayBme280P(int line, bool force)
{
  if (updateDisplayBme280  || force)
  { 
    lcd.setCursor(0, line); 
    // pressure = 123.3;
    lcd.print("P: "); 
    if (pressure < 1000)
      lcd.print(' '); 
    
    lcd.print(pressure, 1); 
    lcd.print(" hPa       ");  // fill up to blank line
    updateDisplayBme280 = false;
  }
}

static int alt = -1; 
static int prev_sec = -1; 
static int prevAlt = -1;

void displayTask(unsigned long now)
{ 
    int alt_sec = rtcsec;
    if (timeValid)
    { 
      alt_sec = sec;
    } 
    if (prev_sec != alt_sec)
    {
      if (alt_sec % 5 == 0)
      {
        alt++;
      }
      if (alt == 3 || alt == -1)
      {
        alt = 0;
      }
      prev_sec = alt_sec;
    }

    bool updateDisplay = updateDisplayTime || updateDisplayRtc || alt != prevAlt;

    if (updateDisplay)
    {
      displayTime(0, false);
      displayLocatorAndSatInfo(1, false); 

      if (alt == 0)
      {
        displayLatDgr(2, alt != prevAlt); 
        displayLonDgr(3, alt != prevAlt);
      }
      else if (alt == 1)
      {
        displayLatDgrDms(2, alt != prevAlt);
        displayLonDgrDms(3, alt != prevAlt);
      }
      else if (alt == 2)
      {
        displayBme280TH(2, alt != prevAlt);
        displayBme280P(3, alt != prevAlt);
      }
    }
    prevAlt = alt;
}

