

import serial



ser = serial.Serial("/dev/ttyUSB0",115200)



while(1):
	ser.write('y')
	print(ord(ser.read(1)))
		
		
	 
    


 
