import socket
import json
import random
import threading

# load jokes from json file
with open('jokes.json', 'r') as f:
    jokes = json.load(f)

SERVER_HOST = '0.0.0.0'
SERVER_PORT = 47684
SERVER_ADDR = (SERVER_HOST, SERVER_PORT)

connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connection.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
connection.bind(SERVER_ADDR)
connection.listen(10)

class JokeThread(threading.Thread):
    """Runs a joke connection in a new thread."""
    def __init__(self, client, address):
        """Constructor"""
        threading.Thread.__init__(self)
        self.client = client
        self.address = address

    def run(self):
        """Runs joke procedure."""
        # choose random joke
        joke = random.choice(jokes)
        prompt = joke['prompt']
        response = joke['response']

        # send joke
        data = b''
        while data.strip() != b"WHO'S THERE?":
            self.client.send(b'KNOCK KNOCK\n')
            data = self.client.recv(4096)
        self.client.send((prompt + '\n').encode())
        self.client.recv(4096)
        self.client.send((response + '\n').encode())
        # end connection
        self.client.shutdown(socket.SHUT_RDWR)
        self.client.close()

try:
    while True:
        # accept connection
        client, address = connection.accept()
        # start joke in new thread.
        thread = JokeThread(client, address)
        thread.start()
# exit gracefully
except KeyboardInterrupt:
    print('Waiting for connections to close...')
    for thread in threading.enumerate():
        if thread != threading.current_thread():
            print(f'Closing {thread.address}')
            thread.join()
    print('Exiting...')
