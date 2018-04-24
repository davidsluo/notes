import argparse
import logging

from file_transfer.server import Server
from file_transfer.utils import Address

logging.basicConfig(level=logging.DEBUG)
log = logging.getLogger('ftserver')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=int, default=47684, help='The port to host the server on.')
    parser.add_argument('--static-id', type=int,
                        help='Force the server to provide any client that connects with the specified ID. '
                             'Generally used only in testing.')
    args = parser.parse_args()

    server = Server(Address('', args.port), args.static_id)
    server.serve()
