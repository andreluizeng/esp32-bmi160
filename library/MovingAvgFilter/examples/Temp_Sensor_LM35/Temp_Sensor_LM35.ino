#include <MovingAvgFilter.h>

// tmp35 connected to GPIO34 (ADC1_CH6)
const int lm35_pin = 34; 

MovingAvgFilter myfilter;

void setup() {

  Serial.begin (115200);
  myfilter.begin (10);

  int value = analogRead (lm35_pin);
  }myfilter.initValue (value);

void loop() {

  float avg;
  int value = analogRead (lm35_pin);
  
  avg = myfilter.calculateAvg (value);
  
  Serial.print(value);
  Serial.print(" , ");
  Serial.print(avg);
  Serial.println("");

  delay (100);
}
