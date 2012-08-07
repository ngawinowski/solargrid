/* Solargrid Node Code
Consists of two threads, one reading the sensor value, calculating the current and measureDone the data to the coordinator and another one for controlling the relay
*/

#include <XBee.h>					// Include xbee library
#include <EmonLib.h>				// Include emon library
#include <pt.h>						// Include protothread library
//#include <stdio.h>

// Digital pins for status, error, data information
#define CURRENTSENSOR1 1
#define CURRENTSENSOR2 2
#define BATTERYSENSOR1 3
#define RELAY_PIN0 8
#define RELAY_PIN1 9
#define STATUS_LED 10
#define ERROR_LED 11
#define DATA_LED 12
#define INTERVAL 5000
#define SENSORINFO 1

static struct pt pt1, pt2;			// each protothread needs one of these
XBee xbee = XBee();					// Create a xbee object
EnergyMonitor 	emon1;				// Create an instance
unsigned long waitUntil=0;
int sending, receiving;

void setup() {
	pinMode(STATUS_LED, OUTPUT);
	pinMode(ERROR_LED, OUTPUT);
	pinMode(DATA_LED,  OUTPUT);
	pinMode(RELAY_PIN0, OUTPUT);
	pinMode(RELAY_PIN1, OUTPUT);
	emon1.current(1, 111.1);		// Current: input pin, calibration.
	xbee.begin(9600);				// Start xbee with 9600 baut
	flashLed(STATUS_LED, 3, 50);
	PT_INIT(&pt1);					// initialise the protothread variables
	PT_INIT(&pt2);
		waitUntil = millis() + INTERVAL;  // Initial Trigger time
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

static int relaythread(struct pt *pt, int sensorID) {
// create reusable response zigbee object for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
// create statusResponse object, for error handling
ModemStatusResponse msr = ModemStatusResponse();

PT_BEGIN(pt);				// Start PT thread


	while(1) {				// Never stop this thread
	if (sending = 0) {
		receiving = 1;
		xbee.readPacket();
		if (xbee.getResponse().isAvailable()) {
		  // got something

		  if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
			// got a zb rx packet

			// now fill our zb rx class
			xbee.getResponse().getZBRxResponse(rx);

			if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
				// the sender got an ACK
				flashLed(STATUS_LED, 2, 500);
			} else {
				// we got it (obviously) but sender didn't get an ACK
				flashLed(ERROR_LED, 2, 2000);
			}

		   // DEBUGGING
		   int i = 0; 
		   while (rx.getData(i) != NULL)
		   {
			 Serial.println(i);
			 Serial.println(rx.getData(i));
			i++;
		   }

		   if (rx.getData(0) == 48) {
		   // we need to switch a relay
		   flashLed(DATA_LED, 2, 200);

			 if(rx.getData(1) == 48) {
			 // we need to switch RELAY_PIN0
			 flashLed(DATA_LED, 2, 200);

			   if(rx.getData(2) == 48) {
				 // we need to switch it off
			   flashLed(DATA_LED, 2, 200);
				 digitalWrite(RELAY_PIN0, LOW);
				 }
				 else {
				   // otherwise obviouslly we need to switch it on
				   digitalWrite(RELAY_PIN0, HIGH);
				 }
			 }

			 if(rx.getData(1) == 49) {
			   // we need to switch RELAY_PIN1
					flashLed(DATA_LED, 2, 200);

			   if(rx.getData(2) == 48) {
				 // we need to switch it off
						flashLed(DATA_LED, 2, 200);

				 digitalWrite(13, LOW);
				 digitalWrite(RELAY_PIN1, LOW);
			   }
			   else {
				 // otherwise obviouslly we need to switch it on
				 digitalWrite(RELAY_PIN1, HIGH);
			   }
			 }
			// Light Status LED twice to show that relay have been switched
			flashLed(STATUS_LED, 2, 1000);

		   }

			// set DATA_LED PWM to value of the first byte in the data
		   // analogWrite(DATA_LED, rx.getData(0));
		  } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
			xbee.getResponse().getModemStatusResponse(msr);
			// the local XBee sends this response on certain events, like association/dissociation

			if (msr.getStatus() == ASSOCIATED) {
			  // yay this is great.  flash led
			  flashLed(STATUS_LED, 10, 10);
			} else if (msr.getStatus() == DISASSOCIATED) {
			  // this is awful.. flash led to show our discontent
			  flashLed(ERROR_LED, 10, 10);
			} else {
			  // another status
			  flashLed(STATUS_LED, 5, 10);
			}
		  } else {
		  	// not something we were expecting
			flashLed(ERROR_LED, 1, 25);    
		  }
		} else if (xbee.getResponse().isError()) {
		  //nss.print("Error reading packet.  Error code: ");  
		  //nss.println(xbee.getResponse().getErrorCode());
		}
		receiving = 0;
		
		}
	}
		PT_END(pt);

	}


