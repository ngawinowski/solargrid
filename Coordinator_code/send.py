#! /usr/bin/python    
# Import and init an XBee device
from xbee import XBee,ZigBee
import serial, time, sqlite3 as lite   
ser = serial.Serial('/dev/tty.usbserial-AH0014W9', 9600)
# Use an XBee 802.15.4 device
# To use with an XBee ZigBee device, replace with use:
# xbee = ZigBee(ser)
# otherwise
# xbee = XBee(ser)

zb = ZigBee(ser)
house1long = '\x00\x13\xA2\x00\x40\x2C\x7C\xE6'
house1short = '\xD6\x33'
house2long = '\x00\x13\xA2\x00\x40\x30\x10\x83'
house2short = '\x4C\x90'
house3long = '\x00\x13\xA2\x00\x40\x2C\x7C\xB1'
house3short = '\xBC\xC0'

def send(destlong, destshort, payload):
		zb.send('tx', dest_addr_long=destlong,dest_addr=destshort, data=payload)
   		print "Sent"
	   	response = zb.wait_read_frame()
	   	if (response.get("id") == "tx_status"):
			print "tx status received"
			if (response.get("deliver_status") == "\x00"):
				print "Sucessfully delivered"
				delivered = True
			else:
				print "Got response but something was wrong"
		else:
			print "no tx response"

while True:
	try:
		#here we need to put some inteligence, like select data from our table, do some calculations
		send( house3long, house3short, 'Relay0_On');
		print "Relay 0 on"
		time.sleep(120) # delays for 5 seconds
		send( house3long, house3short, 'Relay0_Off');
		print "Relay 0 off"
		time.sleep(120) # delays for 5 seconds
	except KeyboardInterrupt:
		break
ser.close()

