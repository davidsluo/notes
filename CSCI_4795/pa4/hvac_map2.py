#!/usr/bin/env python3

import sys
import csv

fields = 'Date,Time,TargetTemp,ActualTemp,System,SystemAge,BuildingID'.split(',')
reader = csv.DictReader(sys.stdin, fieldnames=fields)
# input comes from STDIN (standard input)
for row in reader:
    try:
        hour = row['Time'].split(':')[0]
        if not 9 <= int(hour) <= 17:
            continue
        print('\t'.join(
            (
                row['BuildingID'],
                hour,
                row['ActualTemp'], 
            )
        ))
    except ValueError:
        pass
