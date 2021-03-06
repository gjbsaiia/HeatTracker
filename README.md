## HeatTracker 
### ---> more recent project that essentially expands on this one [keyboardCar](https://github.com/gjbsaiia/keyboardCar)
### working with a team, I handled the construction/code of the car component.
Heat-Tracking Robot:

The idea for this project was to have a Raspberry Pi that functions as a mobile client.
The RPi has a DHT11 temperature sensor mounted on it, as well as two motors and an
Arduino board. The RPi collects the temperature data and sends the data to the server,
all the while accepting movement commands from an SSH'd computer (that is also the server).
The sensor polling process works in parallel to all movements.

This was accomplished through 3 sets of source code files:

server:
	This is the code that would be run on the computer that is SSH'd to the Pi.
	robo_server.c: Displays and recieves the data sent from the Pi - calculates
				  room temperature at the end. Takes an argument to specify the number
					of data points you want.
	makefile: Compiles robo_server.c

pi:
	This is the code that runs on the Pi.
	robo_shell.c: This is the shell script that runs the robot. Takes a command,
				identifies command, then executes the necessary files in a new process.
	robo_polling.c: Reads sensor and has the client setup. Polls sensor continuosly,
				 sends data for each polling (bad or good, the server distinguishes
				 between good and bad data).
	various python scripts: These scripts are specific to the movement commands that
							you enter into the shell. They send a command serially from
							the Pi to the Arduino which is processed in the motor_control.ino
							script.
	makefile: Compiles robo_shell.c and robo_polling.c

 motor_control:
 	This is the code that runs on the Arduino.
 	motor_control.ino: This controls both motors. This code has 5 states, but starts in
 					  a null state indicated by " mode = 'a' ". This allows the Arduino to sit idle
						until a different state is indicated by a python script which changes the value of "mode".



Hardware:
- Raspberry Pi gen 3
- Arduino UNO board
- DROK L298N Motor Drive Controller Board
- DHT11 Temperature and Humidity Sensor
- uxcell DC 5V 9 rpm motor (x2)
- Portable battery for any smart phone
- various wires, USB to Serial Port cable
