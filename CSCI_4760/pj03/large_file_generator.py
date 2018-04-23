import argparse
import platform
import subprocess


def generate(file, size):
    if platform.system() == 'Linux':
        p = subprocess.Popen(['/bin/dd', 'if=/dev/zero', f'of={file}', f'bs={size}', 'count=1'])
    else:
        with open(file, 'wb') as f:
            curr_size = 0
            while curr_size < eval(size):
                try:
                    curr_size += f.write(b'0' * 4096)
                except KeyboardInterrupt:
                    break


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('where')
    parser.add_argument('size')
    args = parser.parse_args()
    generate(args.where, args.size)
