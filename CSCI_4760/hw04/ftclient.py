import argparse
import logging
import socket
import threading
from pathlib import Path

from utils import Address, SocketWrapper

logging.basicConfig(level=logging.DEBUG)
log = logging.getLogger('ftclient')


class Client:
    def __init__(self, server_address: Address):
        self.server_address = server_address

        # socket to interact with server
        self.server_conn = SocketWrapper()
        # socket to interact with other clients
        self.client_conn = SocketWrapper()


class ReceiverClientThread(threading.Thread):
    CHUNK_SIZE = 4096

    def __init__(self, conn, remote_addr):
        super().__init__()
        self.conn: SocketWrapper = conn
        self.remote_addr: Address = remote_addr

    def run(self):
        # supports up to 2^32 bytes, i.e. 4 GB
        filename = self.conn.recv_string(2)
        filesize = self.conn.recv_int(32)

        log.info(f'Receiving {filename}, size {filesize} from {self.remote_addr.host}:{self.remote_addr.port}')
        file = Path(filename)
        if file.is_file():
            self.conn.send(b'\xFF')
            self.conn.close()
            log.critical(f'File {filename} already exists.')
            log.critical('Exiting...')
            return
        self.conn.send(b'\x00')

        with open(filename, 'wb') as f:
            for i in range(0, filesize, self.CHUNK_SIZE):
                recv_size = min(self.CHUNK_SIZE, filesize - i)
                chunk = self.conn.recv(recv_size)
                f.write(chunk)
        self.conn.close()


class ReceiverClient(Client):
    def __init__(self, server_address: Address):
        super(ReceiverClient, self).__init__(server_address)

        self.client_conn.socket.bind(('', 0))
        self.client_conn.socket.listen()

        # address to accept connections from other clients
        self.client_address = Address(*self.client_conn.socket.getsockname())
        if self.client_address.host == '0.0.0.0':
            self.client_address = Address(socket.gethostname(), self.client_address.port)

    def receive(self):
        conn = None
        try:
            self.server_conn.connect(self.server_address)

            log.debug('Sending receiver message...')
            self.server_conn.send(b'\x00')  # send that we are a receiver

            # send our receiving address in the form of <length of hostname><hostname><port>
            # hostname max length=255
            log.debug(f'Sending receiving address {self.client_address.host}:{self.client_address.port}.')
            self.server_conn.send_string(self.client_address.host, len_num_bytes=1)
            self.server_conn.send_int(self.client_address.port, length=2)

            # we should receive an ID if acknowledged.
            id = self.server_conn.recv_int(2)
            log.info(f'Assigned ID {id}.')

            while True:
                conn, remote_addr = self.client_conn.socket.accept()
                thread = ReceiverClientThread(SocketWrapper(conn), Address(*remote_addr))
                thread.start()

        except KeyboardInterrupt:
            log.info('Received keyboard interrupt. Waiting for connections to finish...')
            for thread in threading.enumerate():
                if thread != threading.current_thread():
                    thread.join()
        finally:
            self.server_conn.send(b'disconnect')
            self.server_conn.close()
            log.info('Exiting...')


class SenderClient(Client):
    def send(self, id: int, filename: str):
        try:
            file = Path(filename)
            if not file.is_file():
                log.critical('File {filename} does not exist.')
                log.critical('Exiting...')
                return
            filesize = file.stat().st_size

            self.server_conn.connect(self.server_address)

            log.debug('Sending sender message...')
            self.server_conn.send(b'\xFF')
            log.debug(f'Sending ID {id}.')
            self.server_conn.send_int(id, 2)

            host = self.server_conn.recv_string(1)
            port = self.server_conn.recv_int(2)
            self.server_conn.close()
            log.info(f'Receiving address for {id} at {host}:{port}.')
            receiver_address = Address(host, port)
            if receiver_address == Address('', 0):
                log.critical('ID not found.')
                log.critical('Exiting...')
                return

            self.client_conn.connect(receiver_address)
            log.info(f'Sending file metadata - filename: {file.name}, size: {filesize}...')
            self.client_conn.send_string(file.name, 2)
            self.client_conn.send_int(filesize, 32)

            status = self.client_conn.recv(1)
            if status == b'\xFF':
                log.critical(f'Filename {file.name} already exists at destination.')
                log.critical('Exiting...')
                return

            log.info('Sending file...')
            with file.open('rb') as f:
                self.client_conn.socket.sendfile(f)
            log.info('Disconnecting...')

        except KeyboardInterrupt:
            log.info('Received keyboard interrupt. Exiting...')
        finally:
            self.server_conn.close()
            self.client_conn.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('host', type=str)
    parser.add_argument('port', type=int)
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--receive', action='store_true')
    group.add_argument('--send', metavar='ID filename', nargs=2)
    args = parser.parse_args()

    if args.receive:
        client = ReceiverClient(Address(args.host, args.port))
        client.receive()
    else:
        client = SenderClient(Address(args.host, args.port))
        client.send(int(args.send[0]), args.send[1])
