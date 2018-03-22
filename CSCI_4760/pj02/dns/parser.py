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


class DNSParser:
    """
    Parses DNS messages from byte strings.
    """

    def __init__(self, raw: bytes):
        self.view = BytesView(raw)

    @classmethod
    def parse(cls, raw: bytes):
        parser = cls(raw)
        return parser._parse()

    def _parse(self) -> DNSMessage:
        transaction_id = self.view.read_int(2)
        metadata = self.view.read_int(2)
        opcode = OpCode((metadata & (0b1111 << 11)) >> 11)
        rcode = metadata & 0b1111
        flags = [flag for flag in Flag if flag & metadata != 0]
        z = (metadata & (0b111 << 4)) >> 4
        question_count = self.view.read_int(2)
        answer_count = self.view.read_int(2)
        authority_count = self.view.read_int(2)
        # additional_count = self.view.read_int(2)
        self.view.skip(2)

        questions = []
        for _ in range(question_count):
            name = self.read_name()
            qtype = self.view.read_int(2)
            qclass = self.view.read_int(2)
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
            # additionals=additionals
        )

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

    def read_resource_record(self) -> ResourceRecord:
        name = self.read_name()
        qtype = QType(self.view.read_int(2))
        qclass = QClass(self.view.read_int(2))
        ttl = self.view.read_int(4)
        rdlength = self.view.read_int(2)

        # TODO: other types
        if qtype == QType.A:
            rdata = self.read_address()
        elif qtype == QType.NS:
            rdata = self.read_name()
        elif qtype == QType.CNAME:
            rdata = self.read_name()
        elif qtype == QType.SOA:
            mname = self.read_name()
            rname = self.read_name()
            serial = self.view.read_int(4)
            refresh = self.view.read_int(4)
            retry = self.view.read_int(4)
            expire = self.view.read_int(4)
            minimum = self.view.read_int(4)

            rdata = f'{mname} {rname} {serial} {refresh} {retry} {expire} {minimum}'
        elif qtype == QType.WKS:
            # RFC 1035 s.3.4.2.
            address = self.read_address()
            protocol = self.view.read_bytes(1)
            bit_map = None  # TODO: figure out how this works.
        elif qtype == QType.PTR:
            # RFC 1035 s.3.3.12.
            # ptrdname = self.read_name()
            rdata = self.read_name()
        elif qtype == QType.HINFO:
            # RFC 1035 s.3.3.2.
            pass
        elif qtype == QType.MINFO:
            pass
        elif qtype == QType.MX:
            pass
        elif qtype == QType.TXT:
            pass
        elif qtype == QType.AXFR:
            pass
        elif qtype == QType.MAILB:
            pass
        elif qtype == QType.ANY:
            pass
        else:
            rdata = 'Unsupported record.'

        return ResourceRecord(name, qtype, qclass, ttl, rdlength, rdata)


def parse(raw: bytes):
    parser = DNSParser(raw)
    return parser.parse()
