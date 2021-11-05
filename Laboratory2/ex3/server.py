import socket
import threading
import random
import struct
import sys
import time

random.seed()
start = 0; stop = 1000
chosen_number = random.randint(start, stop)
print("Server number: ", chosen_number);

server_lock = threading.Lock()
client_guessed=False
winner_thread=0
e = threading.Event()
e.clear()
threads = []
client_count=0
min = 1001

#cs - client socket
def client_guess(cs):
    global mylock, client_guessed, chosen_number, winner_thread, client_count, e, min


    my_idcount = client_count
    print("client #", client_count, 'from: ', cs.getpeername(), cs)
    message = 'Hello client #' + str(client_count) + ' !You are entering the competition!\n'
    cs.sendall(bytes(message, 'ascii'))


    while not client_guessed:
        try:

            oldtime = time.time() #get time
            cnumber = cs.recv(4)  #get an integer from the client
            cnumber = struct.unpack('!I', cnumber)[0]
            current_thread = threading.get_ident()
            #print("Client ", current_thread, "sent ", cnumber)


            current_min = abs(chosen_number - cnumber)
            if(current_min < min):
                server_lock.acquire()
                min = current_min
                winner_thread = threading.get_ident()
                server_lock.release()

            # check
            if time.time() - oldtime >= 10:
                server_lock.acquire()
                print("10 seconds have passed. Goodbye\n")
                client_guessed = True
                server_lock.release()
        except socket.error as msg:
            print("Error: ", msg.strerror);
            break

    #print("Minimum error: ", min, " winning thread: ", winner_thread, "\n")
    if client_guessed:
        if threading.get_ident() == winner_thread:
            cs.sendall(b'G')
            print('We have a winner', cs.getpeername())
            print("Thread ", my_idcount, " You have the best guess with an error of ", min)
            e.set()
        else:
            cs.sendall(b'L')
            print("Thread ", my_idcount, " looser")

    cs.close()
    time.sleep(1)


def resetSrv():
    global mylock, client_guessed, winner_thread, my_num, threads,e, client_count
    while True:
        e.wait()
        for t in threads:
            t.join()
        print("all threads are finished now")
        e.clear()
        server_lock.acquire()
        threads = []
        client_guessed = False
        winner_thread=-1
        client_count = 0
        my_num = random.randint(start,stop)
        print('Server number: ',my_num)
        server_lock.release()

if __name__ == '__main__':
    try:
				#TCP socket
        rs = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        rs.bind(('0.0.0.0', 1700))
        rs.listen(5)
    except socket.error as msg:
        print(msg.strerror)
        exit(-1)

    t = threading.Thread(target = resetSrv, daemon = True)
    t.start()

    while True:
        client_socket, addrc = rs.accept()
        t = threading.Thread(target = client_guess, args = (client_socket, ))
        threads.append(t)
        client_count += 1
        t.start()
