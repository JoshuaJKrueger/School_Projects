'''
    Downloads all the countries' flags concurrently with processes
'''
from download import download
from concurrent.futures import ProcessPoolExecutor
from multiprocessing import cpu_count
from time import perf_counter

def main():
    ''' Loads country names from file and uses download helper func to download them concurrently while logging time and size downloaded'''
    countries = []

    with open('flags.txt', 'r') as f:
        countries = f.read().strip().split('\n')
    
    tS = perf_counter()
    with ProcessPoolExecutor(cpu_count()) as p:
        totalBytes = sum(p.map(download, countries))
    tE = perf_counter()
    print(f'Elapsed Time: {tE-tS}\nBytes Downloaded: {totalBytes}\n')

if __name__ == '__main__':
    main()
