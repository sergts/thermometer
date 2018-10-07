import bluetooth
import sys
import time
bd_addr = ""

port = 1
sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
sock.connect((bd_addr, port))
print('Connected')

data_points = []
buffer = ''
while True:
    data = sock.recv(2048)
    if len(data) == 0:
    	break
    data = data.decode("utf-8")
    #print('received: ' + data)
    if '\r\n' in data:
        with open('temps.csv', 'a') as f:
            f.write(str(int(time.time())) + ',' + buffer + data[:-2] + '\n')
    else:
        buffer = data

sock.close()
