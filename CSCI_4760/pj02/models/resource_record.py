from abc import ABC, abstractmethod
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from dns.parser import DNSParser
from .enums import Class, Type


class ResourceRecord(ABC):
    __type__: Type = None

    def __init__(self,
                 name: str,
                 class_: Class,
                 ttl: int,
                 rdlength: int,
                 rdata):
        self.name = name
        self.type = self.__type__
        self.class_ = class_
        self.ttl = ttl
        self.rdlength = rdlength
        self.rdata = rdata

    # def __bytes__(self) -> bytes:
    #     ret = bytearray()
    #     split = self.name.split('.')
    #     for segment in split:
    #         encoded = segment.encode()
    #         ret.append(len(encoded))  # len must be < 64
    #         ret.extend(encoded)
    #     ret.extend(bytes(self.type))
    #     ret.extend(bytes(self.class_))
    #     ret.extend(int_to_bytes(self.ttl))
    #     ret.extend(self.rdata.encode())
    #
    #     return ret

    @classmethod
    def from_parser(cls, parser: 'DNSParser'):
        name = parser.read_name()
        type = Type(parser.read_int(2))
        class_ = Class(parser.read_int(2))
        ttl = parser.read_int(4)
        rdlength = parser.read_int(2)

        py_class = next(subclass for subclass in cls.__subclasses__() if subclass.__type__ == type)

        rdata = py_class.parse_rdata(rdlength, parser)

        return py_class(name, class_, ttl, rdlength, rdata)

    @classmethod
    @abstractmethod
    def parse_rdata(cls, rdlength: int, parser: 'DNSParser'):
        pass

    def __str__(self):
        return f'<{self.__class__.__name__} name={self.name} qtype={self.type.name} rdata={self.rdata}>'


class A(ResourceRecord):
    __type__ = Type.A

    @classmethod
    def parse_rdata(cls, rdlength, parser: 'DNSParser'):
        return parser.read_address()


class NS(ResourceRecord):
    __type__ = Type.NS

    @classmethod
    def parse_rdata(cls, rdlength, parser: 'DNSParser'):
        return parser.read_name()


class CNAME(ResourceRecord):
    __type__ = Type.CNAME

    @classmethod
    def parse_rdata(cls, rdlength, parser: 'DNSParser'):
        return parser.read_name()


class SOA(ResourceRecord):
    __type__ = Type.SOA

    @classmethod
    def parse_rdata(cls, rdlength, parser: 'DNSParser'):
        return {
            'mname': parser.read_name(),
            'rname': parser.read_name(),
            'serial': parser.read_int(4),
            'refresh': parser.read_int(4),
            'retry': parser.read_int(4),
            'expire': parser.read_int(4),
            'minimum': parser.read_int(4)
        }


class WKS(ResourceRecord):
    __type__ = Type.WKS

    @classmethod
    def parse_rdata(cls, rdlength, parser: 'DNSParser'):
        # TODO: match with dig
        address = parser.read_address()
        protocol = parser.read_int(1)
        bits = int.from_bytes(parser.read_bytes(rdlength - 5), 'big')
        bitmap = []
        for i in range((rdlength - 5) * 8, 0, -1):
            if bits & (1 << i) != 0:
                bitmap.append((rdlength - 5) * 8 - i - 1)

        return {
            'address': address,
            'protocol': protocol,
            'bitmap': bitmap
        }


class PTR(ResourceRecord):
    __type__ = Type.PTR

    @classmethod
    def parse_rdata(cls, rdlength, parser: 'DNSParser'):
        return parser.read_name()


class HINFO(ResourceRecord):
    __type__ = Type.HINFO

    @classmethod
    def parse_rdata(cls, rdlength, parser: 'DNSParser'):
        return {
            'cpu': parser.read_string(),
            'os': parser.read_string()
        }


class MINFO(ResourceRecord):
    __type__ = Type.MINFO

    @classmethod
    def parse_rdata(cls, rdlength, parser: 'DNSParser'):
        return {
            'rmailbx': parser.read_name(),
            'emailbx': parser.read_name()
        }


class MX(ResourceRecord):
    __type__ = Type.MX

    @classmethod
    def parse_rdata(cls, rdlength, parser: 'DNSParser'):
        return {
            'preference': parser.read_int(2),
            'exchange': parser.read_name()
        }


class TXT(ResourceRecord):
    @classmethod
    def parse_rdata(cls, rdlength, parser: 'DNSParser'):
        return parser.read_string()

    __type__ = Type.TXT
