import socket
import threading
import time
from pathlib import Path
from typing import List

from file_transfer.client import Client
from file_transfer.utils import Address, Consts, SocketWrapper, human_readable
from ftclient import log


class ReceiverClient(Client):
    def __init__(self, server_address: Address):
        super(ReceiverClient, self).__init__(server_address)

        self.client_conn.socket.bind(('', 0))
        self.client_conn.socket.listen()

        # address to accept connections from other clients
        self.client_address = Address(*self.client_conn.socket.getsockname())
        if self.client_address.host == '0.0.0.0':
            self.client_address.host = socket.gethostname()

        self.id = None

        self.threads: List[ReceiverClientThread] = []

    def receive(self, chunk_size, on_id=None):
        try:
            self.server_conn.connect(self.server_address)

            log.debug('Sending receiver message...')
            self.server_conn.send(Consts.IS_RECEIVING)  # send that we are a receiver

            # send our receiving address in the form of <length of hostname><hostname><port>
            # hostname max length=255
            log.debug(f'Sending receiving address {self.client_address}.')
            self.server_conn.send_string(self.client_address.host, len_num_bytes=1)
            self.server_conn.send_int(self.client_address.port, length=2)

            # we should receive an ID if acknowledged.
            self.id = self.server_conn.recv_int(2)
            log.info(f'Assigned ID {self.id}.')
            on_id(self.id)

            while True:
                conn, remote_addr = self.client_conn.socket.accept()
                thread = ReceiverClientThread(SocketWrapper(conn), Address(*remote_addr), chunk_size)
                self.threads.append(thread)
                thread.start()

        except KeyboardInterrupt:
            log.info('Received keyboard interrupt. Waiting for connections to finish...')
            for thread in self.threads:
                if thread.is_alive():
                    thread.join()
        finally:
            self.server_conn.send(b'disconnect')
            self.server_conn.close()
            log.info('Exiting...')


class ReceiverThreadSpawner(threading.Thread):
    def __init__(self, conn: SocketWrapper, address: Address, chunk_size):
        super().__init__()
        self.conn = conn
        self.address = address
        self.chunk_size = chunk_size

    def run(self):
        type = self.conn.recv(1)
        if type == 'a':
            pass


class ReceiverClientThread(threading.Thread):
    receiving = []

    def __init__(self, conn, remote_addr, chunk_size):
        super().__init__()
        self.conn: SocketWrapper = conn
        self.remote_addr: Address = remote_addr
        self.chunk_size = chunk_size

    def run(self):
        # supports up to 2^32 bytes, i.e. 4 GB
        filename = self.conn.recv_string(2)
        offset = self.conn.recv_int(32)
        length = self.conn.recv_int(32)

        log.info(f'Receiving {filename}, size {length} bytes from {self.remote_addr}')
        file = Path(filename)

        # if filename not in self.receiving and file.is_file():
        #     self.conn.send(b'\xFF')
        #     self.conn.close()
        #     log.critical(f'File {filename} already exists.')
        #     log.critical('Aborting...')
        #     return

        self.conn.send(Consts.FILE_NOT_EXISTS)
        self.receiving.append(filename)

        if file.is_file():
            file.unlink()
        if not file.is_file():
            file.touch()

        start = time.time()
        bytes_left = length
        with file.open('rb+') as f:
            f.seek(offset)
            while bytes_left > 0:
                recv_size = min(self.chunk_size, bytes_left)
                chunk = self.conn.recv(recv_size)
                f.write(chunk)
                bytes_left -= len(chunk)
        self.conn.send(Consts.FILE_RECEIVED)  # send done receiving message
        end = time.time()

        log.info(f'Received {human_readable(length)} in {end-start} seconds '
                 f'({human_readable(length/(end-start))}/second).')

        self.conn.close()
