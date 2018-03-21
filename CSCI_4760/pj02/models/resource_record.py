from .enums import QType, QClass
from .util import int_to_bytes


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