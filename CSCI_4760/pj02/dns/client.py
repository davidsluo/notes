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
                start = time.time()
                self.conn.connect((self.ip, self.port))
                self.conn.send(request)
                if self.protocol == 'TCP':
                    message_len = int.from_bytes(self.conn.recv(2), 'big')
                    raw_resp = self.conn.recv(message_len)
                else:
                    raw_resp = self.conn.recv(512)
                self.conn.close()
                end = time.time()
                break
            except TimeoutError:
                self.on_timeout(try_count + 1)
                time.sleep(2 ** try_count)
        else:
            # max retry count exceeded.
            return None

        response = DNSParser.parse(raw_resp)

        return response, int(round(end - start) * 1000)
