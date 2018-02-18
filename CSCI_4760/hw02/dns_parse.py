import argparse
import sys
from pprint import pprint


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

    def skip(self, amount: int):
        self.index += amount

    def read_bytes(self, amount: int):
        """
        Reads raw bytes from the source.

        :param amount: how many bytes to read.
        :return:
        """
        ret = self.raw[self.index: self.index + amount]
        self.index += amount
        return ret

    def read_int(self, byte_length: int, *, signed=False):
        """
        Reads a length of bytes interpreted as a big-endian integer.

        :param byte_length: how many bytes to read.
        :param signed: signed/unsigned integer.
        :return: the integer value of the bytes read.
        """
        return int.from_bytes(self.read_bytes(byte_length), 'big', signed=signed)

    def read_name(self):
        """
        Reads a domain name. Recursively handles pointers.

        :return: the domain name read.
        """

        ret = ''
        type = self.read_int(1)
        while type > 0:
            if type & 0xC0 != 0:
                # is pointer
                temp_index = ((type & 0x3F) << 4) | self.read_int(1)
                current_index = self.index
                self.index = temp_index
                # recursively resolve pointers
                ret += self.read_name()
                self.index = current_index
                return ret
            else:
                # is label
                read_len = type
                ret += self.read_bytes(read_len).decode()
                ret += '.'
            type = self.read_int(1)

        return ret

    def read_address(self, length=4):
        """
        Reads an IP address

        :param length: the RDLENGTH to read
        :return: an IP address split into a tuple
        """
        return tuple(self.read_int(1) for _ in range(length))


parser = argparse.ArgumentParser(description='Parses DNS messages.')
parser.add_argument('--file', '-f', type=argparse.FileType('rb'))
args = parser.parse_args()

with args.file as f:
    raw = f.read()

view = BytesView(raw)

transaction_id = view.read_int(2)
raw_flags = view.read_int(2)
flags = {
    'QR': (raw_flags & (0b1 << 15)) >> 15,
    'OPCODE': (raw_flags & (0b1111 << 11)) >> 11,
    'AA': (raw_flags & (0b1 << 10)) >> 10,
    'TC': (raw_flags & (0b1 << 9)) >> 9,
    'RD': (raw_flags & (0b1 << 8)) >> 8,
    'RA': (raw_flags & (0b1 << 7)) >> 7,
    'Z': (raw_flags & (0b111 << 4)) >> 4,
    'RCODE': (raw_flags & (0b1111 << 0)) >> 0
}
counts = {
    'QD': view.read_int(2),  # question
    'AN': view.read_int(2),  # answer
    'NS': view.read_int(2),  # authority
    'AR': view.read_int(2)  # additional
}

# questions
questions = [
    {
        'name': view.read_name(),
        'qtype': view.read_int(2),
        'qclass': view.read_int(2)
    } for _ in range(counts['QD'])
]


def read_response_record(number):
    """
    Reads a resource record
    :param number: how many to read.
    :return: a dictionary of resource record values.
    """
    records = []
    for _ in range(number):
        rr = {
            'name': view.read_name(),
            'qtype': view.read_int(2),
            'qclass': view.read_int(2),
            'ttl': view.read_int(4),
            'rdlength': view.read_int(2),
        }

        # A record
        if rr['qtype'] == 1:
            rr['rdata'] = view.read_address()
        # NS, CNAME respectively
        elif rr['qtype'] in (2, 5):
            rr['rdata'] = view.read_name()
        # Everything else
        else:
            rr['rdata'] = 'Record type unsupported.'
            view.skip(rr['rdlength'])
            continue
        records.append(rr)
    return records


# resource records
answers = read_response_record(counts['AN'])
auths = read_response_record(counts['NS'])
addi = read_response_record(counts['AR'])

pprint(locals())
