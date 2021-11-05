from socket import *
import threading
import struct
import sys

array = []
threads = []
my_lock = threading.Lock()
stop = 0
done = False
e = threading.Event()
e.clear()


def mergeSort(arr):
    if len(arr) > 1:
        # Finding the mid of the array
        mid = len(arr) // 2
        # Dividing the array elements
        L = arr[:mid]
        # into 2 halves
        R = arr[mid:]
        # Sorting the first half
        mergeSort(L)
        # Sorting the second half
        mergeSort(R)
        i = j = k = 0

        # Copy data to temp arrays L[] and R[]
        while i < len(L) and j < len(R):
            if L[i] < R[j]:
                arr[k] = L[i]
                i += 1
            else:
                arr[k] = R[j]
                j += 1
            k += 1
        # Checking if any element was left
        while i < len(L):
            arr[k] = L[i]
            i += 1
            k += 1
        while j < len(R):
            arr[k] = R[j]
            j += 1
            k += 1


def clientThread(c):
    global my_lock, array, done, e

    print("Thread ", threading.get_ident(), " has entered the client thread")

    while not done:
        N = c.recv(4)
        N = struct.unpack('!I', N)[0]
        #print("N= ",N)

        if N == 0:
            my_lock.acquire()
            print("Client ", threading.get_ident()," has entered 0. Goodbye!")
            done = True
            my_lock.release()

        if done:
            flag = 1
            c.send(struct.pack('!I', flag))
            break
        else:
            flag = 0
            c.send(struct.pack('!I', flag))

        if(N!=0):
            if(done):
                break
            my_lock.acquire()
            for i in range(0,N):
                nr = c.recv(4)
                nr = round(struct.unpack('!f', nr)[0], 1)
                #print("nr = ", nr)
                array.append(nr)
            print(array)
            my_lock.release()

    print("Client ",threading.get_ident()," has finished execution.")
    mergeSort(array)
    size = len(array)
    c.send(struct.pack('!I', size))
    for i in range(0, size):
        nr = array[i]
        nr = round(nr, 1)
        c.send(struct.pack('!f', nr))
    c.close()

def main():
    global threads
    try:
        port = int(input("Enter port: "))
        s = socket(AF_INET, SOCK_STREAM)
        s.bind(("0.0.0.0", port))
        s.listen(5)
    except error as err:
        print(err.strerror)
        exit(-1)

    while(1):
        c, addrc = s.accept()
        t = threading.Thread(target = clientThread, args=(c,))
        threads.append(t)
        t.start()

main()
