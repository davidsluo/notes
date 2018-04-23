import logging
import os
import select
import socket
import threading
import time
from pathlib import Path
from typing import Dict, List

from file_transfer.utils import Address, Consts, SocketWrapper, human_readable

log = logging.getLogger('ftclient')


class ReceiverClient:
    def __init__(self, server_address: Address, count=-1):
        self.server_conn = SocketWrapper()
        self.server_address = server_address

        self.client_conn = SocketWrapper()
        self.client_conn.socket.bind(('', 0))
        self.client_conn.socket.listen()

        self.client_address = Address(*self.client_conn.socket.getsockname())
        if self.client_address.host in ('0.0.0.0', ''):
            self.client_address.host = socket.gethostname()

        self.id = None
        self.threads: List[threading.Thread] = []

        self.files_received = 0
        self.count = count

        self._read_channel, self._write_channel = os.pipe()

    def receive(self, chunk_size):
        try:
            # Get ID from server and send our receiving address.
            log.info('Querying for ID from server.')
            self.server_conn.connect(self.server_address)

            log.debug('Sending receiver message...')
            self.server_conn.send(Consts.IS_RECEIVING)

            log.debug(f'Sending receiving address {self.client_address}.')
            self.server_conn.send_string(self.client_address.host, len_num_bytes=1)
            self.server_conn.send_int(self.client_address.port, length=2)

            self.id = self.server_conn.recv_int(2)
            log.info(f'Assigned ID {self.id}.')

            # Wait for senders to send data
            log.info(f'Waiting for senders...')
            while True:
                # Non-blocking socket.accept(). Blocks instead here (select.select()), so that it can be interrupted by
                # a write to self._write_channel. See `on_done_receiving`.
                # https://stackoverflow.com/a/32735675
                rfds, _, _ = select.select([self.client_conn.socket.fileno(), self._read_channel], [], [])
                if self._read_channel in rfds:
                    self._stop()
                    return
                conn, remote_addr = self.client_conn.socket.accept()
                thread = ReceiverThreadSpawner(SocketWrapper(conn), Address(*remote_addr), chunk_size, self)
                self.threads.append(thread)
                thread.start()
        except KeyboardInterrupt:
            log.info('Received keyboard interrupt. Waiting for connections to finish...')
            self._stop()
        finally:
            log.info('Disconnecting from server.')
            self.server_conn.send(Consts.DISCONNECTING)
            self.server_conn.close()

    def _stop(self):
        for thread in self.threads:
            if thread.is_alive():
                thread.join()

    def on_done_receiving(self):
        self.files_received += 1
        if self.count != -1 and self.files_received == self.count:
            os.write(self._write_channel, b'!')


class ReceiverThreadSpawner(threading.Thread):
    def __init__(self, conn: SocketWrapper, address: Address, chunk_size, receiver):
        super().__init__()
        self.conn = conn
        self.address = address
        self.chunk_size = chunk_size
        self.receiver: ReceiverClient = receiver

    def run(self):
        type = self.conn.recv(1)
        if type == Consts.META_DATA:
            thread = ReceiverMetaThread(self.conn, self.address, self.receiver)
        elif type == Consts.FILE_DATA:
            thread = ReceiverFileThread(self.conn, self.address, self.chunk_size)
        else:
            raise ValueError()

        self.receiver.threads.append(thread)
        thread.start()


class ReceiverMetaThread(threading.Thread):
    receiving: Dict = {}

    def __init__(self, conn: SocketWrapper, address: Address, receiver: ReceiverClient):
        super().__init__()
        self.conn = conn
        self.address = address
        self.receiver = receiver

    def run(self):
        # Receive file metadata
        filename = self.conn.recv_string(2)
        filesize = self.conn.recv_int(32)
        connections = self.conn.recv_int(2)

        log.info(f'Receiving {filename} ({human_readable(filesize)}) from {self.address} '
                 f'over {connections} connections...')

        # Verify that the file does not already exists, and tell sender to stop if so.
        file = Path(filename)
        if filename not in self.receiving and file.is_file():
            self.conn.send(Consts.FILE_EXISTS)
            self.conn.close()
            log.critical('File already exists.')
            log.critical('Aborting.')
            return
        else:
            file.touch()

        # Send the OK to start sending files
        start = time.time()
        self.conn.send(Consts.FILE_NOT_EXISTS)
        self.receiving[filename] = (filesize, connections)

        # Wait for file to finish receiving
        assert self.conn.recv(1) == Consts.DONE_SENDING
        self.conn.send(Consts.DONE_RECEIVING)
        self.conn.close()
        del self.receiving[filename]
        end = time.time()

        assert filesize == file.stat().st_size

        log.info(f'Received {human_readable(filesize)} in {end-start} seconds over {connections} connections '
                 f'({human_readable(filesize/(end-start))}/second).')

        self.receiver.on_done_receiving()


class ReceiverFileThread(threading.Thread):

    def __init__(self, conn, address, chunk_size):
        super().__init__()
        self.conn: SocketWrapper = conn
        self.address: Address = address
        self.chunk_size = chunk_size

    def run(self):
        # supports up to 2^32 bytes, i.e. 4 GB
        filename = self.conn.recv_string(2)
        length = self.conn.recv_int(32)
        offset = self.conn.recv_int(32)

        file = Path(filename)

        bytes_left = length
        with file.open('rb+') as f:
            f.seek(offset)
            while bytes_left > 0:
                recv_size = min(self.chunk_size, bytes_left)
                chunk = self.conn.recv(recv_size)
                f.write(chunk)
                bytes_left -= len(chunk)

        self.conn.send(Consts.CHUNK_RECEIVED)
        self.conn.close()
