import logging
import random
import socket
import threading
from typing import Dict, List

from file_transfer.utils import Address, Consts, SocketWrapper

log = logging.getLogger('ftserver')


class Server:
    threads: List['ServerThread'] = []

    def __init__(self, address: Address):
        self.address = address

        self.conn = SocketWrapper()
        self.conn.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.conn.socket.bind((self.address.host, self.address.port))
        self.conn.socket.listen()

        self.receivers: Dict[int, Address] = {}

        log.info(f'Server initialized on {address}.')

    def serve(self):
        log.info('Serving...')
        try:
            while True:
                connection, address = self.conn.socket.accept()
                thread = ServerThread(SocketWrapper(connection), Address(*address), self)
                self.threads.append(thread)
                thread.start()
        except KeyboardInterrupt:
            log.info('Received keyboard interrupt. Waiting for connections to finish...')
            for thread in self.threads:
                if thread.is_alive():
                    thread.join()
        finally:
            self.conn.close()
            log.info('Exiting...')


class ServerThread(threading.Thread):
    def __init__(self, conn: SocketWrapper, address: Address, server: 'Server'):
        super().__init__()
        self.conn = conn
        self.address = address
        self.server = server
        self.name = str(self.address)

    def run(self):
        log.debug(f'Accepted connection from {self.address}.')
        mode = self.conn.recv(1)
        # client is receiving
        if mode == Consts.IS_RECEIVING:
            # get receiving address
            host = self.conn.recv_string(1)
            port = self.conn.recv_int(2)
            receiving_address = Address(host, port)
            log.debug(f'{self.address} is receiving at {receiving_address}.')

            # Generate and send ID
            id = random.randint(0, 0xFFFF)
            while id in self.server.receivers:
                id = random.randint(0, 0xFFFF)
            self.conn.send_int(id, 2)

            self.server.receivers[id] = receiving_address
            log.debug(f'{self.address} assigned ID {id}.')

            # keep connection alive until receiver requests disconnect.
            while True:
                try:
                    disconnect = self.conn.recv(10)
                    if disconnect == Consts.DISCONNECTING:
                        log.debug(f'{self.address} disconnected.')
                        break
                except ConnectionError as er:
                    log.debug(f'{self.address} disconnected with error.')
                finally:
                    self.server.receivers.pop(id)

        elif mode == Consts.IS_SENDING:  # client is sending
            log.debug(f'{self.address} is sending.')

            # Get ID
            id = self.conn.recv_int(2)
            log.debug(f'{self.address} provided ID {id}.')

            try:
                # Send receiving address
                receiving_address = self.server.receivers[id]
                self.conn.send_string(receiving_address.host, 1)
                self.conn.send_int(receiving_address.port, 2)
                log.debug(f'{self.address} directed to {receiving_address}.')
            except KeyError:
                # No client receiving on provided ID. Send error and close connection.
                self.conn.send_string('', 1)
                self.conn.send_int(0, 2)
                log.debug(f'{self.address} provided invalid ID {id}.')

        # disconnect
        self.conn.close()
        self.server.threads.remove(self)
