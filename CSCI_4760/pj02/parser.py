from enum import Flag
from typing import List

from CSCI_4760.pj02.enums import OpCode
from CSCI_4760.pj02.records import ResourceRecord


class BytesView:
    """
    Allows reading and interpreting a byte string.
    """

    def __init__(self, raw: bytes):
        """
        Constructor

        :param raw: the raw bytes
        """
        self.raw = raw
        self.index = 0

    def skip(self, length: int):
        """
        Skip over an amount of bytes.

        :param length: the amount to skip
        """
        self.index += length

    def read_bytes(self, length: int):
        """
        Reads raw bytes from the source.

        :param length: how many bytes to read.
        :return:
        """
        ret = self.raw[self.index: self.index + length]
        self.index += length
        return ret

    def read_int(self, length: int):
        """
        Reads a length of bytes interpreted as an unsigned big-endian integer.

        :param length: how many bytes to read.
        :return: the integer value of the bytes read.
        """
        return int.from_bytes(self.read_bytes(length), byteorder='big', signed=False)


class DNSMessage:
    def __init__(self,
                 transaction_id: int,
                 opcode: OpCode,
                 flags: Flag,
                 rcode: int,
                 questions: List[ResourceRecord],
                 answers: List[ResourceRecord],
                 authorities: List[ResourceRecord],
                 additionals: List[ResourceRecord]):
        self.transaction_id = transaction_id
        self.opcode = opcode
        self.flags = flags
        self.rcode = rcode
        self.questions = questions
        self.answers = answers
        self.authorities = authorities
        self.additionals = additionals

    def __repr__(self):
        return f'<DNSResponse id={self.transaction_id}>'

    def __str__(self):
        pass


class Parser:
    """
    Parses DNS messages from byte strings.
    """

    def __init__(self, raw: bytes):
        self.view = BytesView(raw)

    def parse(self):
        pass

    def read_name(self):
        """
        Reads a domain name. Recursively handles pointers.

        :return: the domain name read.
        """

        ret = ''
        type = self.view.read_int(1)
        while type > 0:
            if type & 0xC0 == 0xC0:
                # is pointer
                temp_index = ((type & 0x3F) << 4) | self.view.read_int(1)
                current_index = self.view.index
                self.view.index = temp_index
                # recursively resolve pointers
                ret += self.read_name()
                self.view.index = current_index
                return ret
            else:
                # is label
                read_len = type
                ret += self.view.read_bytes(read_len).decode()
                ret += '.'
            type = self.view.read_int(1)

        return ret

    def read_address(self):
        """
        Reads an IP address

        :return: an IP address
        """
        return '.'.join(tuple(str(self.view.read_int(1)) for _ in range(4)))
