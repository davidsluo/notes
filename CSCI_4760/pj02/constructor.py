import random

from .models import DNSMessage, QType, OpCode, Flag, Question, QClass


class DNSConstructor:
    """Constructs DNS Messages"""

    def __init__(self, qname, qtype):
        self.qname = qname
        self.qtype = qtype

    @classmethod
    def construct(cls, qname: str, qtype: QType):
        if not qname.endswith('.'):
            qname += '.'
        constructor = cls(qname, qtype)
        return constructor._construct()

    def _construct(self) -> DNSMessage:
        transaction_id = random.randint(0, 2 ** 16)
        opcode = OpCode.QUERY
        flags = [Flag.RD]
        z = 0
        rcode = 0
        questions = [Question(self.qname, self.qtype, QClass.IN)]

        return DNSMessage(transaction_id, opcode, flags, z, rcode, questions)
