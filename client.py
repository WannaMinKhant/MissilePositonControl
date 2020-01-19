import socket

host = "192.168.0.101"
port = 5000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
s.sendall('Ready\r\n')
data = s.recv(1024)
s.close()
print('Receive :', repr(data))
