from contextlib import closing
from html import escape
from urllib.parse import parse_qs
from wsgiref.simple_server import make_server

import mysql.connector
from jinja2 import Environment, PackageLoader, select_autoescape

jinja_env: Environment = Environment(
    loader=PackageLoader('pj03', 'templates'),
    autoescape=select_autoescape(['html', 'xml'])
)

mysql_connection_info = {
    'host':        'localhost',
    'user':        'pj03',
    'passwd':      'Patata123!',
    'database':    'pj03',
    'auth_plugin': 'mysql_native_password'
}


def application(env, start_response):
    template = jinja_env.get_template('index.html')

    template_args = {}
    if env['PATH_INFO'] != '/pj03/':
        start_response('404 Not Found', [('Content-Type', 'text/html')])
        return '<h1>404 Not Found</h1>'

    start_response('200 OK', [('Content-Type', 'text/html')])

    try:
        params = parse_qs(env['QUERY_STRING'])
    except:
        params = {}

    try:
        page = int(escape(params['page'][0]))
    except (KeyError, ValueError):
        page = 1

    try:
        per_page = int(escape(params['per_page'][0]))
    except (KeyError, ValueError):
        per_page = 50

    try:
        col = escape(params['col'][0])
    except KeyError:
        col = ''

    columns = (
            'id', 'Station', 'StationName', 'Elevation', 'Latitude', 'Longitude', 'Date', 'SkyConditions', 'Visibility',
            'PresentWeatherType', 'DryBulbTempF', 'WetBulbTempF', 'DewPointTempF', 'RelativeHumidity', 'WindSpeed',
            'WindDirection', 'WindGustSpeed', 'StationPressure', 'PressureTendency', 'PressureChange',
            'SeaLevelPressure', 'Precipitation', 'AltimeterSetting'
    )
    if col not in columns:
        col = ''

    try:
        order = escape(params['order'][0]).upper()
    except KeyError:
        order = 'ASC'

    if col:
        query_string = f'SELECT * FROM weather ORDER BY {col} {"ASC" if order == "ASC" else "DESC"} LIMIT %s, %s;'
    else:
        query_string = 'SELECT * FROM weather LIMIT %s, %s;'

    params = ((page - 1) * per_page, per_page)

    template_args.update(
        {
            'page':     page,
            'per_page': per_page,
            'col':      col,
            'order':    order,
        }
    )

    with closing(mysql.connector.connect(**mysql_connection_info)) as conn:
        with closing(conn.cursor()) as cursor:
            cursor.execute(query_string, params)
            rows = cursor.fetchall()
            columns = tuple(column[0] for column in cursor.description)
            template_args['header'] = columns
            template_args['data'] = rows
            template_args['query_string'] = cursor._executed.decode()

            cursor.execute('SELECT COUNT(*) FROM weather;')
            count = cursor.fetchone()
            page_count = count[0] // per_page + 1
            template_args['pages'] = pagination(page, page_count)
            template_args['last_page'] = page_count

    html = template.render(**template_args)
    return [html.encode(encoding='utf-8')]


def pagination(current, last):
    # https://gist.github.com/kottenator/9d936eb3e4e3c3e02598
    delta = 3
    left = current - delta
    right = current + delta + 1
    range_ = []
    range_dots = []
    l = None

    for i in range(1, last + 1):
        if i == 1 or i == last or left <= i < right:
            range_.append(i)

    for i in range_:
        if l:
            if i - l == 2:
                range_dots.append(l + 1)
            elif i - l != 1:
                range_dots.append('...')
        range_dots.append(i)
        l = i

    return range_dots


if __name__ == '__main__':
    httpd = make_server('localhost', 4303, application)
    httpd.serve_forever()
