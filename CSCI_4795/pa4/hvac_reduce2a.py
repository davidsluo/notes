#!/usr/bin/env python3

import sys
from itertools import groupby
from statistics import mean
from pprint import pprint
import csv

reader = csv.DictReader(sys.stdin, fieldnames=['BuildingID', 'Time', 'ActualTemp'], delimiter='\t')

data = {}
for key, group in groupby(reader, key=lambda x: x['BuildingID']):
    data[key] =  mean(map(lambda x: int(x['ActualTemp']), group))

hottest = sorted(data.items(), key=lambda x: x[1], reverse=True)

for k,v in hottest:
    print(k,v, sep='\t')
