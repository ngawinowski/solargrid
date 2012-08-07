//Programme pour salle 1

#include <XBee.h>

#include <EmonLib.h>				// Include emon library

// Digital pins for status, error, data information
#define CURRENTSENSOR1 1
#define CURRENTSENSOR2 2
#define BATTERYSENSOR1 3
#define RELAY_PIN0 7
#define RELAY_PIN1 8
#define RELAY_PIN2 9
#define STATUS_LED 10
#define ERROR_LED 11
#define DATA_LED 12
#define INTERVAL 1000
#define TIMESPAN 60000
#define SENSORINFO 1

// create the XBee object
XBee xbee = XBee();
EnergyMonitor 	emon1;				// Create an instance
unsigned long waitUntil=0;




XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();
char ReceivedData[10];

boolean ReceiveData()
{
  xbee.readPacket();
	Serial.println("Receiving");
	if (xbee.getResponse().isAvailable()) {
	  // got something
		  Serial.println("Got something");

	  if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
		// got a zb rx packet
				  Serial.println("Got rx package");

		// now fill our zb rx class
		xbee.getResponse().getZBRxResponse(rx);

		if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
			// the sender got an ACK
			 Serial.println("the sender got an ACK");

		} else {
					   Serial.println("the sender got NO ACK");

			// we got it (obviously) but sender didn't get an ACK
		}
		// set dataLed PWM to value of the first byte in the data
		for(int i=0; i < rx.getDataLength(); i++) {

		   ReceivedData[i] = (char)rx.getData(i);
			Serial.println((char)rx.getData(i));


		}
		return true;

	  } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
		xbee.getResponse().getModemStatusResponse(msr);
		// the local XBee sends this response on certain events, like association/dissociation

		if (msr.getStatus() == ASSOCIATED) {
		  // yay this is great.  flash led
		  Serial.print("ASSOCIATED");
		} else if (msr.getStatus() == DISASSOCIATED) {
		  // this is awful.. flash led to show our discontent
		  Serial.print("DISASSOCIATED");
		} else {
		  // another status
		}
	  } else {
	  	// not something we were expecting
	  }
	} else if (xbee.getResponse().isError()) {
	  Serial.print("Error reading packet.  Error code: ");  
	  Serial.println(xbee.getResponse().getErrorCode());
	}
}

void SendData (char SendString[])
{
  char payload[10];
  strncpy(payload, SendString, 9); 

  /** 
  * Sh – 13A200 Sl – 403010B5 Coordinator
  *
  */

  // SH + SL Address of receiving XBee
  XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x403010B5);
  ZBTxRequest zbTx = ZBTxRequest(addr64, (uint8_t*) (payload), sizeof(payload));
  ZBTxStatusResponse txStatus = ZBTxStatusResponse();
  xbee.send(zbTx);

  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  if (xbee.readPacket(500)) {
	// got a response!

	// should be a znet tx status            	
	if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
	  xbee.getResponse().getZBTxStatusResponse(txStatus);

	  // get the delivery status, the fifth byte
	  if (txStatus.getDeliveryStatus() == SUCCESS) {
		// success.  time to celebrate
	  } else {
		// the remote XBee did not receive our packet. is it powered on?
	  }
	}
  } else if (xbee.getResponse().isError()) {
	//nss.print("Error reading packet.  Error code: ");  
	//nss.println(xbee.getResponse().getErrorCode());  
  } else {
	// local XBee did not provide a timely TX Status Response -- should not happen
  }
}

/* For Fashing the LEDS*/
void flashLed(int pin, int times, int wait) {

	for (int i = 0; i < times; i++) {
	  digitalWrite(pin, HIGH);
	  delay(wait);
	  digitalWrite(pin, LOW);

	  if (i + 1 < times) {
		delay(wait);
	  }
	}
}

char* collectData(int sensorID, unsigned long timespan, unsigned int interval) {
  int i = 1;
  double dataAvg = 0;
  waitUntil = millis() + timespan;
  while((long)(millis() - waitUntil) <= 0) {
	Serial.print("LOOP ");
	Serial.println(i);
	double data = emon1.calcIrms(1480);				// Calculate Irms only
	dataAvg = data + dataAvg;
	delay(interval);			// wait 1s
	i++;
	Serial.println((long)(millis() - waitUntil));
	Serial.println(dataAvg);
	}

	dataAvg = (dataAvg / i) * 1000;				// AVG in mA
	int dataAvgINT = (int)(dataAvg+0.5);
	Serial.println(dataAvgINT);
	static char payload[] = {0, 0, 0, 0 };
	  payload[0] = SENSORINFO;
	  payload[1] = CURRENTSENSOR1;
	  payload[2] = dataAvgINT >> 8 & 0xff;
	  payload[3] = dataAvgINT & 0xff;
  	  return payload;
}

void processData(char data[]) {
	if(strcmp(data,"Relay0_On") == 0) { // strcmp returns 0 for matching String
		digitalWrite(RELAY_PIN0, HIGH);
		// Light Status LED twice to show that relay have been switched
		flashLed(STATUS_LED, 2, 1000);
		}
	if(strcmp(data,"Relay0_Off") == 0) { // strcmp returns 0 for matching String
		digitalWrite(RELAY_PIN0, LOW);
		// Light Status LED twice to show that relay have been switched
		flashLed(STATUS_LED, 2, 1000);
		}
	if(strcmp(data,"Relay1_On") == 0) { // strcmp returns 0 for matching String
		digitalWrite(RELAY_PIN1, HIGH);
		// Light Status LED twice to show that relay have been switched
		flashLed(STATUS_LED, 2, 1000);
		}
	if(strcmp(data,"Relay1_Off") == 0) { // strcmp returns 0 for matching String
		digitalWrite(RELAY_PIN1, LOW);
		// Light Status LED twice to show that relay have been switched
		flashLed(STATUS_LED, 2, 1000);
		}
	if(strcmp(data,"Relay2_On") == 0) { // strcmp returns 0 for matching String
		digitalWrite(RELAY_PIN2, HIGH);
		// Light Status LED twice to show that relay have been switched
		flashLed(STATUS_LED, 2, 1000);
		}
	if(strcmp(data,"Relay2_Off") == 0) { // strcmp returns 0 for matching String
		digitalWrite(RELAY_PIN2, LOW);
		// Light Status LED twice to show that relay have been switched
		flashLed(STATUS_LED, 2, 1000);
		}

}


void setup() {

  	pinMode(STATUS_LED, OUTPUT);
	pinMode(ERROR_LED, OUTPUT);
	pinMode(DATA_LED,  OUTPUT);
	pinMode(RELAY_PIN0, OUTPUT);
	pinMode(RELAY_PIN1, OUTPUT);
	pinMode(13, OUTPUT);
	emon1.current(1, 111.1);		// Current: input pin, calibration.
	xbee.begin(9600);				// Start xbee with 9600 baut
	flashLed(STATUS_LED, 3, 50);
	unsigned long waitUntil=0;
}



void loop() {   

  if (ReceiveData() == true) {
	processData(ReceivedData);
	}
  
  char *payload = collectData(CURRENTSENSOR1, TIMESPAN, INTERVAL);
  SendData(payload);


}