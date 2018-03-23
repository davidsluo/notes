import argparse
from pprint import pprint

from dns.client import Client
from models.enums import QType

parser = argparse.ArgumentParser()


def qtype(arg):
    try:
        return getattr(QType, arg)
    except:
        raise argparse.ArgumentTypeError(f'Type must be one of: {", ".join(t.name for t in QType)}')


parser.add_argument('-t', metavar='TYPE', type=qtype, dest='type', default=QType.A)
parser.add_argument('--tcp', action='store_true')
parser.add_argument('--port', type=int, default=53)
parser.add_argument('DNSIP')
parser.add_argument('HOST')

args = parser.parse_args()

if args.tcp:
    client = Client(args.DNSIP, port=args.port, protocol='TCP')
else:
    client = Client(args.DNSIP, port=args.port, protocol='UDP')

response = client.query(args.HOST, args.type)

pprint(vars(response))
