from file_transfer.utils import Address, SocketWrapper


class Client:
    def __init__(self, server_address: Address):
        self.server_address = server_address

        # socket to interact with server
        self.server_conn = SocketWrapper()
        # socket to interact with other clients
        self.client_conn = SocketWrapper()