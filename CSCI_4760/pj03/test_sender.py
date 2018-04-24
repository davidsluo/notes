import socket
import time
from pathlib import Path

from file_transfer.sender import SenderClient
from file_transfer.utils import Address
from large_file_generator import generate

SOURCE_FILE = Path('test_files/large_file.raw')
SOURCE_FILE_SIZE = 1 << 30  # 1 GB
SERVER_ADDRESS = Address('vcf3', 4000)

if not SOURCE_FILE.is_file():
    generate(str(SOURCE_FILE), SOURCE_FILE_SIZE)

s = socket.socket()
with open('conn_info', 'r') as f:
    host, port = f.read().split()
    s.connect((host, int(port)))

while True:
    num_conns: int = int.from_bytes(s.recv(32), 'big', signed=False)
    time.sleep(0.05)
    sender = SenderClient(SERVER_ADDRESS)
    sender.send(5, str(SOURCE_FILE), connections=num_conns)
