import socket

from dns.constructor import DNSConstructor
from dns.parser import DNSParser
from models.enums import QType
from models.util import int_to_bytes


class Client:
    def __init__(self, ip, *, port=53, protocol='UDP'):
        self.ip = ip
        self.port = port
        self.protocol = protocol
        if protocol == 'UDP':
            self.conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        elif protocol == 'TCP':
            self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def query(self, qname: str, qtype: QType):
        self.conn.connect((self.ip, self.port))

        request = bytes(DNSConstructor.construct(qname, qtype))

        if self.protocol == 'TCP':
            request = int_to_bytes(len(request)) + request

        self.conn.send(request)
        raw_resp = self.conn.recv(1024)

        self.conn.close()

        if self.protocol == 'TCP':
            raw_resp = raw_resp[2:]

        response = DNSParser.parse(raw_resp)

        return response
