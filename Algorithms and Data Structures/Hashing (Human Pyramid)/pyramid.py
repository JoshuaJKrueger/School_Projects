'''
    Joshua Krueger
    CS-1410
    Project 2

    This program recursively calculates the weight distribution in a pyramid, and stores
    results in cache.p for faster results.

    Argv[1] is expected to be an unsigned integer, which represents the number of rows
    in the pyramid to calculate.
'''
from sys import argv
from time import perf_counter
import pickle
import os

__location__ = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))
cachePath = os.path.join(__location__, 'cache.p')

def weight_on(r, c):
    ''' Recursive function that returns the weight in a pyramid a
        given position. Searches cache before calculating'''
    weight_on.calls += 1

    if (r, c) in cache:
        weight_on.cacheCalls += 1
        return cache[(r, c)]
    elif r < 0:
        return 0
    elif c < 0 or c > r:
        return 0
    else:
        val = 200 + (weight_on(r - 1, c - 1) + weight_on(r - 1, c)) / 2
        cache[(r, c)] = val

        return val


def main():
    if not len(argv) == 2:
        print('Please enter the number of rows to calculate as a command line argument.')

        return -1
    
    try:
        argv[1] = int(argv[1])
    except ValueError:
        print('Please enter an integer greater than 0')
    
    if argv[1] <= 0:
        print('Please enter an integer greater than 0')
    
    weight_on.calls = 0
    weight_on.cacheCalls = 0
    start = perf_counter()

    for y in range(int(argv[1])):
        line = ''
        for x in range(y + 1):
            line += f'{(weight_on(y, x) - 200):.2f} '
        
        line = line[:-1]
        
        print(line)
    
    end = perf_counter()

    print(f'\nElapsed time: {end-start} seconds')
    print(f'Number of function calls: {weight_on.calls}')
    print(f'Number of cache hits: {weight_on.cacheCalls}')

    pickle.dump(cache, open(cachePath, 'wb'))
 
try:
    cache = pickle.load(open(cachePath, 'rb'))
except EOFError:
    cache = {}
except IOError:
    open(cachePath, 'w+').close()
    cache = {}


if __name__ == '__main__':
    main()
