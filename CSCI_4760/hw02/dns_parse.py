import copy
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

    #     self._sub_index = 0
    #
    # @property
    # def sub_index(self):
    #     return self._sub_index
    #
    # @sub_index.setter
    # def sub_index(self, sub_index: int):
    #     bytes_add, sub_add = divmod(sub_index, 8)
    #     self._sub_index += sub_add
    #     self.index += bytes_add

    def read_bytes(self, amount: int):
        """
        Reads raw bytes from the source.

        :param amount: how many bytes to read.
        :return:
        """
        ret = self.raw[self.index: self.index + amount]
        self.index += amount
        return ret

    # def read_bits(self, amount: int = 1):
    #     mask = 0b1
    #     for _ in range(amount):
    #         mask <<= 1
    #         mask |= 1
    #
    #     b = self.read_bytes(amount % 8 + 1)
    #
    #     return

    def read_int(self, byte_length: int, *, signed=False):
        """
        Reads a length of bytes interpreted as a big-endian integer.

        :param byte_length: how many bytes to read.
        :param signed: signed/unsigned integer.
        :return: the integer value of the bytes read.
        """
        return int.from_bytes(self.read_bytes(byte_length), 'big', signed=signed)

    def read_name(self, *, check_pointer=True):
        """
        Reads a domain name. Handles pointers.

        :return: the domain name read.
        """

        def read(length):
            """
            Helper method to read labels.
            """
            # nonlocal self
            ret = ''
            while length > 0:
                ret += self.read_bytes(length).decode()
                ret += '.'
                length = self.read_int(1)
            return ret

        ret = None
        if check_pointer:
            type = self.read_int(1)
            if type & 0xC0 != 0:
                # is pointer
                temp_index = ((type & 0x3F) << 4) | self.read_int(1)
                current_index = self.index
                self.index = temp_index
                ret = self.read_name(check_pointer=False)
                self.index = current_index
            else:
                # is label
                read_len = type
                ret = read(read_len)
        else:
            read_len = self.read_int(1)
            ret = read(read_len)

        return ret

    def read_address(self, length=4):
        """
        Reads an IP address

        :param length: the RDLENGTH to read
        :return: an IP address split into a tuple
        """
        return tuple(self.read_int(1) for _ in range(length))

    def __getitem__(self, item):
        """
        Allows for indicing and slicing of the raw bytes.

        :param item: an integer or slice.
        :return:
        """
        return self.raw.__getitem__(item)


with open(sys.argv[1], 'rb') as f:
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
    'QD': view.read_int(2),
    'AN': view.read_int(2),
    'NS': view.read_int(2),
    'AR': view.read_int(2)
}

# questions
qnames = [view.read_name(check_pointer=False) for _ in range(counts['QD'])]
qtype = view.read_int(2)
qclass = view.read_int(2)

# answers
answers = []
for _ in range(counts['AN']):
    answer = {
        'name': view.read_name(),
        'qtype': view.read_int(2),
        'qclass': view.read_int(2),
        'ttl': view.read_int(4),
        'rdlength': view.read_int(2),
        'rdata': view.read_address()
    }

    answers.append(answer)

pprint(locals())
