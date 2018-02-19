import argparse


class BytesView:
    """
    Allows reading and interpreting a byte string.
    """

    def __init__(self, raw: bytes):
        """
        Constructor

        :param raw: the raw bytes
        """
        self.raw = raw
        self.index = 0

    def skip(self, amount: int):
        """
        Skip over an amount of bytes.

        :param amount: the amount to skip
        """
        self.index += amount

    def read_bytes(self, amount: int):
        """
        Reads raw bytes from the source.

        :param amount: how many bytes to read.
        :return:
        """
        ret = self.raw[self.index: self.index + amount]
        self.index += amount
        return ret

    def read_int(self, byte_length: int, *, signed=False):
        """
        Reads a length of bytes interpreted as a big-endian integer.

        :param byte_length: how many bytes to read.
        :param signed: signed/unsigned integer.
        :return: the integer value of the bytes read.
        """
        return int.from_bytes(self.read_bytes(byte_length), 'big', signed=signed)

    def read_name(self):
        """
        Reads a domain name. Recursively handles pointers.

        :return: the domain name read.
        """

        ret = ''
        type = self.read_int(1)
        while type > 0:
            if type & 0xC0 != 0:
                # is pointer
                temp_index = ((type & 0x3F) << 4) | self.read_int(1)
                current_index = self.index
                self.index = temp_index
                # recursively resolve pointers
                ret += self.read_name()
                self.index = current_index
                return ret
            else:
                # is label
                read_len = type
                ret += self.read_bytes(read_len).decode()
                ret += '.'
            type = self.read_int(1)

        return ret

    def read_address(self, length=4):
        """
        Reads an IP address

        :param length: the RDLENGTH to read
        :return: an IP address split into a tuple
        """
        return tuple(self.read_int(1) for _ in range(length))


parser = argparse.ArgumentParser(description='Parses DNS messages.')
parser.add_argument('--file', '-f', type=argparse.FileType('rb'))
args = parser.parse_args()

with args.file as f:
    raw = f.read()

view = BytesView(raw)

transaction_id = view.read_int(2)
raw_flags = view.read_int(2)
flags = {
    'qr': (raw_flags & (0b1 << 15)) >> 15,
    'opcode': (raw_flags & (0b1111 << 11)) >> 11,
    'aa': (raw_flags & (0b1 << 10)) >> 10,
    'tc': (raw_flags & (0b1 << 9)) >> 9,
    'rd': (raw_flags & (0b1 << 8)) >> 8,
    'ra': (raw_flags & (0b1 << 7)) >> 7,
    'z': (raw_flags & (0b111 << 4)) >> 4,
    'rcode': (raw_flags & (0b1111 << 0)) >> 0
}
counts = {
    'qd': view.read_int(2),  # question
    'an': view.read_int(2),  # answer
    'ns': view.read_int(2),  # authority
    'ar': view.read_int(2)  # additional
}

# questions
questions = [
    {
        'name': view.read_name(),
        'qtype': view.read_int(2),
        'qclass': view.read_int(2)
    } for _ in range(counts['qd'])
]


def read_response_record(number):
    """
    Reads a resource record
    :param number: how many to read.
    :return: a dictionary of resource record values.
    """
    records = []
    for _ in range(number):
        rr = {
            'name': view.read_name(),
            'qtype': view.read_int(2),
            'qclass': view.read_int(2),
            'ttl': view.read_int(4),
            'rdlength': view.read_int(2),
        }

        # A record
        if rr['qtype'] == 1:
            rr['rdata'] = view.read_address()
        # NS, CNAME respectively
        elif rr['qtype'] in (2, 5):
            rr['rdata'] = view.read_name()
        # Everything else
        else:
            rr['rdata'] = 'Record type unsupported.'
            view.skip(rr['rdlength'])
            continue
        records.append(rr)
    return records


# resource records
answers = read_response_record(counts['an'])
authorities = read_response_record(counts['ns'])
additional = read_response_record(counts['ar'])

# pprint(locals())
# format output
print(f'; <<>> {parser.prog} David Luo 811357331 <<>> {args.file.name}')
print(f'; ({len(questions)} server{"s" if len(questions)>1 else ""} found)')
print(';; Got answer:')
opcodes = ['QUERY', 'IQUERY', 'STATUS', *('UNUSED' for _ in range(3, 16))]
opcode = opcodes[flags['opcode']]
status = 'NOERROR' if flags['rcode'] else 'ERROR'
print(f';; ->>HEADER<<- opcode: {opcode}, status: {status}, id:{transaction_id}')
flag_str = ' '.join(key for key, value in flags.items() if value == 1 and key in ('qr', 'aa', 'tc', 'rd', 'ra'))
print(
    f';; flags: {flag_str}; QUERY: {len(questions)}, ANSWER: {counts["an"]}, AUTHORITY: {counts["ns"]}, ADDITIONAL: {counts["ar"]}')
print()
print(';; QUESTION SECTION:')
classes = {1: 'IN', 2: 'CS', 3: 'CH', 4: 'HS'}
types = {1: 'A', 2: 'NS', 5: 'CNAME', 255: 'ANY'}
for q in questions:
    print(f';{q["name"]:<35}    {classes[q["qclass"]]:<4}    {types[q["qtype"]]:<8}')
print()

for name, section in {'ANSWER': answers, 'AUTHORITY': authorities, 'ADDITIONAL': additional}.items():
    if len(section) > 0:
        print(f';; {name} SECTION:')
        for r in answers:
            data = '.'.join(str(i) for i in r['rdata']) if isinstance(r['rdata'], tuple) else r['rdata']
            print(f'{r["name"]:<24}    {r["ttl"]:<8}    {classes[r["qclass"]]:<4}    {types[r["qtype"]]:<8}    {data:<24}')
        print()
