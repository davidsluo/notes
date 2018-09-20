import string
from html import escape
from pprint import pprint
from urllib.parse import parse_qs
from wsgiref.simple_server import make_server

from jinja2 import Environment, PackageLoader, select_autoescape

jinja_env: Environment = Environment(
    loader=PackageLoader('pj02', 'templates'),
    autoescape=select_autoescape(['html', 'xml'])
)


def application(env, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    template = jinja_env.get_template('index.html')
    args = {}
    if env['PATH_INFO'] == '/':
        params = {}
        if env.get('REQUEST_METHOD', None) == 'GET':
            try:
                params = parse_qs(env['QUERY_STRING'])
            except:
                pass
        elif env.get('REQUEST_METHOD', None) == 'POST':
            try:
                request_body_size = int(env.get('CONTENT_LENGTH', 0))
            except ValueError:
                request_body_size = 0
            try:
                body = env['wsgi.input'].read(request_body_size)
                params = parse_qs(body.decode())
            except:
                pass

        if 'num' in params:
            args: dict = handle_params(params)
    html = template.render(**args)
    return [html.encode(encoding='utf-8')]


def handle_params(params):
    args = {
        'errors': []
    }
    try:
        num = escape(params['num'][0])
    except KeyError:
        args['errors'].append('Input number not specified.')
        return args
    try:
        frombase = escape(params['frombase'][0])
    except KeyError:
        args['errors'].append('Original base not specified.')
    try:
        tobase = escape(params['tobase'][0])
    except KeyError:
        args['errors'].append('Target base not specified.')

    if len(args['errors']) > 0:
        return args

    try:
        frombase = int(frombase)
    except:
        args['errors'].append('Could not parse original base.')
    try:
        tobase = int(tobase)
    except:
        args['errors'].append('Could not parse target base.')

    supported = (2, 8, 10, 16)
    if frombase not in supported or tobase not in supported:
        args['errors'].append('Base not in list of supported bases: {}'.format(', '.join(map(str, supported))))
    else:
        try:
            num = int(num, frombase)
        except:
            args['errors'].append('Could not parse input number {} for base {}.'.format(num, frombase))

    if len(args['errors']) > 0:
        return args

    try:
        output = int2base(num, tobase)
    except:
        args['errors'].append('Could not convert number.')
        return args

    args['output'] = output.upper()
    args['num'] = num
    args['base'] = tobase

    return args


# https://stackoverflow.com/a/2267446
def int2base(x, base):
    digs = string.digits + string.ascii_letters
    if x < 0:
        sign = -1
    elif x == 0:
        return digs[0]
    else:
        sign = 1

    x *= sign
    digits = []

    while x:
        digits.append(digs[int(x % base)])
        x = int(x / base)

    if sign < 0:
        digits.append('-')

    digits.reverse()

    return ''.join(digits)


if __name__ == '__main__':
    httpd = make_server('localhost', 8000, application)
    httpd.serve_forever()
