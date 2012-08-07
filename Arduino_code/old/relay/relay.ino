#define RELAY_PIN1 3
#define RELAY_PIN2 4

void setup()
{
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  Serial.begin(9600); // open serial
  Serial.println("Press the 1 to toggle relay 1 on/off, 2 to toggle relay 2 on/off");
}

void loop()
{
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

