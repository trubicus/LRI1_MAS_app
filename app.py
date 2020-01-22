#!/usr/python

import socket
import time
from PIL import Image

HOST = '127.0.0.1'  # Localhost for testing
PORT = 1234  # Predefined port


# Open Socket
try:
    #                          IPv4             TCP
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except Exception:
    print("Failed to initialize socket!")
    exit(1)


soc.bind((HOST, PORT))  # bind application and port
print("Waiting for a connection")
buff = bytearray()
try:
    while True:
        soc.listen()  # Wait for connection
        conn, addr = soc.accept()  # Accept connection
        print("Connected by", addr)

        while True:
                data = conn.recv(1024)
                buff.extend(data)
                if not data:
                    break

        imgName = str(time.time())+".pgm"
        rcv = open(imgName, "wb")
        rcv.write(buff)
        rcv.close()
        image = Image.open(imgName)
        image.show()
        image.close()
except KeyboardInterrupt:
    print("exiting")
    exit(1)
