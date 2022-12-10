'''
    Project 6
    Author: Joshua J. Krueger
    Course: CS 2420-002

    Description: This program calculates the weights of a human
     pyramid and uses this custom ADT to cache the results
'''


from dataclasses import dataclass


@dataclass
class SearchResult:
    '''
        Used to return information from HashMap.search_key
    '''

    found: bool
    bucket: tuple
    index: int = None
    value: float = None


class HashMap:
    '''
        Custom hashmap ADT to cache pyramid calculations
    '''

    def __init__(self) -> None:
        '''
            Initialization for hashmap
        '''

        self.buckets = []

        self.add_buckets()


    def add_buckets(self, num_of_buckets:int=7) -> list:
        '''
            Adds
        '''

        for _ in range(num_of_buckets):
            self.buckets.append([])


    def search_key(self, key:tuple) -> tuple:
        '''
            Searches for the key value pair and returns a SearchResult
        '''

        bucket = self.buckets[self.hash(key) % self.capacity()]

        found_key = False
        for i, pair in enumerate(bucket):
            if pair[0] == key:
                found_key = True
                break

        return (SearchResult(found_key, bucket, i, pair[1])
                if found_key else SearchResult(found_key, bucket))

    @staticmethod
    def hash(key:tuple) -> str:
        '''
            Hashes input and returns the hashed value
            Uses the fact that python by default uses
             32bit ints. Magic number from https://github.com/h2database/h2database/blob/master/h2/src/test/org/h2/test/store/CalculateHashConstant.java
            The hash can be reversed with 0x119de1f3
            Number percentage of collisions seems to be inverse
             logarithmic to number of inputs
        '''

        shift = lambda x: ((x >> 16) ^ x) * 0x45d9f3b
        prime = 7838549627
        mod = 4294967296 # 2^32

        a = key[0]
        b = key[1]
        c = (a * prime % mod) + (b * prime % mod)

        c = shift(c)
        c = shift(c)
        c = (a >> 16) ^ c

        return c


    def refactor(self) -> list:
        '''
            Resorts items in map after load factor becomes to high
        '''

        old_layout = self.buckets

        self.clear()
        self.add_buckets((2 * self.capacity() - 1) - self.capacity())

        for bucket in old_layout:
            for pair in bucket:
                self.set(pair[0], pair[1])


    def get(self, key:tuple) -> float:
        '''
            Returns the value associated with the key
        '''

        search_result = self.search_key(key)

        if not search_result.found:
            raise KeyError

        return search_result.value


    def set(self, key:tuple, value:float) -> None:
        '''
            Adds the pair to the hashmap and rehashes if load factor >= 80%
        '''

        search_result = self.search_key(key)

        if search_result.found:
            search_result.bucket[search_result.index] = (key, value)
        else:
            search_result.bucket.append((key, value))


        if self.size() / self.capacity() >= 0.8:
            self.refactor()


    def remove(self, key:tuple) -> None:
        '''
            Removes the key value pair if it exists
        '''

        search_result = self.search_key(key)

        if search_result.found:
            search_result.bucket.pop(search_result.index)


    def clear(self) -> None:
        '''
            Empties the hashmap
        '''

        self.__init__()


    def capacity(self) -> int:
        '''
            Returns the number of buckets
        '''

        return len(self.buckets)


    def size(self) -> int:
        '''
            Returns the number of key value pairs
        '''

        return sum(len(bucket) for bucket in self.buckets)


    def keys(self) -> list:
        '''
            Returns a list of the keys
        '''

        return [pair[0] for bucket in self.buckets for pair in bucket]
