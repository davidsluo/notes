import sys

import struct
from pprint import pprint

masks = {
    'QR': (0b1, 15),
    'OPCODE': (0b1111, 11),
    'AA': (0b1, 10),
    'TC': (0b1, 9),
    'RD': (0b1, 8),
    'RA': (0b1, 7),
    'Z': (0b111, 4),
    'RCODE': (0b1111, 0),
}

with open(sys.argv[1], 'rb') as f:
    raw = f.read(4)

    id, metadata = struct.unpack('HH', raw)

    print(f'id       = {id:0>19_b}')
    print(f'metadata = {metadata:0>19_b}')

    properties = {key: (metadata & (bits << shift)) >> shift for key, (bits, shift) in masks.items()}
    pprint(properties)
