#define RELAY_PIN1 8
#define RELAY_PIN2 9
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

void setup()
{
  emon1.current(1, 111.1);             // Current: input pin, calibration.
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  Serial.begin(9600); // open serial
  Serial.println("Press the 1 to toggle relay 1 on/off, 2 to toggle relay 2 on/off");
}

void loop()
{
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  
  Serial.print(Irms*230.0);	       // Apparent power
  Serial.print(" ");
  Serial.println(Irms);		       // Irms
  static int relayVal1 = 0;
  static int relayVal2 = 0;
  int cmd;
  
  while (Serial.available() > 0)
  {
    cmd = Serial.read();
      
    switch (cmd)
    {
    case '1':
      {
        relayVal1 ^= 1; // xor current value with 1 (causes value to toggle)
        if (relayVal1)
          Serial.println("Relay on");
        else
          Serial.println("Relay off");
        break;
      }
      
     case '2':
      {
        relayVal2 ^= 1; // xor current value with 1 (causes value to toggle)
        if (relayVal2)
          Serial.println("Relay on");
        else
          Serial.println("Relay off");
        break;
      }
    default:
      {
       Serial.println("Press the 1 to toggle relay 1 on/off, 2 to toggle relay 2 on/off");
      }
    }
      
    if (relayVal1)
      digitalWrite(RELAY_PIN1, HIGH);
    else
      digitalWrite(RELAY_PIN1, LOW);
        if (relayVal2)
      digitalWrite(RELAY_PIN2, HIGH);
    else
      digitalWrite(RELAY_PIN2, LOW);
   }
   
}

