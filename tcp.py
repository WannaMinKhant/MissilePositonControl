import socket

serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serv.bind(('192.168.0.101', 5000))
serv.listen(5)
while True:
	conn, addr = serv.accept()
	from_client = ''
	data = conn.recv(4096)
	if not data: break
	from_client += data
	print from_client
    	conn.send(":10:25")
conn.close()
print 'client disconnected'
