import argparse
import random


def generate(file, size: int):
    with file as f:
        curr_size = 0
        while curr_size < size:
            try:
                data = random.randint(0, 1 << 4096)
                b = data.to_bytes(4096, byteorder='big', signed=False)
                curr_size += f.write(b)
            except KeyboardInterrupt:
                break


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('where', type=argparse.FileType('wb'))
    parser.add_argument('size', type=eval)
    args = parser.parse_args()
    generate(args.where, args.size)
