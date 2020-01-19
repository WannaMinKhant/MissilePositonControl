import socket
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = ('192.168.0.101', 5000)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

# Listen for incoming connections
sock.listen(5)


while True:
    # Wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()
    try:

       data = connection.recv(1024)
       print('received {!r}'.format(data))
       if data:
            if data == "ready\r\n":
		connection.sendall("10:15")
		print('sending data back to the client   10:15')
	    elif data == "busy\r\n":
	   	print('controller busy')
	    	#connection.sendall(data)
       else:
           print('no data from', client_address)
           break

    finally:
        # Clean up the connection
        connection.close()
	#sock.close()
