import socket

from dns.constructor import DNSConstructor
from dns.parser import DNSParser
from models.enums import QType


class Client:
    def __init__(self, ip, *, port=53, protocol='UDP'):
        self.ip = ip
        self.port = port
        if protocol == 'UDP':
            self.conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        elif protocol == 'TCP':
            self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def query(self, qname: str, qtype: QType):
        self.conn.connect((self.ip, self.port))

        request = DNSConstructor.construct(qname, qtype)
        self.conn.send(bytes(request))

        raw_resp = self.conn.recv(1024)
        response = DNSParser.parse(raw_resp)

        self.conn.close()

        return response
