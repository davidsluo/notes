import argparse
import logging
import random
import socket
import threading
from typing import Dict

from utils import Address, SocketWrapper

logging.basicConfig(level=logging.DEBUG)
log = logging.getLogger('ftserver')


class ServerThread(threading.Thread):
    def __init__(self, conn: SocketWrapper, address: Address, server: 'Server'):
        super().__init__()
        self.conn = conn
        self.address = address
        self.server = server
        self.name = f'{address.host}:{address.port}'

    def run(self):
        log.debug(f'Accepted connection from {self.address.host}:{self.address.port}.')
        mode = self.conn.recv(1)
        # client is receiving
        if mode == b'\x00':
            # get receiving address
            host = self.conn.recv_string(1)
            port = self.conn.recv_int(2)
            receiving_address = Address(host, port)
            log.debug(f'{self.address.host}:{self.address.port} is receiving at'
                      f' {receiving_address.host}:{receiving_address.port}.')

            # Generate and send ID
            id = random.randint(0, 0xFFFF)
            while id in self.server.receivers:
                id = random.randint(0, 0xFFFF)
            self.conn.send_int(id, 2)

            self.server.receivers[id] = receiving_address
            log.debug(f'{self.address.host}:{self.address.port} assigned ID {id}.')

            # keep connection alive until receiver requests disconnect.
            while True:
                try:
                    disconnect = self.conn.recv(10)
                    if disconnect == b'disconnect':
                        log.debug(f'{self.address.host}:{self.address.port} disconnected.')
                        break
                except ConnectionError as er:
                    log.debug(f'{self.address.host}:{self.address.port} disconnected with error.')
                finally:
                    self.server.receivers.pop(id)

        elif mode == b'\xFF':  # client is sending
            log.debug(f'{self.address.host}:{self.address.port} is sending.')

            # Get ID
            id = self.conn.recv_int(2)
            log.debug(f'{self.address.host}:{self.address.port} provided ID {id}.')

            try:
                # Send receiving address
                receiving_address = self.server.receivers[id]
                self.conn.send_string(receiving_address.host, 1)
                self.conn.send_int(receiving_address.port, 2)
                log.debug(f'{self.address.host}:{self.address.port} directed to'
                          f' {receiving_address.host}:{receiving_address.port}.')
            except KeyError:
                # No client receiving on provided ID. Send error and close connection.
                self.conn.send_string('', 1)
                self.conn.send_int(0, 2)
                log.debug(f'{self.address.host}:{self.address.port} provided invalid ID {id}.')

        # disconnect
        self.conn.close()


class Server:
    def __init__(self, address: Address):
        self.address = address

        self.conn = SocketWrapper()
        self.conn.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.conn.socket.bind(self.address)
        self.conn.socket.listen()

        self.receivers: Dict[int, Address] = {}

        log.info(f'Server initialized on {address[0]}:{address[1]}.')

    def serve(self):
        log.info('Serving...')
        try:
            while True:
                connection, address = self.conn.socket.accept()
                thread = ServerThread(SocketWrapper(connection), Address(*address), self)
                thread.start()
        except KeyboardInterrupt:
            log.info('Received keyboard interrupt. Waiting for connections to finish...')
            for thread in threading.enumerate():
                if thread != threading.current_thread():
                    thread.join()
        finally:
            self.conn.close()
            log.info('Exiting...')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('host', type=str)
    parser.add_argument('port', type=int)
    args = parser.parse_args()

    server = Server((args.host, args.port))
    server.serve()
