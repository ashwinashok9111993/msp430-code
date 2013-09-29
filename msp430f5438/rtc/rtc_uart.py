import serial


a = []
ser = serial.Serial("/dev/ttyUSB1",115200)



while(1):
	ser.write(str(raw_input(">>> enter mode s,t,c")))
	print(ser.readline())
		
		
	 
    
