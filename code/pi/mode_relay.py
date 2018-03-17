#Griffin Saiia, Gjs64
#routine to send all states through to arduino
import serial
import time
import os
import sys

def main():
    #sets the port of the arduino + the baud rate
    ser = serial.Serial('/dev/ttyACM0',9600)
    #path for FIFO
    FIFO = "/tmp/myfifo"
    #control loop
    while True:
        with open(FIFO) as fifo:
            print("pipe open")
            while True:
                #checks state
                state = fifo.read()
                if len(state) == 0:
                    print("no state in pipe.")
                    break
                print(b'{0}'.format(state))
                ser.write(b'{0}'.format(state))
                time.sleep(1)
                if state == "-1":
                    raise KeyboardInterrupt
                break
    ser.close()
    os.remove("/tmp/myfifo")

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print 'Interrupted'
        try:
            ser = serial.Serial('/dev/ttyACM0',9600)
            ser.close()
            os.remove("/tmp/myfifo")
            sys.exit(0)
        except SystemExit:
            os._exit(0)serial
