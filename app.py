#!/usr/python

import socket
import time
from PIL import Image

# HOST = '192.168.1.100'  # IP for production

# PORT = 7  # production port
PORT = 1234  # Test port

# REMOTE_IP = '192.168.1.10'
REMOTE_IP = '127.0.0.1'  # Localhost for testing


# Open Socket
try:
    #                          IPv4             TCP
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except Exception:
    print("Failed to initialize socket!")
    exit(1)

print("Socket created!")
# soc.bind((HOST, PORT))  # bind application and port
# try:
soc.connect((REMOTE_IP, PORT))
print("Waiting for a connection")
# except Exception:
#    print("Failed to establish connection with host!")
#    exit()

buff = bytearray()
try:
    while True:
        # soc.listen()  # Wait for connection
        # conn, addr = soc.accept()  # Accept connection
        # print("Connected by", addr)
        usr_response = input("Send request?[Y/n]")
        if usr_response not in ["", 'Y', 'y']:
            usr_response = input("Exit program?[Y/n]")
            if usr_response in ["", 'Y', 'y']:
                exit()

        message = str.encode("gib")
        soc.send(message)
        print("Request sent!")

        while True:
                # data = conn.recv(1024)
                data = soc.recv(1024)
                buff.extend(data)
                # print(data)
                end = buff[-2:]
                if end == b'\xff\xd9' or not data:
                    break

        imgName = str(time.time())+".jpg"
        rcv = open(imgName, "wb")
        rcv.write(buff)
        rcv.close()
        image = Image.open(imgName)
        image.show()
        image.close()
except KeyboardInterrupt:
    print("exiting")
    exit(1)
