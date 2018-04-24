import itertools
import socket
from enum import Enum


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

    disconnect = close

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


# https://stackoverflow.com/a/1094933
def human_readable(num, suffix='B'):
    for unit in ['', 'Ki', 'Mi', 'Gi', 'Ti', 'Pi', 'Ei', 'Zi']:
        if abs(num) < 1024.0:
            # return "%3.1f%s%s" % (num, unit, suffix)
            return f'{num:3.1f} {unit}{suffix}'
        num /= 1024.0
    # return "%.1f%s%s" % (num, 'Yi', suffix)
    return f'{num:.1f} Yi{suffix}'


# http://www.geekviewpoint.com/python/bitwise/msb
def base_two_round(num):
    i = 0
    while num > 1:
        num >>= 1
        i += 1
    return 1 << i


def divide_into_sections(size, divisions):
    if divisions == 0:
        raise ZeroDivisionError
    elif divisions < 0:
        raise ValueError('Number of divisions cannot be less than zero.')

    section_size = base_two_round(size // divisions)
    sections = [section_size] * divisions
    bytes_left = size - sum(sections)
    i = 0
    while i < len(sections) and bytes_left > section_size:
        sections[i] += section_size
        bytes_left -= section_size
        i += 1
    sections[-1] += bytes_left

    indicies = [0] + list(itertools.accumulate(sections))
    start_end = ((start, end) for start, end in zip(indicies[:-1], indicies[1:]))
    offset_length = [(start, end - start) for start, end in start_end]
    return offset_length


def divide_into_even_sections(size, divisions):
    if divisions == 0:
        raise ZeroDivisionError
    elif divisions < 0:
        raise ValueError('Number of divisions cannot be less than zero.')

    section_size, remainder = divmod(size, divisions)
    sections = [section_size] * (divisions - remainder)
    sections.extend([section_size + 1] * remainder)

    indicies = [0] + list(itertools.accumulate(sections))
    start_end = ((start, end) for start, end in zip(indicies[:-1], indicies[1:]))
    offset_length = [(start, end - start) for start, end in start_end]
    return offset_length


class Consts(bytes, Enum):
    IS_SENDING = b'\x00'
    IS_RECEIVING = b'\x01'

    FILE_DATA = b'\x02'
    META_DATA = b'\x03'

    FILE_EXISTS = b'\x04'
    FILE_NOT_EXISTS = b'\x05'

    FILE_RECEIVED = b'\x06'
    CHUNK_RECEIVED = b'\07'

    DONE_SENDING = b'\x08'
    DONE_RECEIVING = b'\x09'

    DISCONNECTING = b'\x10'

    ERROR = b'\xFF'


SCRIPT_LOG_LEVEL = 100
