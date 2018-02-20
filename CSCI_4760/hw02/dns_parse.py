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
            if type & 0xC0 == 0xC0:
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
parser.add_argument('file', type=argparse.FileType('rb'))
args = parser.parse_args()

with args.file as f:
    raw = f.read()

view = BytesView(raw)

transaction_id = view.read_int(2)
raw_flags = view.read_int(2)
opcodes = ['QUERY', 'IQUERY', 'STATUS', *('UNUSED' for _ in range(3, 16))]
opcode = opcodes[(raw_flags & (0b1111 << 11)) >> 11]
rcode = ((raw_flags & (0b1111 << 0)) >> 0) == 0
flags = {
    'qr': (raw_flags & (0b1 << 15)) >> 15,
    'aa': (raw_flags & (0b1 << 10)) >> 10,
    'tc': (raw_flags & (0b1 << 9)) >> 9,
    'rd': (raw_flags & (0b1 << 8)) >> 8,
    'ra': (raw_flags & (0b1 << 7)) >> 7,
    # 'z': (raw_flags & (0b111 << 4)) >> 4,
}
counts = {
    'qd': view.read_int(2),  # question
    'an': view.read_int(2),  # answer
    'ns': view.read_int(2),  # authority
    'ar': view.read_int(2)  # additional
}

classes = {1: 'IN', 2: 'CS', 3: 'CH', 4: 'HS'}
types = {1: 'A', 2: 'NS', 5: 'CNAME', 6: 'SOA', 255: 'ANY'}

# questions
questions = [
    {
        'name': view.read_name(),
        'qtype': types.get(view.read_int(2), 'UNKNOWN'),
        'qclass': classes.get(view.read_int(2), 'UNKNOWN')
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
            'qtype': types.get(view.read_int(2), 'UNKNOWN'),
            'qclass': classes.get(view.read_int(2), 'UNKNOWN'),
            'ttl': view.read_int(4),
            'rdlength': view.read_int(2),
        }

        if rr['qtype'] == 'A':
            rr['rdata'] = view.read_address()
        elif rr['qtype'] in ('NS', 'CNAME'):
            rr['rdata'] = view.read_name()
        elif rr['qtype'] == 'SOA':
            mname = view.read_name()
            rname = view.read_name()
            serial = view.read_int(4)
            refresh = view.read_int(4)
            retry = view.read_int(4)
            expire = view.read_int(4)
            minimum = view.read_int(4)
            rr['rdata'] = f'{mname} {rname} {serial} {refresh} {retry} {expire} {minimum}'
        # Everything else
        else:
            rr['rdata'] = 'Record type unsupported.'
            view.skip(rr['rdlength'])
            # continue
        records.append(rr)
    return records


# resource records
answers = read_response_record(counts['an'])
authorities = read_response_record(counts['ns'])
# additional = read_response_record(counts['ar'])

# format output
print(f'; <<>> {parser.prog} David Luo 811357331 <<>> {args.file.name}')
print(f'; ({len(questions)} server{"s" if len(questions)>1 else ""} found)')
print(';; Got answer:')
status = 'NOERROR' if rcode else 'ERROR'
print(f';; ->>HEADER<<- opcode: {opcode}, status: {status}, id:{transaction_id}')
flag_str = ' '.join(key for key, value in flags.items() if value == 1 and key in ('qr', 'aa', 'tc', 'rd', 'ra'))
print(f';; flags: {flag_str}; QUERY: {len(questions)}, ANSWER: {counts["an"]}, AUTHORITY: {counts["ns"]}, ADDITIONAL: {counts["ar"]}')
print()
print(';; QUESTION SECTION:')
for q in questions:
    print(f';{q["name"]:<35}    {q["qclass"]:<4}    {q["qtype"]:<8}')
print()

# for name, section in {'ANSWER': answers, 'AUTHORITY': authorities, 'ADDITIONAL': additional}.items():
for name, section in {'ANSWER': answers, 'AUTHORITY': authorities}.items():
    if len(section) > 0:
        print(f';; {name} SECTION:')
        for r in section:
            data = '.'.join(str(i) for i in r['rdata']) if isinstance(r['rdata'], tuple) else r['rdata']
            print(f'{r["name"]:<24}    {r["ttl"]:<8}    {r["qclass"]:<4}    {r["qtype"]:<8}    {data:<24}')
        print()
