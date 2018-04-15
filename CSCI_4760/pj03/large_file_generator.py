import random

if __name__ == '__main__':
    # print()
    with open('large_file/large_file.raw', 'wb') as f:
        size = 0
        while size < (1 << 30):
            try:
                data = random.randint(0, 1 << 4096)
                b = data.to_bytes(4096, byteorder='big', signed=False)
                f.write(b)
                size += 4096
            except KeyboardInterrupt:
                break
