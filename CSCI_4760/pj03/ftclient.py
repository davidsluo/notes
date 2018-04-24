import argparse
import logging

from file_transfer.receiver import ReceiverClient
from file_transfer.sender import SenderClient
from file_transfer.utils import Address, SCRIPT_LOG_LEVEL

logging.basicConfig(level=logging.DEBUG)
log = logging.getLogger('ftclient')


def address_type(arg: str):
    try:
        host, port = arg.split(':', maxsplit=1)
        port = int(port)
        return Address(host, port)
    except:
        raise argparse.ArgumentError('Improperly formatted address. Addresses must be in the format <host>:<port>')


def log_level_type(arg: str):
    levels = dict(
        CRITICAL=logging.CRITICAL,
        FATAL=logging.FATAL,
        ERROR=logging.ERROR,
        WARNING=logging.WARNING,
        WARN=logging.WARN,
        INFO=logging.INFO,
        DEBUG=logging.DEBUG,
        NOTSET=logging.NOTSET,
        SCRIPT=SCRIPT_LOG_LEVEL
    )
    try:
        try:
            level = levels[arg.upper()]
        except KeyError:
            level = int(arg)
        return level
    except:
        argparse.ArgumentParser(f'Log level an int or must be one of {", ".join(levels.keys())}, SCRIPT.')


if __name__ == '__main__':
    parse = argparse.ArgumentParser()
    parse.add_argument('--server', metavar='HOST:PORT', type=address_type, required=True,
                       help='specifies the host and port of the tracker server.')
    group = parse.add_mutually_exclusive_group(required=True)
    group.add_argument('--receive', nargs='?', const=-1, default=None, metavar='COUNT', type=int,
                       help='indicates that the client is in "receive" mode. '
                            'May also specify how many files to receive. -1 for unlimited. Only supported on Linux '
                            '(default -1).')
    group.add_argument('--send', metavar='ID filename', nargs=2,
                       help='indicates that the client is in "send" mode.')
    parse.add_argument('-c', '--cons', metavar='CNUM', type=int, default=1,
                       help='specifies the number of parallel, concurrent connections to use when sending (default 1).')
    parse.add_argument('-s', '--size', metavar='SIZE', type=int, default=4096,
                       help="specifies the size of the buffer size when receiving (default 4096).")
    parse.add_argument('-p', '--port', metavar='PORT', default=47685,
                       help='specifies the port that the client will use for receiving (default to 47685).')
    parse.add_argument('--log-level', default=logging.INFO, type=log_level_type,
                       help='specifies the level of logging (default INFO).')
    args = parse.parse_args()

    log.setLevel(args.log_level)

    if args.receive == -1:
        client = ReceiverClient(args.server)
        client.receive(chunk_size=args.size)
    elif args.receive is not None:
        client = ReceiverClient(args.server, count=args.receive)
        client.receive(chunk_size=args.size)
    elif args.send:
        client = SenderClient(args.server)
        client.send(int(args.send[0]), args.send[1], connections=args.cons)
