import threading
import time
from pathlib import Path

from file_transfer.client import Client
from file_transfer.utils import Address, Consts, SocketWrapper, divide_into_sections, human_readable
from ftclient import log


class SenderClient(Client):
    def send(self, id: int, filename: str, *, connections: int = 1, chunk_size: int = 4096):
        try:
            file = Path(filename)
            if not file.is_file():
                log.critical(f'File {filename} does not exist.')
                log.critical('Exiting...')
                return
            filesize = file.stat().st_size

            self.server_conn.connect(self.server_address)

            log.debug('Sending sender message...')
            self.server_conn.send(Consts.IS_SENDING)
            log.debug(f'Sending ID {id}.')
            self.server_conn.send_int(id, 2)

            host = self.server_conn.recv_string(1)
            port = self.server_conn.recv_int(2)
            self.server_conn.close()
            receiver_address = Address(host, port)
            log.info(f'Receiving address for {id} at {receiver_address}.')
            if receiver_address == Address('', 0):
                log.critical('ID not found.')
                log.critical('Exiting...')
                return

            # Ends communication with server, keeps connection open until file is done sending.

            threads = [SenderClientThread(receiver_address, file, offset, length)
                       for offset, length in divide_into_sections(filesize, connections)]

            start = time.time()
            for thread in threads:
                thread.start()
            for thread in threads:
                thread.join()
            end = time.time()

            log.info(f'Sent {human_readable(filesize)} in {end-start} seconds '
                     f'({human_readable(filesize/(end-start))}/second).')

            log.info('Disconnecting...')

        except KeyboardInterrupt:
            log.info('Received keyboard interrupt. Exiting...')
        finally:
            self.server_conn.close()
            self.client_conn.close()


class SenderThreadSpawner(threading.Thread):
    def __init__(self, *args, **kwargs):
        super().__init__()
        self.args = args
        self.kwargs = kwargs


class SenderClientThread(threading.Thread):
    def __init__(self, remote_addr: Address, file: Path, offset, length):
        super(SenderClientThread, self).__init__()
        self.conn: SocketWrapper = SocketWrapper()
        self.remote_addr = remote_addr
        self.file = file
        self.offset = offset
        self.length = length

    def run(self):
        self.conn.connect(self.remote_addr)
        self.conn.send_string(self.file.name, 2)
        self.conn.send_int(self.offset, 32)
        self.conn.send_int(self.length, 32)

        status = self.conn.recv(1)
        if status == Consts.FILE_EXISTS:
            return

        with self.file.open('rb') as f:
            self.conn.socket.sendfile(f, offset=self.offset, count=self.length)
        done = self.conn.recv(1)
        assert done == Consts.FILE_RECEIVED
