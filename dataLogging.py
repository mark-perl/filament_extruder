import serial
from datetime import datetime
import os
import time
import csv

# Set com port
com = '/dev/cu.usbmodem101'

try:
    nano = serial.Serial(com, 9600)
    print("Serial OK\n")
except serial.SerialException as e:
    print("Error reading from serial!")
    print(e)
    exit()

file_path = './data'
now = datetime.now()
nowstr = now.strftime("%Y_%m_%d_%H_%M_%S")
name = "Filament_log_" + nowstr + ".csv"
filename = os.path.join(file_path, name)
print(name)

nano.flush()
time.sleep(5)

file = open(filename, 'a+', newline='')
csvwriter = csv.writer(file, dialect='excel', delimiter=',')
header = ["Mode", "Tensioner Speed", "Spooler Speed", "Measured Diameter"]
csvwriter.writerow(header)
print(header)

try:
    while True:
        if nano.in_waiting > 0:
            line = nano.readline().decode('utf-8').strip()
            csvwriter.writerow([line])
            print(line)

except KeyboardInterrupt:
    print("Data logging stopped.")
finally:
    nano.close()
    file.close()

    # save = input("save file y/n: ")
    # if save != 'y':
    #     os.remove(file_path)

    print("Compelte")