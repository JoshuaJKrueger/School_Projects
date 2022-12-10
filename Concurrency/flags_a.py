'''
    Downloads all the countries' flags sequentially
'''
from download import download
from time import perf_counter

def main():
    ''' Reads country names in from file then downloads each flag logging the time and download size'''
    countries = []

    with open('flags.txt', 'r') as f:
        countries = f.read().strip().split('\n')
    
    tS = perf_counter()
    totalBytes = 0

    for country in countries:
        totalBytes += download(country)
    tE = perf_counter()
    print(f'Elapsed Time: {tE-tS}\nBytes Downloaded: {totalBytes}\n')

if __name__ == '__main__':
    main()
