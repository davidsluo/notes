import pymysql.cursors

connection = pymysql.connect(
    host='52.70.223.35',
    user='clinicuser',
    password='sparky19',
    db='ClinicDB'
)

try:
    with connection.cursor() as cursor:
        sql = 'SELECT FirstName, LastName FROM nurses ORDER BY LastName'
        cursor.execute(sql, ())
        nurses = cursor.fetchall()

        nurses = ['%s %s ' % (first, last) for first, last in nurses]
        print 'There are the following nurses:'
        print '\n'.join(nurses)
finally:
    connection.close()
