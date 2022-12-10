#ifndef POOL_H
#define POOL_H

#include <cstddef>
#include <new>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>

using std::byte;
using std::cout;
using std::endl;
using std::size_t;

template <typename T>
class Pool
{
    union element
    {
        byte object[sizeof(T)];
        element *next;
    };

    const size_t _blockSize;
    const bool _trace;

    element *_freehead;
    element **_thepool;

    int _num_blocks;
    int live_cells;

    void expand(); // Add more blocks

    void link(element *);

public:
    Pool(size_t blockSize = 5, bool traceFlag = true) : _blockSize(blockSize), _trace(traceFlag),
                                                        _freehead(nullptr),
                                                        _thepool(nullptr),
                                                        _num_blocks(0),
                                                        live_cells(0)
    {
        if (traceFlag)
            std::cout << "Initializing a pool with element size " << sizeof(element) << " and block size " << blockSize << '\n'
                      << std::endl;
    }
    ~Pool();
    void *allocate();        // Get a pointer inside a pre-allocated block for a new object
    void deallocate(void *); // Free an object's slot (push the address on the "free list")
    void profile();
};

template <typename T>
void Pool<T>::expand()
{
    if (_trace)
        std::cout << "Expanding pool..." << std::endl;

    // Increment pool size and count
    element **newPool{new element *[++_num_blocks]};

    if (_thepool)
    {
        std::copy(_thepool, _thepool + _num_blocks, newPool);
        delete[] _thepool;
    }

    _thepool = newPool;

    // Allocate new block
    void *block = ::operator new(sizeof(element) * _blockSize);

    _thepool[_num_blocks - 1] = static_cast<element *>(block);

    if (_trace)
        std::cout << "Linking cells starting at " << &_thepool[_num_blocks - 1] << std::endl;

    // Link newly allocated elements
    for (int i{static_cast<int>(_blockSize) - 1}; i >= 0; --i)
        link(_thepool[_num_blocks - 1] + i);

    if (_trace)
        std::cout << std::endl;

    return;
}

template <typename T>
void Pool<T>::link(element *elem)
{
    element *temp = _freehead;
    _freehead = elem;
    _freehead->next = temp;

    return;
}

template <typename T>
void *Pool<T>::allocate()
{
    if (_freehead == nullptr)
        expand();

    element *temp = _freehead;
    _freehead = _freehead->next;

    live_cells++;

    if (_trace)
        std::cout << "Cell allocated at " << temp << std::endl;

    return static_cast<void *>(temp);
}

template <typename T>
void Pool<T>::deallocate(void *ptr)
{
    link(static_cast<element *>(ptr));

    live_cells--;

    if (_trace)
        std::cout << "Cell deallocated at " << ptr << std::endl;

    return;
}

template <typename T>
Pool<T>::~Pool()
{
    for (int i{0}; i < _num_blocks; ++i)
        delete _thepool[i];

    delete[] _thepool;
    return;
}

template <typename T>
void Pool<T>::profile()
{
    std::cout << "Live Cells: " << live_cells << " Free Cells: " << _num_blocks * _blockSize - live_cells << std::endl;
    std::cout << "Free List:" << std::endl;

    element *iter = _freehead;

    while (iter)
    {
        std::cout << &iter << std::endl;

        iter = iter->next;
    }

    std::cout << std::endl;
}
#endif
