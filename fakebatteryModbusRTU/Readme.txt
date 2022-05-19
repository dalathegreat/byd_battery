Code from devnull, original code was used with an ESP32, and depended on lib emelianov/modbus-esp8266@^4.1.0
Modified quickly for Arduino usage, RX/TX pins and serial type might not match.
To test, you need 2 RS485 to TTL converter units and a usb->serial TTL to test it with a computer
To test on a linux pc, for example: READ 24regs start@300, use command:
mbpoll -b 9600 /dev/ttyUSB1 -a15 -t4 -r300 -0 -c24