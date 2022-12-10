'''
    Joshua J. Krueger
    Project 2 Sorting
    This program benchmarks several sorting algorithms
'''

from random import seed, sample
from time import perf_counter

def quick_sort(lst):
    '''
        Takes in a list and returners it sorted.
        This implementation is recursive and is not done in-place
    '''

    # Make sure list is long enough to sort
    if len(lst) < 2:
        return lst

    # Set pivot and swapping index
    pivot = lst[0]
    j = 0

    for i in range(len(lst) - 1):
        if lst[i + 1] < pivot:
            # Swap values if get lesser and greater values on opposite sides of pivot
            lst[i + 1], lst[j + 1] = lst[j + 1], lst[i + 1]
            j += 1

    # Set next pivot
    lst[0], lst[j] = lst[j], lst[0]

    sub_list0 = quick_sort(lst[:j])
    sub_list1 = quick_sort(lst[j + 1:])

    sub_list0.append(lst[j])

    return sub_list0 + sub_list1


def merge_sort(lst):
    '''
        Takes in a list and returns it sorted. Done recursively.
    '''

    # Make sure list is long enough to sort
    if len(lst) < 2:
        return lst

    mid = len(lst) // 2
    sub_list0 = lst[mid:]
    sub_list1 = lst[:mid]

    merge_sort(sub_list0)
    merge_sort(sub_list1)

    i = j = k = 0

    # Merge lists
    while i < len(sub_list0) and j < len(sub_list1):
        if sub_list0[i] < sub_list1[j]:
            lst[k] = sub_list0[i]
            i += 1
        else:
            lst[k] = sub_list1[j]
            j += 1

        k += 1

    # Catch any stragglers
    while i < len(sub_list0):
        lst[k] = sub_list0[i]
        i += 1
        k += 1

    while j < len(sub_list1):
        lst[k] = sub_list1[j]
        j += 1
        k += 1

    return lst


def insertion_sort(lst):
    '''
        Takes in a list and returns it sorted.
    '''

    for i in range(len(lst) - 1):
        # Make sure index stays in bounds
        if i + 1 <= len(lst) - 1:

            j = i
            # Store the value being compared
            swap_storage = lst[i + 1]

            # If lesser iterate back wards until not moving values as needed
            while swap_storage < lst[j] and j >= 0:
                lst[j + 1] = lst[j]
                j -= 1

            lst[j + 1] = swap_storage

    return lst


def selection_sort(lst):
    '''
        Takes in a list and returns it sorted.
    '''

    sorted_list = []

    while len(lst) > 0:
        # Store the index and value lowest value in lst
        lowest_iteration = (0, lst[0])

        # Find the lowest value and store it
        for i, value in enumerate(lst):
            if value < lowest_iteration[1]:
                lowest_iteration = i, value

        # Add the lowest value to the sorted list and remove it from lst
        sorted_list.append(lowest_iteration[1])
        del lst[lowest_iteration[0]]

    return sorted_list


def is_sorted(lst):
    '''
        Takes in a list and makes sure it is a list of sorted integers
    '''
    if type(lst) is not list:
        return False

    for i, value in enumerate(lst):
        if type(value) is not int:
            return False

        if i + 1 <= len(lst) - 1:
            if value > lst[i + 1]:
                return False

    return True


def main():
    '''
        Entry to program runs the benchmarks
    '''

    # Initialization
    DATA_SIZE = 10000
    seed(371890)
    DATA = sample(range(DATA_SIZE * 3), k=DATA_SIZE)
    SORTING_METHODS = [('selection_sort', selection_sort),
               ('insertion_sort', insertion_sort),
               ('merge_sort', merge_sort),
               ('quick_sort', quick_sort),
               ('timsort', sorted)]

    # Start Benchmarking
    for method in SORTING_METHODS:
        data_instance = DATA.copy()

        print(F'Starting {method[0]}:')

        time_0 = perf_counter()
        data_instance = method[1](data_instance)
        time_1 = perf_counter()

        print(f'\t{method[0]} took {round(time_1 - time_0, 4)} seconds to complete.')

if __name__ == '__main__':
    main()
