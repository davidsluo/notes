import argparse
from pprint import pprint

from dns.client import Client
from models.enums import QType

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
    client = Client(args.DNSIP, protocol='TCP')
else:
    client = Client(args.DNSIP, protocol='UDP')

response = client.query(args.HOST, args.type)

pprint(vars(response))
