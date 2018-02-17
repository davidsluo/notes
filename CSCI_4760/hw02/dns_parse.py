import struct
import sys

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
    raw = f.read(12)

    id, metadata, qd, an, ns, ar = struct.unpack('>HHHHHH', raw)

    print(f'id       = {id: >19_b} ({id})')
    print(f'metadata = {metadata: >19_b}')

    properties = {key: (metadata & (bits << shift)) >> shift for key, (bits, shift) in masks.items()}
    print(properties)
    print(f'QD_COUNT: {qd}')
    print(f'AN_COUNT: {an}')
    print(f'NS_COUNT: {ns}')
    print(f'AR_COUNT: {ar}')

    for _ in range(qd):
        read = int.from_bytes(f.read(1), byteorder='big')
        while read > 0:
            name = f.read(read).decode()
            print(name)
            read = int.from_bytes(f.read(1), byteorder='big')
