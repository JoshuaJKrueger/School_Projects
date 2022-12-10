#include <cmath>
#include <cctype>

#include "CircBuf.h"

CircBuf::CircBuf(size_t reserve /*0*/)
{
    CircBuf::buff = new char[0];
    CircBuf::head = CircBuf::buff;
    CircBuf::tail = CircBuf::buff;
    CircBuf::cap = 0;
    CircBuf::sz = 0;

    if (reserve)
        CircBuf::grow(CircBuf::chunkAlign(reserve));
}

CircBuf::~CircBuf() { delete[] CircBuf::buff; }

size_t CircBuf::size() { return CircBuf::sz; }

size_t CircBuf::capacity() { return CircBuf::cap; }

/*
    Determines if the memory in the buffer represents a valid character
    and either returns the character or '-'
*/
char CircBuf::retChar(char c) { return isprint(c) ? c : '-'; }

/*
    Creates a new array of the appropriate size
    and copies the old buffer into it
*/
void CircBuf::resize(size_t sz)
{
    sz = CircBuf::chunkAlign(sz);
    char *temp = new char[sz];
    char *iter = temp;

    // Used when the buffer is full capacity
    if (CircBuf::tail == CircBuf::head && CircBuf::sz > 0)
    {
        *iter++ = CircBuf::get();
        CircBuf::sz++;
    }

    // Loop though buffer and copy characters
    while (CircBuf::tail != CircBuf::head)
    {
        *iter++ = CircBuf::get();
        CircBuf::sz++; // CircBuf::get() decrements the sz
    }

    delete[] CircBuf::buff;

    CircBuf::buff = temp;
    CircBuf::head = iter;
    CircBuf::tail = CircBuf::buff;
    CircBuf::cap = sz;

    return;
}

/*
    Used for rounding sizes to the nearest power of CircBuf::CHUNK
*/
size_t CircBuf::chunkAlign(size_t sz) { return static_cast<size_t>(ceil(static_cast<double>(sz) / CircBuf::CHUNK) * CircBuf::CHUNK); }

void CircBuf::insert(char c)
{
    if (CircBuf::size() + 1 > CircBuf::capacity())
        CircBuf::grow();

    *CircBuf::head++ = c;

    CircBuf::sz++;

    // Loops the head to the front of the buffer when needed
    if (static_cast<void *>(CircBuf::head) == static_cast<void *>(CircBuf::buff + CircBuf::capacity()))
        CircBuf::head = CircBuf::buff;

    return;
}

void CircBuf::insert(const char *in, size_t sz)
{
    while (CircBuf::size() + sz > CircBuf::capacity())
        CircBuf::grow();

    for (size_t i{0}; i < sz; ++i)
        CircBuf::insert(*in++);

    return;
}

void CircBuf::insert(const std::string &in)
{
    for (auto &c : in)
        CircBuf::insert(c);

    return;
}

char CircBuf::get()
{
    char ret = *CircBuf::tail;
    *CircBuf::tail++ = '\r';

    CircBuf::sz--;

    // Loops the tail to the front of the buffer when needed
    if (static_cast<void *>(CircBuf::tail) == static_cast<void *>(CircBuf::buff + CircBuf::capacity()))
        CircBuf::tail = CircBuf::buff;

    return CircBuf::retChar(ret);
}

std::string CircBuf::get(size_t sz)
{
    std::string ret{""};
    // Limits the return sz to the number of characters in the buffer
    size_t end = sz > CircBuf::sz ? CircBuf::sz : sz;

    for (size_t i{0}; i < end; ++i)
    {
        ret += CircBuf::get();
    }

    return ret;
}

std::string CircBuf::flush() { return CircBuf::get(CircBuf::capacity()); }

std::string CircBuf::examine()
{
    std::string ret{"["};
    char *iter = CircBuf::buff;

    for (size_t i{0}; i < CircBuf::capacity(); ++i)
    {
        char c = *iter++;

        ret += CircBuf::retChar(c);
    }

    return ret + ']';
}

void CircBuf::shrink()
{
    CircBuf::resize(CircBuf::size());

    return;
}

void CircBuf::grow(size_t sz /*CircBuf::CHUNK*/)
{
    size_t newSize = CircBuf::capacity() + sz;

    CircBuf::resize(newSize);

    return;
}
