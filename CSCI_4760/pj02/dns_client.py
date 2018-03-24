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


parser.add_argument('-t', metavar='TYPE', type=qtype, dest='type', default=QType.A, help='DNS QType')
parser.add_argument('--tcp', action='store_true', help='If set, use TCP instead of UDP.')
parser.add_argument('--port', type=int, default=53, help='Port on DNS server.')
parser.add_argument('--timeout', type=int, default=5, help='Timeout on connection operations.')
parser.add_argument('--retries', type=int, default=3, help='Number of times to retry querying DNS server.')
parser.add_argument('DNSIP', help='Address of DNS server.')
parser.add_argument('HOST', help='DNS QName')

args = parser.parse_args()

if args.tcp:
    client = Client(args.DNSIP, port=args.port, protocol='TCP', timeout=args.timeout, retries=args.retries)
else:
    client = Client(args.DNSIP, port=args.port, protocol='UDP', timeout=args.timeout, retries=args.retries)

response, query_time = client.query(args.HOST, args.type)

print(f'; <<>> {parser.prog} David Luo 811357331 <<>> @{args.DNSIP} {args.HOST}')
if response is not None:
    print(f'; (1 server found)')
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
else:
    print(f';; connection timed out')
