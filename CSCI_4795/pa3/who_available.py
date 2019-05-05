import sys
import dateparser
import pymysql.cursors
import datetime

when = dateparser.parse(' '.join(sys.argv[1:]))

open = when.replace(hour=9, minute=0, second=0)
close = when.replace(hour=17, minute=0, second=0)

if when.weekday() in (5, 6) or not (open <= when <= close):
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
       sql1 = 'SELECT NurseID FROM nurse_schedule WHERE SlotDate=%s AND SlotStart<=%s AND SlotEnd>=%s'
       cursor.execute(sql1, (when.date(), when.time(), when.time()))
       nurse_ids = cursor.fetchall()
#      print(nurse_ids)
       
       if len(nurse_ids) == 0:
           print('No nurses are available at %s.' % when)
       else:
           sql2 = 'SELECT LastName FROM nurses WHERE id=%s'
           nurses = []
           for id in nurse_ids:
               cursor.execute(sql2, (id,))
               nurse = cursor.fetchone()
               nurses.append(nurse[0])
           
           if len(nurses) == 1:
               print('Nurse %s is available at that time.' % nurses[0])
           else:
               nurses = 'Nurse ' + 'Nurse '.join(nurses[:-1]) + ' and Nurse %s' % nurse[-1]
               print('%s are available at that time.' % nurses)
finally:
    connection.close()
