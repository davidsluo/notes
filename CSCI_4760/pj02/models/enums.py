from enum import IntEnum, IntFlag

from .util import int_to_bytes


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
    ANY = 255

    def __bytes__(self):
        return int_to_bytes(self)


class Flag(IntFlag):
    QR = 1 << 15
    AA = 1 << 10
    TC = 1 << 9
    RD = 1 << 8
    RA = 1 << 7