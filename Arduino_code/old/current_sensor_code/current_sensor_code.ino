int averageValue=0;
int lowestValue=0;
int highestValue=0;
int range=0;
int multiplicator=42;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  
  delay(50);
  lowestValue = analogRead(A0);
  delay(50);
  highestValue = analogRead(A0);
  delay(50);
  averageValue = (lowestValue + highestValue) /2;
  for (int i=0;i<1000;i++)
  {
  //average
  int currentValue = analogRead(A0);
  //print the current value
  Serial.print(i);
  Serial.print(". Value: ");
  Serial.println(currentValue);
  delay(10);        // delay in between reads for stability
  averageValue = (currentValue + averageValue) / 2;
  
  if (currentValue < lowestValue)
  {
    lowestValue=currentValue;
  }
  if (currentValue > highestValue)
  {
    highestValue = currentValue;
  }
  
  
  
  }
  Serial.print("average value ");
  Serial.println(averageValue);
  range = highestValue-lowestValue;
  Serial.print("range value ");
  Serial.println(range);
  int measurement=multiplicator*range;
  //multiplicator = 424/range;
  Serial.println("Measurement ");
  Serial.print(measurement);
  Serial.print(" mA");
  delay(10000000);


  }

