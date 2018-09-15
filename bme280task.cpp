
// enviromental parameters
float temp = 0.0;
float pressure = 0.0;
float humidity = 0.0;  

float prevTemp = 0.0;
float prevPressure = 0.0;
float prevHumidity = 0.0; 

bool updateDisplayBme280 = false;

#include "src/BME280I2C.h"
#include "displaytask.h"
#include "debug.h"

// BME280 sensor object
BME280I2C bme;     

// Default : forced mode, standby time = 1000 ms
static unsigned long prev, interval = 1000;
static int counter = 0;

void bme280TaskInit()
{
    bme.begin();
}

void updateEnvParameters()
{
    temp = bme.temp(true);
    pressure = bme.pres(1);    // 1 = hPa (milliBar)  
    humidity =  bme.hum(); 
} 
 
// dit moet nog iets anders.
// je moet 2 x sample-en om een goede humidity te krijgen
// maar delay(1000) is niet de nette oplossing.
void bme280Task(unsigned long now)
{
  if (now - prev > interval)  
  {
    prev = now;
    if (counter == 1) // first sample
    {
      updateEnvParameters();
    }
    else if (counter == 2)
    {
      updateEnvParameters();
      if (temp != prevTemp ||
          humidity != prevHumidity ||
          pressure != prevPressure)
      {
        prevTemp = temp;
        prevHumidity = humidity;
        prevPressure = pressure;
        updateDisplayBme280 = true;
      }
    }
    else if (counter == 60)
    {
      counter = 0;    
    }
    counter++;
  }
}


