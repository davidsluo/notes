#!/usr/bin/env python3

import sys
import csv

fields = 'Date,Time,TargetTemp,ActualTemp,System,SystemAge,BuildingID'.split(',')
reader = csv.DictReader(sys.stdin, fieldnames=fields)
# input comes from STDIN (standard input)
for row in reader:
    try:
        print('%s\t%s' % (row['System'], abs(int(row['TargetTemp']) - int(row['ActualTemp']))))
    except:
        pass
