import socket
import json
import random

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

while True:
    # choose random joke
    joke = random.choice(jokes)
    prompt = joke['prompt']
    response = joke['response']

    client, address = connection.accept()
    client.send(b'KNOCK KNOCK\n')
    data = client.recv(4096)
    if data.strip() == b"WHO'S THERE?":
        client.send((prompt + '\n').encode())
        client.recv(4096)
        client.send((response + '\n').encode())
    client.shutdown(socket.SHUT_RDWR)
    client.close()

