from socket import AF_INET, SOCK_DGRAM
import sys
import socket
import struct, time

host = "pool.ntp.org"
port = 123
read_buffer = 1024
address = (host,port)
data = b'\x1b' + 47 * b'\0'

epoch = 2208988800

client = socket.socket(AF_INET,SOCK_DGRAM)

client.sendto(data, address)
data,address = client.recvfrom(read_buffer)
t = struct.unpack("!12I", data)[10]
t -= epoch

print("time = %s" % time.ctime(t))
