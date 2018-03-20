from enum import IntEnum, IntFlag, Enum, Flag
from typing import List


class OpCode(Enum):
    QUERY = 0
    IQUERY = 1
    STATUS = 2
    UNUSED = tuple(range(3, 16))

    @classmethod
    def from_opcode(cls, opcode):
        if opcode in cls.UNUSED.value:
            return cls.UNUSED
        else:
            return cls.__call__(opcode)


class QClass(IntEnum):
    IN = 1
    CH = 3
    HS = 4
    ANY = 255


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


class Flags(IntFlag):
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

    def __str__(self):
        return f'<ResourceRecord name={self.name} rdata={self.rdata}>'


class DNSMessage:
    def __init__(self,
                 transaction_id: int,
                 opcode: OpCode,
                 flags: List[Flag],
                 z: int,
                 rcode: int,
                 questions: List[Question],
                 answers: List[ResourceRecord],
                 authorities: List[ResourceRecord],
                 additionals: List[ResourceRecord] = None):
        self.transaction_id = transaction_id
        self.opcode = opcode
        self.flags = flags
        self.z = z
        self.rcode = rcode
        self.questions = questions
        self.answers = answers
        self.authorities = authorities
        self.additionals = additionals

    def __repr__(self):
        return f'<DNSResponse id={self.transaction_id}>'

    def __str__(self):
        pass


