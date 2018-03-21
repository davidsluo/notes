import argparse
import socket

# from .constructor import DNSConstructor
# from .models import QType
# from .parser import DNSParser
from pprint import pprint

from CSCI_4760.pj02.constructor import DNSConstructor
from CSCI_4760.pj02.models.enums import QType
from CSCI_4760.pj02.parser import DNSParser

parser = argparse.ArgumentParser()


def qtype(arg):
    try:
        return getattr(QType, arg)
    except:
        raise argparse.ArgumentTypeError()


parser.add_argument('-t', metavar='TYPE', type=qtype, required=True, dest='type')
parser.add_argument('--tcp', action='store_true')
parser.add_argument('DNSIP')
parser.add_argument('HOST')

args = parser.parse_args()

if args.tcp:
    conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
else:
    conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

conn.connect((args.DNSIP, 53))

request = DNSConstructor.construct(args.HOST, args.type)

conn.send(bytes(request))
raw_resp = conn.recv(1024)

response = DNSParser.parse(raw_resp)

conn.close()

pprint(vars(response))
