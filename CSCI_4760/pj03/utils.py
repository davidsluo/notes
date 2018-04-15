import socket


class Address:
    def __init__(self, host: str, port: int):
        self.host = host
        self.port = port

    def __str__(self):
        return f'{self.host}:{self.port}'


class SocketWrapper:
    """
    Wrapper around socket methods for convenience and error checking.
    """

    def __init__(self, sock: socket.socket = None):
        self.socket = sock or socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self, address: Address):
        """
        Wrapper method for socket.connect()

        :param address: The address to connect to.
        :return:
        """
        return self.socket.connect((address.host, address.port))

    def close(self):
        return self.socket.close()

    def send(self, bytes):
        """
        Wrapper method for socket.send().

        :param bytes: Bytes to send.
        :return: Number of bytes sent.
        """
        num_sent = self.socket.send(bytes)
        assert num_sent == len(bytes)  # todo: is a good way to verify?
        return num_sent

    def recv(self, buffersize) -> bytes:
        """
        Wrapper method for socket.recv().

        :param buffersize: Number of bytes to receive.
        :return: The bytes received.
        """
        return self.socket.recv(buffersize)

    def send_int(self, integer: int, length):
        """
        Send an integer.

        :param integer: The integer.
        :param length: Number of bytes the integer takes up.
        :return:
        """
        return self.send(integer.to_bytes(length, 'big', signed=False))

    def recv_int(self, length) -> int:
        """
        Receives an integer.

        :param length: Number of bytes the integer takes up.
        :return: The integer.
        """
        return int.from_bytes(self.recv(length), 'big', signed=False)

    def send_string(self, string: str, len_num_bytes):
        """
        Send a string

        :param string: The string
        :param len_num_bytes: The number of bytes to read for the length of the string.
        :return:
        """
        encoded = string.encode()
        self.send_int(len(encoded), length=len_num_bytes)
        self.send(encoded)

    def recv_string(self, len_num_bytes) -> str:
        """
        Receive a string.

        :param len_num_bytes: The number of bytes to read for the length of the string.
        :return:
        """
        str_len = self.recv_int(len_num_bytes)
        encoded = self.recv(str_len)
        return encoded.decode()
