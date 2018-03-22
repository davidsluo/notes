from abc import ABC, abstractmethod

from .enums import QClass, QType
from .util import int_to_bytes


class ResourceRecord(ABC):
    def __init__(self,
                 name: str,
                 qtype: QType,
                 qclass: QClass,
                 ttl: int,
                 rdlength: int,
                 rdata: bytes):
        self.name = name
        self.qtype = qtype
        self.qclass = qclass
        self.ttl = ttl
        self.rdlength = rdlength
        self.rdata = rdata

    @abstractmethod
    def __bytes__(self) -> bytes:
        ret = bytearray()
        split = self.name.split('.')
        for segment in split:
            encoded = segment.encode()
            ret.append(len(encoded))  # len must be < 64
            ret.extend(encoded)
        ret.extend(bytes(self.qtype))
        ret.extend(bytes(self.qclass))
        ret.extend(int_to_bytes(self.ttl))
        # ret.append()  # This is done in subclasses

        return ret

    def __str__(self):
        return f'<{self.__class__.__name__} name={self.name} qtype={self.qtype.name} rdata={self.rdata}>'


class A(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.A, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(A, self).__bytes__()


class NS(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.NS, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(NS, self).__bytes__()


class CNAME(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.CNAME, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(CNAME, self).__bytes__()


class SOA(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.SOA, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(SOA, self).__bytes__()


class WKS(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.WKS, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(WKS, self).__bytes__()


class PTR(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.PTR, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(PTR, self).__bytes__()


class HINFO(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.HINFO, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(HINFO, self).__bytes__()


class MINFO(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.MINFO, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(MINFO, self).__bytes__()


class MX(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.MX, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(MX, self).__bytes__()


class TXT(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.TXT, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(TXT, self).__bytes__()


class ANY(ResourceRecord):
    def __init__(self, name: str, qclass: QClass, ttl: int, rdlength: int, rdata: bytes):
        super().__init__(name, QType.ANY, qclass, ttl, rdlength, rdata)

    def __bytes__(self):
        header = super(ANY, self).__bytes__()
