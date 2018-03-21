from enum import IntEnum, IntFlag
from typing import List

import itertools


def int_to_bytes(integer):
    """
    Helper method to convert
    :param integer:
    :return:
    """
    return (integer).to_bytes(2, 'big')


class OpCode(IntEnum):
    QUERY = 0
    IQUERY = 1
    STATUS = 2


class QClass(IntEnum):
    IN = 1
    CH = 3
    HS = 4
    ANY = 255

    def __bytes__(self):
        return int_to_bytes(self)


class QType(IntEnum):
    A = 1
    NS = 2
    CNAME = 5
    SOA = 6
    WKS = 11
    PTR = 12
    HINFO = 13
    MINFO = 14
    MX = 15
    TXT = 16
    AXFR = 252
    MAILB = 253
    ANY = 255

    def __bytes__(self):
        return int_to_bytes(self)


class Flag(IntFlag):
    QR = 1 << 15
    AA = 1 << 10
    TC = 1 << 9
    RD = 1 << 8
    RA = 1 << 7


class Question:
    def __init__(self, name: str, qtype: QType, qclass: QClass):
        self.name = name
        self.qtype = qtype
        self.qclass = qclass

    def __str__(self):
        return f'<Question name={self.name}>'

    def __bytes__(self):
        ret = bytearray()
        split = self.name.split('.')
        for segment in split:
            encoded = segment.encode()
            ret.append(len(encoded))
            ret.extend(encoded)
        ret.extend(bytes(self.qtype))
        ret.extend(bytes(self.qclass))

        return bytes(ret)


class ResourceRecord:
    def __init__(self,
                 name: str,
                 qtype: QType,
                 qclass: QClass,
                 ttl: int,
                 rdlength: int,
                 rdata: str):
        self.name = name
        self.qtype = qtype
        self.qclass = qclass
        self.ttl = ttl
        self.rdlength = rdlength
        self.rdata = rdata

    def __bytes__(self):
        ret = bytearray()
        split = self.name.split('.')
        for segment in split:
            encoded = segment.encode()
            ret.append(len(encoded))  # len must be < 64
            ret.extend(encoded)
        ret.extend(bytes(self.qtype))
        ret.extend(bytes(self.qclass))
        ret.extend(int_to_bytes(self.ttl))
        # ret.append()  # TODO: R Data, consider making this class abc, and subclassing different qtypes.

        return bytes(ret)

    def __str__(self):
        return f'<ResourceRecord name={self.name} rdata={self.rdata}>'


class DNSMessage:
    def __init__(self,
                 transaction_id: int,
                 opcode: OpCode,
                 flags: List[Flag],
                 z: int,
                 rcode: int,
                 questions: List[Question] = None,
                 answers: List[ResourceRecord] = None,
                 authorities: List[ResourceRecord] = None,
                 additionals: List[ResourceRecord] = None):
        self.transaction_id = transaction_id
        self.opcode = opcode
        self.flags = flags
        self.z = z
        self.rcode = rcode
        self.questions = questions
        self.answers = answers or []
        self.authorities = authorities or []
        self.additionals = additionals or []

    def __repr__(self):
        return f'<DNSResponse id={self.transaction_id}>'

    def __str__(self):
        pass

    def __bytes__(self):
        ret = bytearray()

        # Transaction ID
        # First we split the 16 bit number into two bytes, so bytes() will accept it
        ret.extend(int_to_bytes(self.transaction_id))

        # Flags, Op Code, Z, R Code
        line2 = 0
        line2 |= self.opcode << 11
        for flag in self.flags:
            line2 |= flag
        line2 |= self.z << 4
        line2 |= self.rcode
        print(bin(line2))
        ret.extend(int_to_bytes(line2))

        for section in (self.questions, self.answers, self.authorities, self.additionals):
            ret.extend(int_to_bytes(len(section)))

        for question in self.questions:
            ret.extend(bytes(question))

        for rr in itertools.chain(self.answers, self.authorities, self.additionals):
            ret.extend(bytes(rr))

        return bytes(ret)
