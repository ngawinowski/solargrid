#define S1_PIN 1    // input pin for first inductive sensor
#define SAMPLES 100 // number of samples to take
#define NUM_READINGS 10 // number of readings to average
#define MULTIPLICATOR 209.9502
int val1 = 0;
int reading_cnt = 0; // counter for 1 second samples
float total_rms1 = 0;


void setup() 
  {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Starting measurement");
  } 
void loop() 
{
 
  int max_val1 = 0;
  float rms_val1 = 0;
  
  for (int cnt=0; cnt < SAMPLES; cnt++) 
    {     
  val1 = analogRead(S1_PIN);
  Serial.print(cnt);
  Serial.print(". value read: ");
  Serial.println(val1);  
  rms_val1 = rms_val1 + sq((float)val1);     
     
    }   
  rms_val1 = sqrt(rms_val1 / (SAMPLES/2) );  
  Serial.print("rms value: ");
  Serial.println(rms_val1);
 delay(1000); 
  total_rms1 = total_rms1 + rms_val1;   
  reading_cnt=reading_cnt+1;   
  Serial.print("Iteration: ");
  Serial.println(reading_cnt);
  //delay(1000);
  if (reading_cnt >= NUM_READINGS) 
      {
     float average_rms1 = total_rms1 / NUM_READINGS;
     
    Serial.print("AVG VALUES *RMS: ");
    Serial.print(average_rms1);
    Serial.print("   ");
    float I = average_rms1 * MULTIPLICATOR;
    Serial.print("Current measured: ");
    Serial.println(I);
    delay(10000000);
    
      }
 
    //total_rms1 = 0;
    
    
    
   
  }
 

