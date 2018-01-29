import argparse
import os
import socket
import threading

parser = argparse.ArgumentParser(description='A basic HTTP server.')


def directory_type(arg):
    """Validates root directory argument."""
    if os.path.isdir(arg):
        return arg
    else:
        raise argparse.ArgumentError(arg, 'Could not resolve root path.')


parser.add_argument('--root', '-R', metavar='PATH', type=directory_type, default=os.getcwd() + '\\www')
parser.add_argument('--host', '-H', metavar='ADDRESS', default='0.0.0.0')
parser.add_argument('--port', '-P', metavar='PORT', type=int, default='47684')
args = parser.parse_args()

SERVER_ROOT = args.root
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
    def from_bytes(cls, raw: bytes):
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


class ServerThread(threading.Thread):
    """Runs an HTTP connection in a new thread."""

    suffixes = ['.txt', '.html', '.png']
    # suffixes = ['.txt', '.html', '.png', '.css', '.js', '.jpg', '.svg']
    error_codes = {
        200: b'HTTP/1.1 200 OK\r\n\r\n<h1>200 OK</h1>',
        400: b'HTTP/1.1 400 Bad Request\r\n\r\n<h1>400 Bad Request</h1>',
        403: b'HTTP/1.1 403 Forbidden\r\n\r\n<h1>403 Forbidden</h1>',
        404: b'HTTP/1.1 404 Not Found\r\n\r\n<h1>404 Not Found</h1>',
    }

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
            request = Request.from_bytes(request)
        except:
            response = self.error_codes[400]
        else:
            response = self.respond(request)

        self.client.send(response)
        self.client.shutdown(socket.SHUT_RDWR)
        self.client.close()

    def respond(self, request: Request):
        response = b''

        path = f'{SERVER_ROOT}{request.url}'

        if os.path.isfile(path):
            if any(path.endswith(suffix) for suffix in self.suffixes):
                # serve file
                response += b'HTTP/1.1 200 OK\r\n\r\n'
                with open(path, 'rb') as f:
                    response += f.read()
            else:
                # do not serve. invalid suffix.
                response = self.error_codes[403]
        elif os.path.isdir(path):
            # serve index.html/index.txt or 404 if no index found.
            file = None
            if os.path.isfile(path + '/index.html'):
                file = '/index.html'
            elif os.path.isfile(path + '/index.txt'):
                file = '/index.txt'

            if file is not None:
                response += b'HTTP/1.1 200 OK\r\n\r\n'
                with open(path + file, 'rb') as f:
                    response += f.read()
            else:
                # 404 not found
                response = self.error_codes[404]
        else:
            # 404 not found
            response = self.error_codes[404]

        return response


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
