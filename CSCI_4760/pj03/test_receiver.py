import logging
import socket
from multiprocessing import Process
from pathlib import Path

from file_transfer.receiver import ReceiverClient
from file_transfer.utils import Address

STATIC_ID = 5
POWER_RANGE = range(30, 5, -1)
CONN_RANGE = range(50, 0, -1)
SERVER_ADDRESS = Address('vcf3', 4000)
TARGET_FILE = Path('large_file.raw')
OUTPUT_FILE = 'data.csv'
TIMEOUT = 20
SAMPLES = 3


def run_receive(chunk_size):
    receiver = ReceiverClient(SERVER_ADDRESS, count=1)
    receiver.receive(chunk_size)


if __name__ == '__main__':

    logging.basicConfig(filename=OUTPUT_FILE)
    # log.setLevel(SCRIPT_LOG_LEVEL)

    s = socket.socket()
    s.bind(('', 0))
    with open('conn_info', 'w') as f:
        f.write(f'{socket.gethostname()} {s.getsockname()[1]}')
    s.listen()
    conn, addr = s.accept()

    for _ in range(SAMPLES):
        for power in POWER_RANGE:
            for num_conn in CONN_RANGE:
                if TARGET_FILE.is_file():
                    TARGET_FILE.unlink()

                chunk_size = 1 << power
                print('testing: ', chunk_size, num_conn)

                recv = Process(target=run_receive, args=(chunk_size,))
                recv.start()

                conn.send(int.to_bytes(num_conn, 32, 'big', signed=False))

                recv.join(timeout=TIMEOUT)
                if recv.is_alive():
                    recv.terminate()
