'''
    Joshua Krueger
    CS 2420-002 Project 1
    This program tests the performance of several search methods using multiple targets.
'''

from time import perf_counter
from math import sqrt
from random import seed, sample

def linear_search(lst, target):
    '''Searches for a target in a list linearly. List should be sorted.'''

    for i in range(len(lst)):
        if lst[i] == target:
            return True

    return False


def binary_search(lst, target):
    '''Searches for a target in a list by seeing if the target is greater or less than the middle
        element. It then takes that half and repeats. List should be sorted.'''

    lwr_bound = 0
    upr_bound = len(lst) - 1

    while lwr_bound <= upr_bound:
        mid = (lwr_bound + upr_bound) // 2

        if lst[mid] == target:
            return True
        if lst[mid] < target:
            lwr_bound = mid + 1
        else:
            upr_bound = mid - 1

    return False


def jump_search(lst, target):
    '''Searches for a target in a sorted list by jumping in the list a step length, checking if
        the current index is greater then or less than target. If greater linear search from last_step to current step, if less than jump again.'''

    prev_step = 0
    step = int(sqrt(len(lst)))

    while lst[step] <= target and step < len(lst):
        prev_step = step
        step += int(sqrt(len(lst)))

        if step > len(lst) - 1:
            step = len(lst)
            break

    for j in range(prev_step, step):
        if lst[j] == target:
            return True

    return False


class Benchmark:
    '''A class to contain the seach functions and test them to avoid repeated code'''
    def __init__(self, search_function):
        self.search_function = search_function

    def run_test(self, data):
        '''Runs the function and times the time it takes to find the value.'''

        targets = [data[0], data[len(data) // 2], data[-1], -1]
        times = []

        for target in targets:
            start = perf_counter()
            self.search_function(data, target)
            end = perf_counter()
            times.append(end - start)

        return times


def main():
    '''Entry point in program initializes data then starts the tests'''

    seed(371890)
    data = sample(range(999999999), k=5000000)
    data.sort()

    methods = [Benchmark(linear_search),
               Benchmark(binary_search),
               Benchmark(jump_search)]
    method_names = ['Linear Search', 'Binary Search', 'Jump Search']
    task_names = ['First Element',
                 'Middle Element',
                 'Last Element',
                 'Element not in List']

    for i, method in enumerate(methods):
        times = method.run_test(data)

        print(method_names[i] + ': ')
        for j, task in enumerate(task_names):
            print('\t' + task + ' - ' + str(times[j]) + ' seconds')

if __name__ == '__main__':
    main()
