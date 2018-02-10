from socket import *
import sys

myHandle = "HAL-9000"

if not len(sys.argv) == 2:
    serverPort = 33330
else:
    serverPort = int(sys.argv[1])

#bind the port and address to the current process
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind( ('', serverPort) )   

#sets max connection queue to 1
serverSocket.listen(1)

print( "Listening on port " + str(serverPort) )

while True:
    
    connectionSocket, addr = serverSocket.accept()
    data = connectionSocket.recv(1024)
    
    while data :
        clientMessage = data.decode()
        print(clientMessage)
        
        sys.stdout.write( myHandle + "> ")
        userInput = input()
        
        print(userInput)
        
        if userInput == "\quit":
            break
        
        myMessage = "%s> %s" % (myHandle, userInput)
        print(myMessage)
        connectionSocket.send( myMessage.encode() )
        
        data = connectionSocket.recv(1024)
    

    print("client left chat")
    connectionSocket.close()
        
