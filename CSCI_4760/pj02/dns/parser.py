from models.enums import Flag, OpCode, QClass, QType
from models.message import DNSMessage
from models.question import Question
from models.resource_record import ResourceRecord

__all__ = ['DNSParser']


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


class DNSParser:
    """
    Parses DNS messages from byte strings.
    """

    def __init__(self, raw: bytes):
        self.raw = raw
        self.index = 0

    @classmethod
    def parse(cls, raw: bytes):
        parser = cls(raw)
        return parser._parse()

    def _parse(self) -> DNSMessage:
        transaction_id = self.read_int(2)
        metadata = self.read_int(2)
        opcode = OpCode((metadata & (0b1111 << 11)) >> 11)
        rcode = metadata & 0b1111
        flags = [flag for flag in Flag if flag & metadata != 0]
        z = (metadata & (0b111 << 4)) >> 4
        question_count = self.read_int(2)
        answer_count = self.read_int(2)
        authority_count = self.read_int(2)
        # additional_count = self.read_int(2)
        self.skip(2)

        questions = []
        for _ in range(question_count):
            name = self.read_name()
            qtype = self.read_int(2)
            qclass = self.read_int(2)
            questions.append(Question(name=name,
                                      qtype=QType(qtype),
                                      qclass=QClass(qclass)
                                      ))
        answers = [self.read_resource_record() for _ in range(answer_count)]
        authorities = [self.read_resource_record() for _ in range(authority_count)]
        # additionals = [self.read_resource_record() for _ in range(additional_count)]

        return DNSMessage(
            transaction_id=transaction_id,
            opcode=opcode,
            flags=flags,
            z=z,
            rcode=rcode,
            questions=questions,
            answers=answers,
            authorities=authorities,
            # additionals=additionals,
            raw=self.raw
        )

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
        :return: the bytes read.
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

    def read_string(self):
        """
        Reads a character string.

        :return: the string.
        """
        read_len = self.read_int(1)
        raw = self.read_bytes(read_len)
        return raw.decode()

    def read_name(self):
        """
        Reads a domain name. Recursively handles pointers.

        :return: the domain name read.
        """

        ret = ''
        type = self.read_int(1)
        while type > 0:
            if type & 0xC0 == 0xC0:
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

    def read_address(self):
        """
        Reads an IP address

        :return: an IP address
        """
        return '.'.join(tuple(str(self.read_int(1)) for _ in range(4)))

    def read_resource_record(self) -> ResourceRecord:
        return ResourceRecord.from_parser(self)
