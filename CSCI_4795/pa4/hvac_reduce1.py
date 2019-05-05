#!/usr/bin/env python3

import sys
from itertools import groupby
from statistics import mean

stdin_iter = map(lambda x: x.strip().split('\t', 1), sys.stdin)

data = {}
for key, group in groupby(stdin_iter, key=lambda x: x[0]):
    group = list(map(lambda x: int(x[1]), group))
    data[key] = mean(group)

worst = sorted(data.items(), key=lambda x: x[1], reverse=True)

print('building\tavg temperature difference')
for k,v in worst:
    print(k,v, sep='\t')
