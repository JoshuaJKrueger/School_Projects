'''
    Downloads all the countries' flags concurrently with threads
'''
from download import download
from time import perf_counter
from concurrent.futures import ThreadPoolExecutor

def main():
    '''
        Reads country names from file
        Downloads flags w/ download helper func
        Logs download time and size
    '''
    countries = []

    with open('flags.txt', 'r') as f:
        countries = f.read().strip().split('\n')
    
    tS = perf_counter()
    with ThreadPoolExecutor(max_workers=None) as p:
        totalBytes = sum(p.map(download, countries))
    tE = perf_counter()
    print(f'Elapsed Time: {tE-tS}\nBytes Downloaded: {totalBytes}\n')

if __name__ == '__main__':
    main()
