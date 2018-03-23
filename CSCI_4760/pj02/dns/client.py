import socket
import time
from typing import Callable

from dns.constructor import DNSConstructor
from dns.parser import DNSParser
from models.enums import QType
from models.util import int_to_bytes


class Client:
    def __init__(self, ip, *, port=53, protocol='UDP', timeout=5, retries=3,
                 on_timeout: Callable[[int], None] = lambda x: None):
        self.ip = ip
        self.port = port
        self.protocol = protocol
        self.timeout = timeout
        self.retries = retries

        if protocol == 'UDP':
            self.conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        elif protocol == 'TCP':
            self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.conn.settimeout(timeout)
        self.on_timeout = on_timeout

    def query(self, qname: str, qtype: QType):
        # Construct request
        request = bytes(DNSConstructor.construct(qname, qtype))
        if self.protocol == 'TCP':
            request = int_to_bytes(len(request)) + request

        # query dns server, retrying if necessary, with exponential backoff.
        for try_count in range(self.retries):
            try:
                self.conn.connect((self.ip, self.port))
                self.conn.send(request)
                raw_resp = self.conn.recv(1024)
                self.conn.close()
                break
            except TimeoutError:
                self.on_timeout(try_count + 1)
                time.sleep(2 ** try_count)
        else:
            # max retry count exceeded.
            return None

        # parse response
        if self.protocol == 'TCP':
            raw_resp = raw_resp[2:]
        response = DNSParser.parse(raw_resp)

        return response
