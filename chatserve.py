#   Author:     Andrius Kelly
#   Date:       Feb 10, 2018
#   Description: CS 372 Project 1 - Server side implimentation of basic chat program.

from socket import *
import sys

myHandle = "HAL-9000"

if not len(sys.argv) == 2:
    print("Usage: chatserv.py [port number]")
    exit(1)
else:
    serverPort = int(sys.argv[1])

#bind the port and address to the current process
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind( ('', serverPort) )   

#sets max connection queue to 1
serverSocket.listen(1)

while True:
    
    print( "listening on port " + str(serverPort) + "..." )
    connectionSocket, addr = serverSocket.accept()
    print("client has entered chat")
    data = connectionSocket.recv(1024)
   
    
    while data:           #chat while connection is open
        clientMessage = data.decode()
        print(clientMessage)
        
        sys.stdout.write( myHandle + "> ")
        userInput = input()
         
        if userInput == "\quit":
            print("you closed chat")
            break
        
        myMessage = "%s> %s" % (myHandle, userInput)
        connectionSocket.send( myMessage.encode() )
        
        data = connectionSocket.recv(1024)
        
        if not data:
            print("client left chat")

    connectionSocket.close()
        
