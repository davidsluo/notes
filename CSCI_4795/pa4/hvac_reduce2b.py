#!/usr/bin/env python3

import sys
from itertools import groupby
from statistics import mean
from pprint import pprint
import csv

reader = csv.DictReader(sys.stdin, fieldnames=['BuildingID', 'Time', 'ActualTemp'], delimiter='\t')

data = {}
print('building\thour\taverage temp')
for building, grouped_buildings in groupby(reader, key=lambda x: x['BuildingID']):
    grouped_buildings = sorted(grouped_buildings, key=lambda x: x['Time'])
    for hour, grouped_hour in groupby(grouped_buildings, key=lambda x: x['Time']):
        avg = mean(map(lambda x: int(x['ActualTemp']), grouped_hour))
        print(building, hour, avg, sep='\t')

