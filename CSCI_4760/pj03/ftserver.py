import argparse
import logging

from file_transfer.server import Server
from file_transfer.utils import Address

logging.basicConfig(level=logging.DEBUG)
log = logging.getLogger('ftserver')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('port', type=int)
    parser.add_argument('--static-id', type=int)
    args = parser.parse_args()

    server = Server(Address('', args.port), args.static_id)
    server.serve()
