import random
from os import PathLike
from typing import Union


def generate(file: Union[str, bytes, int, PathLike], size: int):
    with open(file, 'wb') as f:
        curr_size = 0
        while curr_size < size:
            try:
                data = random.randint(0, 1 << 4096)
                b = data.to_bytes(4096, byteorder='big', signed=False)
                curr_size += f.write(b)
            except KeyboardInterrupt:
                break


if __name__ == '__main__':
    # print()
    generate('large_file/large_file.raw', 10 << 30)
