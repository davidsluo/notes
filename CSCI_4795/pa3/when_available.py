import sys
import pymysql.cursors
import dateparser
from datetime import datetime, time

who = sys.argv[1]
when = ' '.join(sys.argv[2:])
when = dateparser.parse(when)

if when.weekday() in (5, 6):
    print 'Sorry, the clinic is only open from 9am-5pm Mon-Fri.'
    sys.exit(0)

connection = pymysql.connect(
    host='52.70.223.35',
    user='clinicuser',
    password='sparky19',
    db='ClinicDB'
)

try:
    with connection.cursor() as cursor:
        sql1 = 'SELECT (id) FROM nurses WHERE LastName=%s'
        cursor.execute(sql1, who)
        nurse_id = cursor.fetchone()
        if nurse_id is None:
            print("Sorry, but we don't have a Nurse %s in this office. Make sure to look up your nurse by last name!" % who)
        else:
            sql2 = 'SELECT SlotDate, SlotStart, SlotEnd FROM nurse_schedule WHERE NurseID=%s AND SlotDate=%s'
            cursor.execute(sql2, (nurse_id[0], when.date()))
            result = cursor.fetchone()
            if result is not None:
                date, start, end = result

                dt = datetime.combine(date, time(0))

                start = (dt + start).strftime('%-I:%M %p')
                end = (dt + end).strftime('%-I:%M %p')

                date = dt.strftime('%B %-d, %Y')
                print('Nurse %s is available on %s from %s to %s.' % (who, date, start, end)) 
            else:
                print('Nurse %s is not available on %s.' % (who, when.date()))

finally:
    connection.close()
