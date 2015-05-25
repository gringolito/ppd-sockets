#!/usr/bin/python
"""
confidence.py

"THE BEER-WARE LICENSE" (Revision 42):
<filipeutzig@gmail.com> wrote this file. As long as you retain this
notice you can do whatever you want with this stuff. If we meet some
day, and you think this stuff is worth it, you can buy me a beer in
return.

Initial version by Filipe Utzig <filipeutzig@gmail.com> on 4/8/15.

The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
"SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in
this document are to be interpreted as described in RFC 2119.

Script that calculate the confidence interval of data
"""

import sys
from re import search
from scipy.stats import bayes_mvs
from os import listdir
from os.path import join


def print_usage():
    print sys.argv[0] + "<input_dir> <output_file>"
    print ""
    print "e.g." + sys.argv[0] + "var/log results.txt"


def _parse_time(line):
    tmp = search('(?P<s>\d+)s:(?P<ms>\d+)ms:(?P<us>\d+)us', line)
    if tmp:
        time = float(tmp.group('s')) + \
               float(tmp.group('ms')) / 1000 + \
               float(tmp.group('us')) / 1000000
    else:
        time = None

    return time


def main(args):
    if len(args) != 3:
        print_usage()
        exit(0)

    confidence = 0.99
    data_dir = args[1]
    output_file = args[2]

    results = open(output_file, 'w')
    for filename in listdir(data_dir):
        if not filename.endswith(".log"):
            continue

        _file = open(join(data_dir, filename), 'r')
        data = []
        for line in _file:
            if line[0] == '#':
                results.write(line)
                continue

            time = _parse_time(line)
            if time:
                data.append(time)

        _file.close()
        mean_cntr, var_cntr, std_cntr = bayes_mvs(data, confidence)
        mean = float(mean_cntr[0])
        interval = float(mean_cntr[1][1] - mean)
        results.write("Mean time: " + str(round(mean, 3)) + "+-" + \
                      str(round(interval, 3)) + "s\nConfidence: " + \
                      str(confidence) + "\n\n")

    results.close()


if __name__ == '__main__':
    main(sys.argv)

