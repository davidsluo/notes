from .enums import QClass, QType


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