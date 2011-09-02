#------------------------------------------------------------------------------
# Simple Websocket Server for the hixie-76 protocol only(!)
#
# Author: Martin Hecher
# E-Mail: martin.hecher@fraunhofer.sg
#------------------------------------------------------------------------------

import socket
import sys
import threading

class WebSocketServer:
    
    clients      = []
    host         = "0.0.0.0"
    port         = 8080
    
    def init(self, host, port):
        self.host = host
        self.port = port
        
        socket = socket.socket(socket.AF_INET, socket.socket_STREAM)
        socket.setsocketopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # listen for upto 50 cnxns on port gPort
        self.socket.bind((host, port))
        self.socket.listen(50)
    
        print('Listening on %s:%s. This is a blocking call!' % (self.host, self.port))
    
        clientsocket,clientaddress = socket.accept()
        print 'Connection from: ', clientaddress
        self.clients.append((clientsocket, clientaddress))
        
        # Start a thread to service each cnxn
        t = threading.Thread(target=self.handle, args=(clientsocket,))
        t.start()
    
    def handle(csocket):
        shake1 = csocket.recv(1024)
    
        print 'ClientHandshake:'
        print shake1
    
        shakelist = shake1.split('\r\n')
        # The body follows a \r\n after the 'headers'
        body = shake1.split('\r\n\r\n')[1]
    
        # Extract key1 and key2
        for elem in shakelist:
            if elem.startswith('Sec-Websocketet-Key1:'):
                key1 = elem[20:]  # Sec-Websocketet-Key1: is 20 chars
            elif elem.startswith('Sec-Websocketet-Key2:'):
                key2 = elem[20:]
            else:
                continue
    
        # Count spaces
        nums1 = key1.count(' ')
        nums2 = key2.count(' ')
        # Join digits in the key
        num1 = ''.join([x for x in key1 if x.isdigit()])
        num2 = ''.join([x for x in key2 if x.isdigit()])
    
        # Divide the digits by the num of spaces
        key1 = int(int(num1)/int(nums1))
        key2 = int(int(num2)/int(nums2))
    
        # Pack into Network byte ordered 32 bit ints
        import struct
        key1 = struct.pack('!I', key1)
        key2 = struct.pack('!I', key2)
    
        # Concat key1, key2, and the the body of the client handshake and take the md5 sum of it
        key = key1 + key2 + body
        import hashlib
        m = hashlib.md5()
        m.update(key)
        d = m.digest()
    
        serverHandshake  = 'HTTP/1.1 101 Websocketet Protocol Handshake\r\n'
        serverHandshake += 'Upgrade: Websocketet\r\n'
        serverHandshake += 'Connection: Upgrade\r\n'
        serverHandshake += 'Sec-Websocketet-Origin: http://localhost:9023\r\n'
        serverHandshake += 'Sec-Websocketet-Location: ws://localhost:9999/\r\n'
        serverHandshake += '\r\n'
    
        print 'ServerHandshake'
        print serverHandshake
    
        csocket.send(serverHandshake)
        csocket.send(d)
    
        # Send 'headers'
        #csocket.send('HTTP/1.1 101 Websocketet Protocol Handshake\r\n')
        #csocket.send('Upgrade: Websocketet\r\n')
        #csocket.send('Connection: Upgrade\r\n')
    
        # Firefox settings localhost
        #csocket.send('Sec-Websocketet-Origin: file://\r\n')
        #csocket.send('Sec-Websocketet-Location: ws://localhost:9999/\r\n')
    
        # iPad settings:
        #csocket.send('Sec-Websocketet-Origin: http://www3.ntu.edu.sg\r\n')
        #csocket.send('Sec-Websocketet-Location: ws://155.69.99.99:9023/\r\n')
        #csocket.send('Sec-Websocketet-Protocol: chat\r\n')
        #csocket.send('\r\n')
        #Send digest
        #csocket.send(d)
    
        # Message framing - 0x00 utf-8-encoded-body 0xFF
        def send(data):
            first_byte = chr(0x00)
            payload = data.encode('utf-8')
            pl = first_byte + payload + chr(0xFF)
            csocket.send(pl)
    
    
        from time import sleep
    
        i = 0
        time = 1
        while True:
            #self.translation = osg.Vec3f(sin(time.clock()),0,0)
            send(u'CONNECTION ALIVE %s' % (i))
            i += 1
            sleep(time)
            buf = csocket.recv(1024)
            l = buf.split('\xff')
            buf = l[0]
            buf = buf.strip('\x00')
            self.coords = buf
            #self.coords = str(buf[0]) + ' ' + str(buf[1]) + ' ' + str(buf[2])
            #print('Sending: %s' % buf)
            self.hasNewData = True
            time = 0.01
