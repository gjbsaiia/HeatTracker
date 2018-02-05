#Griffin Saiia, Gjs64
#routine to put arduino into drive state

import serial
import time

#sets the port of the arduin0 + the baud rate
ser = serial.Serial('/dev/ttyACM0',9600)
#waits on connection
time.sleep(2)
#writes mode number
ser.write(b"1")
time.sleep(1)
#sets 0 state
ser.write(b"a")
#closes connection
ser.close()
