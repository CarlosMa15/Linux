'''
Created on Jan 26, 2019
@author: Carlos Martinez
'''

import socket
from _socket import SHUT_RD

host = '127.0.0.1'
port = 50000
size = 1024
message = ''
address = (host,port)

# Create a socket1
socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the socket1 made in the server side
socket.connect(address)

# exit loop condition of user input ‘quit’
while message != 'quit':

    #get user input to send to server side
    message = input("Enter message: ") # GET http://www.google.com/ HTTP/1.1

    # send user input to server and receive in data from the server  
    socket.send(message.encode("utf-8")) # "ascii"))
    data = socket.recv(size)
    print("\n********** Message Received by Server **********")
    print(data.decode("utf-8")) # "ascii"))
    message = "quit" # Remove

# confirm client stopping and close socket1
socket.shutdown(SHUT_RD)
socket.close()
print('Quit')