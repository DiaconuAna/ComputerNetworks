from socket import *
import struct

def main():
    port = int(input("Enter port number: "))
    sv_addr = "192.168.100.20"

    try:
        s = socket(AF_INET, SOCK_STREAM)
        s.connect((sv_addr, port))
    except socket.error as err:
        print(err.strerror)
        exit(-1)

    done = False
    while not done:
        N = int(input("Enter size of the array: "))
        s.send(struct.pack('!I',N))

        flag = s.recv(4)
        flag = struct.unpack('!I', flag)[0]

       # print("received ", flag)

        if (flag == 1 or N == 0):
            done = True
            break

        if(N!=0):
            for i in range(0,N):
                nr = float(input("Enter number: "))
                nr = round(nr, 1)
                s.send(struct.pack('!f', nr))



    n = s.recv(4)
    n = struct.unpack('!I', n)[0]
    #print("Client got ", n, "elems.")
    for i in range(0, n):
        nr_a = s.recv(4)
        nr_a = struct.unpack('!f', nr_a)[0]
        nr_a = round(nr_a, 1)
        print(nr_a, end=' ')

main()
