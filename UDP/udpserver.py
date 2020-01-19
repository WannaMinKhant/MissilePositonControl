import socket
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the port
server_address = ('192.168.0.101', 5000)
print >>sys.stderr, 'starting up on %s port %s' % server_address
sock.bind(server_address)

check = 1

while True:
    print >>sys.stderr, '\nwaiting to receive message'
    data, address = sock.recvfrom(4096)
    
    print >>sys.stderr, 'received %s bytes from %s' % (len(data), address)
    print >>sys.stderr, data
    
    if check == 1:
        sent = sock.sendto('25:20', address)
        print >>sys.stderr, 'sent %s bytes back to %s' % (sent, address)
	check = 2
    else:
	sent = sock.sendto('20:25', address)
	print >>sys.stderr, 'sent %s bytes back to %s' % (sent, address)
	check = 1
