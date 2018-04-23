import argparse
import platform
import random
import subprocess


def generate(file, size):
    if platform.system() == 'Linux':
        p = subprocess.Popen(['dd', 'if=/dev/zero', f'of={file}', f'bs={size}', 'count=1'])
    else:
        with open(file, 'wb') as f:
            curr_size = 0
            while curr_size < eval(size):
                try:
                    data = random.randint(0, 1 << 4096)
                    b = data.to_bytes(4096, byteorder='big', signed=False)
                    curr_size += f.write(b)
                except KeyboardInterrupt:
                    break


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('where')
    parser.add_argument('size')
    args = parser.parse_args()
    generate(args.where, args.size)
