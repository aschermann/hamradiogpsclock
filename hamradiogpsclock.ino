/*  
 * ------------------------------------------------------------------------
 * "PH2LB LICENSE" (Revision 1) : (based on "THE BEER-WARE LICENSE" Rev 42) 
 * <lex@ph2lb.nl> wrote this file. As long as you retain this notice
 * you can do modify it as you please. It's Free for non commercial usage 
 * and education and if we meet some day, and you think this stuff is 
 * worth it, you can buy me a beer in return
 * Lex Bolkesteijn 
 * ------------------------------------------------------------------------ 
 * Filename : gps-clock.ino  
 * Version  : 0.1 (DRAFT)
 * ------------------------------------------------------------------------
 * Description :  
 * ------------------------------------------------------------------------
 * Revision : 
 *  - 2016-jan-26 0.1 initial version  
 * ------------------------------------------------------------------------
 * Hardware used : 
 *  - Arduino Mega 2560 PRO (Embed) 
 *    https://robotdyn.com/mega-2560-pro-embed-ch340g-atmega2560-16au.html
 *  - NeoBlox NEO6 GPS module
 * ------------------------------------------------------------------------
 * Software used :   
 *  - TinyGps++ library https://github.com/mikalhart/TinyGPSPlus/
 *  - DS3231 library https://github.com/rodan/ds3231
 *  - BME280 library https://github.com/finitespace/BME280
 *  
 * For licenses of the used libraries, check the links above.
 * ------------------------------------------------------------------------ 
 * TODO LIST : 
 *  - add more sourcode comment
 *  - add debounce for the keys 
 *  - harden the rotary switch (sometimes strange behaviour due to timing
 *    isues (current no interrupt).
 * ------------------------------------------------------------------------ 
 */

// inspire : 
//  http://w8bh.net/avr/clock2.pdf
//  http://on4cdu.net/arduino-en-gps-data/
 
#include "gpstask.h" 
#include "rtctask.h" 
#include "displaytask.h" 
#include "bme280task.h"  
#include "debug.h"

       
//SoftwareSerial gpsSerial(10, 11); // RX, TX -> we only use RX on the serial port leaving TX for debug.
#define gpsSerial Serial1 

// Playtime
void setup() 
{ 
  debugSerial.begin(57600); 
  debugPrintLn(F("setup"));

  // call the task init function
  displayTaskInit();
  delay(2000);
  gpsTaskInit(&gpsSerial);     
  rtcTaskInit();
  bme280TaskInit();
  
  debugPrintLn(F("start loop"));  
  updateDisplayRtc = true;
  updateDisplayTime = true;
  updateDisplayBme280 = true;
  updateDisplayGps = true;
}
   
// the main loop
void loop() 
{    
  unsigned long now = millis(); 
  
  gpsTask(now);
  rtcTask(now);
  bme280Task(now);
  displayTask(now); 
}

