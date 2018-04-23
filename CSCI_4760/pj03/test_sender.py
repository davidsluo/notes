import socket
import time
from pathlib import Path

from file_transfer.sender import SenderClient
from file_transfer.utils import Address
from large_file_generator import generate

source_file = Path('test_files/large_file.raw')
if not source_file.is_file():
    generate(str(source_file), 1 << 30)

s = socket.socket()
with open('conn_info', 'r') as f:
    host, port = f.read().split()
    s.connect((host, int(port)))

while True:
    num_conns: int = int.from_bytes(s.recv(32), 'big', signed=False)
    time.sleep(1)
    sender = SenderClient(Address('vcf3', 4000))
    sender.send(5, str(source_file), connections=num_conns)
