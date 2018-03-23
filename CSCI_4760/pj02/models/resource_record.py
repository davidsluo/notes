from abc import ABC, abstractmethod
from collections import OrderedDict
from typing import TYPE_CHECKING

from .enums import Class, Type

if TYPE_CHECKING:
    from dns.parser import DNSParser


class DictRdataMixin:
    def __str__(self):
        rdata = ' '.join(str(value) for key, value in self.rdata.items())
        return f'{self.name:<24}    {self.ttl:<8}    {self.class_.name:<4}    {self.type.name:<8}    {rdata}'


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
        try:
            type = Type(parser.read_int(2))
        except ValueError:
            type = None
        class_ = Class(parser.read_int(2))
        ttl = parser.read_int(4)
        rdlength = parser.read_int(2)
        py_class = next(subclass for subclass in cls.__subclasses__() if subclass.__type__ == type)

        rdata = py_class.parse_rdata(rdlength, parser)

        return py_class(name, class_, ttl, rdlength, rdata)

    @staticmethod
    @abstractmethod
    def parse_rdata(rdlength: int, parser: 'DNSParser'):
        pass

    def __repr__(self):
        type = self.type.name if self.type else 'UNKNOWN'
        return f'<{self.__class__.__name__} name={self.name} qtype={type} rdata={self.rdata}>'

    def __str__(self):
        type = self.type.name if self.type else 'Unknown'
        return f'{self.name:<24}    {self.ttl:<8}    {self.class_.name:<4}    {type:<8}    {self.rdata}'


class Unknown(ResourceRecord):
    @staticmethod
    def parse_rdata(rdlength: int, parser: 'DNSParser'):
        parser.skip(rdlength)
        return 'UNKNOWN'


class A(ResourceRecord):
    __type__ = Type.A

    @staticmethod
    def parse_rdata(rdlength, parser: 'DNSParser'):
        return parser.read_address()


class NS(ResourceRecord):
    __type__ = Type.NS

    @staticmethod
    def parse_rdata(rdlength, parser: 'DNSParser'):
        return parser.read_name()


class CNAME(ResourceRecord):
    __type__ = Type.CNAME

    @staticmethod
    def parse_rdata(rdlength, parser: 'DNSParser'):
        return parser.read_name()


class SOA(DictRdataMixin, ResourceRecord):
    __type__ = Type.SOA

    @staticmethod
    def parse_rdata(rdlength, parser: 'DNSParser'):
        return OrderedDict(
            [
                ('mname', parser.read_name()),
                ('rname', parser.read_name()),
                ('serial', parser.read_int(4)),
                ('refresh', parser.read_int(4)),
                ('retry', parser.read_int(4)),
                ('expire', parser.read_int(4)),
                ('minimum', parser.read_int(4))
            ]
        )


class WKS(ResourceRecord):
    __type__ = Type.WKS

    @staticmethod
    def parse_rdata(rdlength, parser: 'DNSParser'):
        address = parser.read_address()
        protocol = parser.read_int(1)

        bitlen = (rdlength - 5) * 8
        bits = int.from_bytes(parser.read_bytes(rdlength - 5), 'big')
        bitmap = [bitlen - i - 1 for i in range(bitlen) if bits & (1 << i) != 0]

        return OrderedDict(
            [
                ('address', address),
                ('protocol', protocol),
                ('bitmap', sorted(bitmap))
            ]
        )

    def __str__(self):
        rdata = f'{self.rdata["address"]} {self.rdata["protocol"]} {" ".join(str(i) for i in self.rdata["bitmap"])}'
        return f'{self.name:<24}    {self.ttl:<8}    {self.class_:<4}    {self.type:<8}    {rdata}'


class PTR(ResourceRecord):
    __type__ = Type.PTR

    @staticmethod
    def parse_rdata(rdlength, parser: 'DNSParser'):
        return parser.read_name()


class HINFO(DictRdataMixin, ResourceRecord):
    __type__ = Type.HINFO

    @staticmethod
    def parse_rdata(rdlength, parser: 'DNSParser'):
        return OrderedDict(
            [
                ('cpu', parser.read_string()),
                ('os', parser.read_string())
            ]
        )


class MINFO(DictRdataMixin, ResourceRecord):
    __type__ = Type.MINFO

    @staticmethod
    def parse_rdata(rdlength, parser: 'DNSParser'):
        return OrderedDict(
            [
                ('rmailbx', parser.read_name()),
                ('emailbx', parser.read_name())
            ]
        )


class MX(DictRdataMixin, ResourceRecord):
    __type__ = Type.MX

    @staticmethod
    def parse_rdata(rdlength, parser: 'DNSParser'):
        return OrderedDict(
            [
                ('preference', parser.read_int(2)),
                ('exchange', parser.read_name())
            ]
        )


class TXT(ResourceRecord):
    __type__ = Type.TXT

    @staticmethod
    def parse_rdata(rdlength, parser: 'DNSParser'):
        return parser.read_string()

    def __str__(self):
        type = self.type.name if self.type else 'Unknown'
        return f'{self.name:<24}    {self.ttl:<8}    {self.class_.name:<4}    {type:<8}    "{self.rdata}"'
