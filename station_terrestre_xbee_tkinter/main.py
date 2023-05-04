import serial
import json

device = serial.Serial()
device.baudrate = 38400
device.port = 'COM3'
print(device.name)
device.open()

x = ""

while True:
     while device.in_waiting:
          mes = device.readline().decode("utf-8")

          if not "EOF" in mes and not "SOF" in mes:
               x = x + mes

          elif "EOF" in mes:
               x = "{"+x+"}"
               f = open("data.json", mode="w", newline='')
               f.write(x)
               f.close
               x = ""
               print("written")
