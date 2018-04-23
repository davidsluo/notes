import logging
import threading
import time
from pathlib import Path

from file_transfer.utils import Address, Consts, SocketWrapper, divide_into_sections, human_readable

log = logging.getLogger('ftclient')


class SenderClient:

    def __init__(self, server_address):
        self.server_conn = SocketWrapper()
        self.server_address: Address = server_address

        self.client_conn = SocketWrapper()
        self.client_address: Address = None

    def send(self, id: int, filename: str, *, connections: int = 1):
        try:
            # Verify that file exists
            file = Path(filename)
            if not file.is_file():
                log.critical(f'File {filename} does not exist.')
                log.critical('Exiting...')
                return
            filesize = file.stat().st_size

            # Connect to server to obtain receiver address
            self.server_conn.connect(self.server_address)

            self.server_conn.send(Consts.IS_SENDING)
            self.server_conn.send_int(id, 2)

            host = self.server_conn.recv_string(1)
            port = self.server_conn.recv_int(2)
            self.server_conn.close()
            self.client_address = Address(host, port)
            if self.client_address == Address('', 0):
                log.critical('ID not found.')
                log.critical('Exiting...')
                return
            else:
                log.info(f'Received address for {id} at {self.client_address}.')

            # Ends communication with server, keeps connection open until file is done sending.

            # Begin communications with receiver client
            self.client_conn.connect(self.client_address)

            self.client_conn.send(Consts.META_DATA)

            # Send Metadata
            self.client_conn.send_string(file.name, 2)
            self.client_conn.send_int(filesize, 32)
            self.client_conn.send_int(connections, 2)

            # Verify that file does not already exist at destination and stop if so.
            status = self.client_conn.recv(1)
            if status == Consts.FILE_EXISTS:
                log.critical('File exists at destination.')
                log.critical('Aborting.')
                return
            else:
                # Got the OK to start sending files
                assert status == Consts.FILE_NOT_EXISTS

            threads = [SenderDataThread(self.client_address, file, offset, length)
                       for offset, length in divide_into_sections(filesize, connections)]

            start = time.time()
            for thread in threads:
                thread.start()
            for thread in threads:
                thread.join()
            self.client_conn.send(Consts.DONE_SENDING)
            assert self.client_conn.recv(1) == Consts.DONE_RECEIVING
            end = time.time()

            log.info(f'Sent {human_readable(filesize)} in {end-start} seconds over {connections} connections '
                     f'({human_readable(filesize/(end-start))}/second).')

        except KeyboardInterrupt:
            log.info('Received keyboard interrupt. Exiting...')
        finally:
            log.info('Disconnecting from server.')
            self.server_conn.close()
            self.client_conn.close()


class SenderDataThread(threading.Thread):
    def __init__(self, address: Address, file: Path, offset, length):
        super().__init__()
        self.conn: SocketWrapper = SocketWrapper()
        self.address = address
        self.file = file
        self.offset = offset
        self.length = length

    def run(self):
        self.conn.connect(self.address)
        self.conn.send(Consts.FILE_DATA)
        self.conn.send_string(self.file.name, 2)
        self.conn.send_int(self.length, 32)
        self.conn.send_int(self.offset, 32)

        with self.file.open('rb') as f:
            self.conn.socket.sendfile(f, offset=self.offset, count=self.length)

        done = self.conn.recv(1)
        assert done == Consts.CHUNK_RECEIVED
        self.conn.close()
