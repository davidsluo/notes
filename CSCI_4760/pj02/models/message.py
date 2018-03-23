import itertools
from typing import List

from .enums import Flag, OpCode
from .question import Question
from .resource_record import ResourceRecord
from .util import int_to_bytes


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
                 additionals: List[ResourceRecord] = None,
                 *,
                 raw=None):
        self.transaction_id = transaction_id
        self.opcode = opcode
        self.flags = flags
        self.z = z
        self.rcode = rcode
        self.questions = questions
        self.answers = answers or []
        self.authorities = authorities or []
        self.additionals = additionals or []
        self.raw = raw or bytes(self)
        self.size = len(self.raw)

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
        ret.extend(int_to_bytes(line2))

        for section in (self.questions, self.answers, self.authorities, self.additionals):
            ret.extend(int_to_bytes(len(section)))

        for question in self.questions:
            ret.extend(bytes(question))

        for rr in itertools.chain(self.answers, self.authorities, self.additionals):
            ret.extend(bytes(rr))

        return bytes(ret)