static int measurethread(struct pt *pt, int sensorID) {
	int i = 1;
	double Irmsavg;
	// 16-bit addressing: Enter address of remote XBee, typically the coordinator
	XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x403010B5);
	ZBTxStatusResponse txStatus = ZBTxStatusResponse();
  	PT_BEGIN(pt);
  while(1) {				// Never stop this thread
		  i = 1;		//Reset timer
		  Irmsavg = 0;
	  while((long)(millis() - waitUntil) <= 0) {
				  Serial.print("LOOP ");
				  Serial.println(i);
		  double Irms = emon1.calcIrms(1480);				// Calculate Irms only
		  Irmsavg = Irms + Irmsavg;
		  delay(1000);			// wait 1s
		  i++;
		  Serial.println((long)(millis() - waitUntil));
		  Serial.println(Irmsavg);

	  }
		Irmsavg = (Irmsavg / i) * 1000;				// AVG in mA

		int IrmsavgINT = (int)(Irmsavg+0.5);
		Serial.println(IrmsavgINT);
		uint8_t payload[] = {0, 0, 0, 0 };
		payload[0] = SENSORINFO;
		payload[1] = CURRENTSENSOR1;
		payload[2] = IrmsavgINT >> 8 & 0xff;
		payload[3] = IrmsavgINT & 0xff;

		PT_WAIT_UNTIL(pt, receiving=0);
		sending = 1;
		ZBTxRequest tx = ZBTxRequest(addr64, payload, sizeof(payload));
		xbee.send(tx);
		flashLed(STATUS_LED, 2, 500);
				delay(5000);
		if (xbee.readPacket(5000)) {
	 			// got a response!

	 			// should be a znet tx status                   
	 			if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
	 		   	xbee.getResponse().getZBTxStatusResponse(txStatus);

	 			   // get the delivery status, the fifth byte
	 			   if (txStatus.getDeliveryStatus() == SUCCESS) {
												flashLed(DATA_LED, 2, 2000);
	 					// success.  time to celebrate
	 		  	 } else {
	 				   flashLed(ERROR_LED, 10, 200);

		 				// the remote XBee did not receive our packet. is it powered on?
	 			   }
	 			}      
	 		} else {
				   flashLed(ERROR_LED, 10, 100);
	 	  // local XBee did not provide a timely TX Status Response -- should not happen
	 	}

		  waitUntil = millis() + INTERVAL;  // wait another interval cycle
	 sending = 0;
 }
	 PT_END(pt);

 }

	/* each time the function is called the second boolean
	*  argument "millis() - timestamp > interval" is re-evaluated
	*  and if false the function exits after that. */
//	PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
//	timestamp = millis(); // take a new timestamp
//	toggleLED();
//  }
//}


void loop() {
	relaythread(&pt2, 42);
	measurethread(&pt1, 900); // schedule the two threads
}