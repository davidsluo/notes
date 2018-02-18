import copy
import sys


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

    def read_bytes(self, amount: int = 1):
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

    def read_int(self, byte_length: int = 1, signed=False):
        """
        Reads a length of bytes interpreted as a big-endian integer.

        :param byte_length: how many bytes to read.
        :param signed: signed/unsigned integer.
        :return: the integer value of the bytes read.
        """
        return int.from_bytes(self.read_bytes(byte_length), 'big', signed=signed)

    def read_name(self, index: int = None, *, copy_view=True):
        """
        Reads a domain name. Handles pointers.

        :param index: where to start reading.
        :param copy_view: TODO
        :return: the domain name read.
        """
        if index is None:
            index = self.index
        if copy_view:
            view = copy.copy(self)
        else:
            view = self
        view.index = index

        ret = ''
        read = view.read_int(1)
        while read > 0:
            ret += view.read_bytes(read).decode()
            ret += '.'
            read = view.read_int(1)

        return ret

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
for i in range(counts['QD']):
    if i == 0:
        qname = view.read_name(12, copy_view=False)
    else:
        qname = view.read_name(copy_view=False)
qtype = view.read_int(2)
qclass = view.read_int(2)

# answers
