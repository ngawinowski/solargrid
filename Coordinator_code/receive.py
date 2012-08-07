#! /usr/bin/python    
# Import and init an XBee device
from xbee import ZigBee
import serial, logging, sqlite3 as lite, sys   
logging.basicConfig(filename='example.log',level=logging.DEBUG)
ser = serial.Serial('/dev/tty.usbserial-AH0014W9', 9600)
# Use an XBee 802.15.4 device
# To use with an XBee ZigBee device, replace with use:
# xbee = ZigBee(ser)
# otherwise
# xbee = XBee(ser)
logging.debug('This message should go to the log file')
logging.info('So should this')
logging.warning('And this, too')

zb = ZigBee(ser)
address64 = '\x00\x13\xa2\x00\x40\x2c\x7c\xe6'

while True:
	try:
		response = zb.wait_read_frame()
		print response
		if (response["source_addr_long"] == "\x00\x13\xa2\x00@,|\xe6"):
			# we know that data is coming from R1
			house = 1
			print "House Nr. : "+str(house)	
		if (response["source_addr_long"] == "\x00\x13\xa2\x00@0\x10\x83"):
			# we know that data is coming from R2
			house = 2
			print "House Nr. : "+str(house)	
		if (response["source_addr_long"] == "\x00\x13\xa2\x00@,|\xb1"):
			# we know that data is coming from R3
			house = 3
			print "House Nr. : "+str(house)
		if (ord(response.get("rf_data")[1]) == 1):
			#we know that we got sensor information
			print "Data is from a Sensor"	
			if (ord(response.get("rf_data")[1]) == 1):
							# we know that it is Sensor1
							sensor = 1
							char1 = response.get("rf_data")[2]
							char2 = response.get("rf_data")[3]
							num1 = ord(char1)
							num2 = ord(char2)
				   			mAmpere = (num1*256)+num2
							#print int(response.get("rf_data")[0])	
							data = mAmpere
							tablename = "h"+str(house)+"s"+str(sensor)
							print tablename
							con = lite.connect('solargrid.db')
							with con:				
								cur = con.cursor()   
								cur.execute("INSERT INTO  {t} VALUES(time('now','localtime'),date(),?)".format(t=tablename),(data,))     
	except KeyboardInterrupt:
		break
ser.close()
