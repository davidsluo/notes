import argparse
import time

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

response, query_time = client.query(args.HOST, args.type)

print(f'; <<>> {parser.prog} David Luo 811357331 <<>> @{args.DNSIP} {args.HOST}')
print(f'; (1 server found)')  # TODO: how is this determined?
print(';; Got answer:')
status = 'NOERROR' if response.rcode == 0 else 'ERROR'
print(f';; ->>HEADER<<- opcode: {response.opcode}, status: {status}, id:{response.transaction_id}')
flag_str = ' '.join(flag.name for flag in response.flags)
print(f';; flags: {flag_str}; QUERY: {len(response.questions)}, ANSWER: {len(response.answers)}, '
      f'AUTHORITY: {len(response.authorities)}, ADDITIONAL: {len(response.additionals)}')
print()
print(';; QUESTION SECTION:')
for q in response.questions:
    print(f';{q.name:<35}    {q.qclass.name:<4}    {q.qtype.name:<8}')
print()
for name, section in {'ANSWER': response.answers, 'AUTHORITY': response.authorities}.items():
    if len(section) > 0:
        print(f';; {name} SECTION:')
        for rr in section:
            print(str(rr))
        print()

print(f';; Query time: {query_time} msec')
print(f';; SERVER: {args.DNSIP}#{args.port}({args.DNSIP})')
print(f';; WHEN: {time.strftime("%a %b %d %H:%M:%S %Z %Y")}')
print(f';; MSG SIZE  rcvd: {response.size}')
