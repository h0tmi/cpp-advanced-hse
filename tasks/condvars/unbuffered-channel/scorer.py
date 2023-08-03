#!/usr/bin/python3
import json
import sys


def get_benchmarks(filename):
    return json.load(open(filename))['benchmarks']


def get_score(results):
    for result in results:
        if result['run_type'] != 'aggregate' or result['aggregate_name'] != 'median':
            continue
        times = {
            'real_time': result['real_time'],
            'cpu_time': result['cpu_time']
        }
        if times['real_time'] > 4000:
            print('Median value', times, 'is too high ¯\_(ツ)_/¯')
            sys.exit(1)
        print('Ok:', times)


if __name__ == '__main__':
    print('Checking benchmark medians results...')
    get_score(get_benchmarks(sys.argv[1]))
    print('Passed benchmark validation 🎉🎉🎉')
