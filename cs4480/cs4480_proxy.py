'''
Created on Jan 26, 2019
@author: Carlos Martinez

This python class was designed to act like a proxy.
This proxy only sends 1 message it receives from
the client send it to the remote server then sends
the respond it receives from the remote server to
the client. This proxy only can handle 1 message at
the moment.

The proxy was designed to handle multiply clients. The
proxy now does a check on the headers and sends them
over in the request. The proxy Also takes the resonse
from the remote server and changes the response to any
use of the word simple it gets changed to the word silly
'''

# Import Statements
import socket
import threading
import sys
import hashlib

'''
This is the list of the common request headers that are accepted in this proxy
'''
properHeaders = ["A-IM","Accept","Accept-Charset","Accept-Encoding"
                 ,"Accept-Language","Accept-Datetime","Access-Control-Request-Method"
                 ,"Access-Control-Request-Headers","Authorization","Cache-Control"
                 ,"Connection","Content-Length","Content-MD5","Content-Type","Cookie"
                 ,"Date","Expect","Forwarded","From","Host","HTTP2-Settings","If-Match"
                 ,"If-Modified-Since","If-None-Match","If-Range","If-Unmodified-Since"
                 ,"Max-Forwards","Origin","Pragma","Proxy-Authorization","Range"
                 ,"Referer","TE","User-Agent","Upgrade","Via","Warning"]

host = '127.0.0.1'

# The port is read if provided, other wise 50005 is default
try:
    port = int(sys.argv[1])
except IndexError:
    port = 50005
    
# The key is read in if provided, other wise a default is provided
try:
    key = sys.argv[2]
except IndexError:
    key = "cc6745e122995e742da4399a512676491f092f4ec9d58b9dd4baefef8d1726ce"

# Create a socket
socket1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the current host name and port number designated
socket1.bind((host, port))

# Socket starts Listening
socket1.listen()
print("Proxy Server listening!")

'''
'''
def isProperHeader(header):
    for i in range(0,len(properHeaders)):
        if header == properHeaders[i]:
            return True
    
    return False

'''
This method takes care of the functionality that happens when a
new client connects to the proxy server
'''
def clientthread(client):
    
    validExpression = True
    extentionValadility = False
    
    # Receiving Data and Processing message
    data = client.recv(1000000)    
    data = data.decode("utf-8")
    print("Proxy Received ",data)
    
    # More miner checks of the input from the client and braking it down    
    if "\r\n" in data[len(data) - 4:]:
        validExpression = False
    
    headers = data.split("\\r\\n")
    data = headers[0]
    
    parts = data.split()
    if len(parts) != 3:
        validExpression = False
    

    # A few miner checks of input from the client
    if "HTTP/1.0" not in parts[2] and "HTTP/1.1" not in parts[2]:
        validExpression = False
    if "GET" not in parts[0]:
        validExpression = False
    if "/~" in data:
        extentionValadility =  True
        
    # This checks that they are proper headers
    print("HEADERS SIZE: ",len(headers))
    for i in range(1,len(headers)):
        element = headers[i].split(":")
        if len(headers[i]) != 0 and validExpression:
            validExpression = isProperHeader(element[0])
        if len(element) != 2 and len(headers[i]) != 0 and validExpression:
            validExpression = False;
    
    if validExpression:
        # Getting the host  
        message = parts[1][7:]
        middleSection = "/"

        # Checking the middle section of the GET command and fixing the host
        if extentionValadility:
            secondParts = message.split("/~")
            message = secondParts[0]
            middleSection = "/~" + secondParts[1]
        else:
            message = message[:len(message) - 1]
        
        # Server turns to clients and connect to server
        socket2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        # Gets the port number or uses the default port 80
        port1 = 80
        if ":" in message:
            sections = message.split(":")
            message = sections[0]
            port1 = int(sections[1])
        
        print(message)
        # Gets the address
        address1 = (message,port1)
        
        # Creates the message to the remote server
        message = parts[0] + " " + middleSection + " " + parts[2] + "\r\nHost: " + message
        message = message + "\r\nConnection: close\r\nAccept-Encoding: None\r\n"
        
        for i in range(1,len(headers)):
            element = headers[i].split(":")
            if len(headers[i]) != 0 and element[0] != "Connection" and element[0] != "Accept-Encoding":
                message = message + headers[i] + "\r\n"
                
        message = message + "\r\n"
                
        try:
            #try to connect to the remote server
            socket2.connect(address1)
    
            # Send message to remote server
            print("********** Message To Remote Server **********")
            print(message)
            socket2.send(message.encode("utf-8"))
            
            # Reads in all the message from remote server
            buffer = socket2.recv(1024)
            data = buffer
            while len(buffer) > 0:
                buffer = socket2.recv(1024)
                data = data + buffer
                
            data = data.decode("utf-8")
    
            # Closing connection to remote server
            socket2.close()
        except ConnectionRefusedError:
            data = "HTTP/1.1 400 Bad Request\r\n"
    
    else:
        data = "HTTP/1.1 400 Bad Request\r\n"
    
    # send message to client
    print("********** Message Received from Remote Server **********")
    print(data)
    
    # Turns the out from Simple to Silly or simple to silly
    parts = data.split("\r\n\r\n")
    if len(parts) == 2:
        secondPart = parts[1].replace("Simple","Silly")
        secondPart = secondPart.replace("simple","silly")
        data = parts[0] + "\r\n\r\n"  + secondPart
        
    print("********** Message Modified from  Remote Server **********")
    print(data)
    
    # WalWare Check
    # Server turns to clients and connect to server
#     socket3 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#     host3 = 'www.virustotal.com'
#     port3 = 80
#     address3 = (host3,port3)
#     socket3.connect(address3)
#     m = hashlib.md5()
#     m.update(data.encode("utf-8"))
#     resource = m.hexdigest()
#     print("Resource: ",resource)
#     # message3 = "GET \ https://www.virustotal.com/vtapi/v2/file/report?apikey=<"+ key + ">&resource=<" + resource + ">"
#     message3 = "GET /vtapi/v2/file/report?apikey=<" + key + ">&resource=<"
#     message3 = message3 + resource + ">" + " HTTP/1.1" + "\r\nHost: " + host3
#     message3 = message3 + "\r\nConnection: close\r\nAccept-Encoding: None\r\n\r\n"
#     print("message3: ",message3)
#     socket3.send(message3.encode("utf-8"))
#        
#     buffer = socket3.recv(1024)
#     data = buffer
#     while len(buffer) > 0:
#         buffer = socket3.recv(1024)
#         data = data + buffer
#     
#     data = data.decode("utf-8")
#     
#     print("********** Message from Virus Server **********")
#     print(data)
        
    # Sends the modified input from remote server to client
    client.send(data.encode("utf-8"))
    
    # Closes the client connection
    client.close()

# Listens for new clients and handles then in a new thread
while True:
    # Socket accepting connections
    client, address = socket1.accept()
    print("Client Connected")
    thread1 = threading.Thread(target = clientthread, args=(client,))
    thread1.start()

# Confirm Server Closing
socket1.close()
print('Quit')