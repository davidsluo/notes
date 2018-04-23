import logging
import socket
from multiprocessing import Process
from pathlib import Path

from file_transfer.receiver import ReceiverClient
from file_transfer.utils import Address

static_id = 5


def run_receive(chunk_size):
    receiver = ReceiverClient(Address('vcf3', 4000), count=1)
    receiver.receive(chunk_size)


if __name__ == '__main__':

    logging.basicConfig(filename='data.csv')
    # log.setLevel(SCRIPT_LOG_LEVEL)

    target_file = Path('large_file.raw')

    s = socket.socket()
    s.bind(('', 0))
    with open('conn_info', 'w') as f:
        f.write(f'{socket.gethostname()} {s.getsockname()[1]}')
    s.listen()
    conn, addr = s.accept()

    for power in range(50, 0, -1):
        for num_conn in range(50, 0, -1):
            if target_file.is_file():
                target_file.unlink()

            chunk_size = 1 << power
            print(chunk_size, num_conn)

            recv = Process(target=run_receive, args=(chunk_size,))
            recv.start()

            conn.send(int.to_bytes(num_conn, 32, 'big', signed=False))

            recv.join(timeout=20)
            if recv.is_alive():
                recv.terminate()
