int averageValue=0;
int lowestValue=0;
int highestValue=0;
int range=0;
int multiplicator;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  
  lowestValue = analogRead(A0);
  highestValue = analogRead(A0);
  
  for (int i=0;i<1000;i++)
  {
  //average
  int currentValue = analogRead(A0);
  //print the current value
  Serial.println(i+". Value "+currentValue);
  delay(50);        // delay in between reads for stability
  averageValue = currentValue + averageValue / 2;
  
  if (currentValue < lowestValue)
  {
    lowestValue=currentValue;
  }
  if (currentValue > highestValue)
  {
    highestValue = currentValue;
  }
  
  
  
  }
  Serial.println("Average Value (should be 513)"+averageValue);
  range = highestValue-lowestValue;
  multiplicator = 430/range;
  delay(10000000);


  }

