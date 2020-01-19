import socket

 

# Create a server socket

serverSocket = socket.socket()

print("Server socket created")

 

# Associate the server socket with the IP and Port

ip      = "192.168.0.101"

port    = 5000

serverSocket.bind((ip, port))

print("Server socket bound with with ip {} port {}".format(ip, port))

 

# Make the server listen for incoming connections

serverSocket.listen(5)

 

# Server incoming connections "one by one"

count = 0

while(True):

    (clientConnection, clientAddress) = serverSocket.accept()

    count = count + 1

    print("Accepted {} connections so far".format(count))

   

    # read from client connection

    while(True):

        data = clientConnection.recv(1024)

        print(data)

 

        if(data!=b''):

            msg1            = "Hi Client! Read everything you sent"

            msg1Bytes       = str.encode(msg1)           

           

            msg2            = "Now I will close your connection"

            msg2Bytes       = str.encode(msg2) 

           

            clientConnection.send(msg1Bytes)

            clientConnection.send(msg2Bytes)

 
	    clientConnection.close()
            print("Connection closed")

            break

serverSocket.close()
