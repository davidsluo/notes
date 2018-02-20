from enum import IntEnum, IntFlag


class OpCode(IntEnum):
    QUERY = 0
    IQUERY = 1
    STATUS = 2
    UNUSED = -1

    @classmethod
    def from_opcode(cls, opcode):
        if opcode in range(3, 16):
            return cls.UNUSED
        else:
            return cls(opcode)


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
    ANY = 255


class Flags(IntFlag):
    QR = 1 << 15
    AA = 1 << 10
    TC = 1 << 9
    RD = 1 << 8
    RA = 1 << 7
