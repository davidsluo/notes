import argparse
import mimetypes
import os
import socket
import threading
from pathlib import Path
from urllib.parse import urlparse

parser = argparse.ArgumentParser(description='A basic HTTP server.')


def directory_type(arg):
    """Validates root directory argument."""
    if os.path.isdir(arg):
        return arg
    else:
        raise argparse.ArgumentError(arg, 'Could not resolve root path.')


default_path = Path('./www')
parser.add_argument('--root', '-R', metavar='PATH', type=directory_type, default=default_path.absolute())
parser.add_argument('--host', '-H', metavar='ADDRESS', default='0.0.0.0')
parser.add_argument('--port', '-P', metavar='PORT', type=int, default='47684')
args = parser.parse_args()

SERVER_ROOT = args.root if isinstance(args.root, Path) else Path(args.root)
SERVER_HOST = args.host
SERVER_PORT = args.port
SERVER_ADDR = (SERVER_HOST, SERVER_PORT)

connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connection.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
connection.bind(SERVER_ADDR)
connection.listen(10)


class Request:
    """Represents an HTTP request."""

    def __init__(self):
        self.method: str = None
        self.url: str = None
        self.version: str = None
        self.body: str = None
        self.bytes = None

    def __str__(self):
        return str(self.__dict__)

    @classmethod
    def decode(cls, raw: bytes):
        """Turns a raw request into an instance of this class."""

        request = cls()

        setattr(request, 'bytes', raw)

        raw = raw.decode()
        print(raw)
        fields = raw.splitlines()

        body_start = 0  # determines where the body of the request starts

        request_line = fields[0].split()
        del fields[0]
        method = request_line[0]
        url = next((param for param in request_line if param.startswith('/')))
        version = next((param for param in request_line if param.startswith('HTTP')))

        setattr(request, 'method', method)
        setattr(request, 'url', url)
        setattr(request, 'version', version)

        for field in fields:
            if field == '':
                break

            body_start += 1

            key, value = field.split(':', 1)
            setattr(request, key.lower(), value.strip())

        body = '\n'.join(fields[body_start:])

        setattr(request, 'body', body)

        return request


class Response:
    status_codes = {
        200: 'OK',
        400: 'Bad Request',
        403: 'Forbidden',
        404: 'Not Found',
    }

    default_body = {key: f'<h1>{key} {value}</h1>'.encode() for key, value in status_codes.items()}

    def __init__(self, status: int, version: str = '1.1', headers: dict = None, body: bytes = None):
        self.status = status
        self.version = version
        self.headers = headers or {}
        self.body = body or self.default_body[status]

    def encode(self):
        return bytes(self)

    def __bytes__(self):
        status_code = self.status_codes[self.status]
        status = f'HTTP/{self.version} {self.status} {status_code}\r\n'
        headers = ''.join(f'{key}: {value}\r\n' for key, value in self.headers.items())

        return (status + headers + '\r\n').encode() + self.body


class ServerThread(threading.Thread):
    """Runs an HTTP connection in a new thread."""

    suffixes = ['.txt', '.html', '.png']

    def __init__(self, client, address):
        """Constructor"""
        threading.Thread.__init__(self)
        self.client: socket.socket = client
        self.address = address

    def run(self):
        """Serve client's request."""
        request = self.client.recv(1024)

        # TODO: why does this happen?
        if len(request) == 0:
            return

        try:
            request = Request.decode(request)
        except:
            response = Response(status=400)
        else:
            response = self.respond(request)

        self.client.send(response.encode())
        self.client.shutdown(socket.SHUT_RDWR)
        self.client.close()

    def respond(self, request: Request):
        parser = urlparse(request.url)

        # if '..' in parser.path or '.' in parser.path:
        #     return self.error_codes[403]

        path = SERVER_ROOT / parser.path.strip('/')

        if path.is_file():
            # if path.suffix in self.suffixes:
            if True:
                # serve file
                with open(path, 'rb') as f:
                    body = f.read()

                content_type = mimetypes.types_map.get(path.suffix, None)  # resolve content_type
                return Response(status=200, body=body, headers={'Content-Type': content_type} if content_type else None)
            else:
                # do not serve. invalid suffix.
                return Response(status=403)
        elif path.is_dir():
            # serve index.html/index.txt or 404 if no index found.
            file = None
            if (path / 'index.html').is_file():
                file = 'index.html'
            elif (path / 'index.txt').is_file():
                file = 'index.txt'

            if file is not None:
                with open(path / file, 'rb') as f:
                    body = f.read()
                content_type = mimetypes.types_map.get(path.suffix, None)  # resolve content_type
                return Response(status=200, body=body, headers={'Content-Type': content_type} if content_type else None)
            else:
                # 404 not found
                return Response(status=404)
        else:
            # 404 not found
            return Response(status=404)


if __name__ == '__main__':
    try:
        while True:
            # accept connection and serve in new thread.
            client, address = connection.accept()
            thread = ServerThread(client, address)
            thread.start()
    # exit gracefully
    except KeyboardInterrupt:
        print('Waiting for connections to close...')
        for thread in threading.enumerate():
            if thread != threading.current_thread():
                print(f'Closing {thread.address}')
                thread.join()
        print('Exiting...')
