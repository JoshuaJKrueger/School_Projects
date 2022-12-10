#ifndef CIRCBUF_H
#define CIRCBUF_H

#include <string>
#include <cstddef>

class CircBuf
{
private:
    static const size_t CHUNK{8};
    char *head;
    char *tail;
    char *buff;
    size_t cap;
    size_t sz;

public:
    CircBuf(size_t reserve = 0); // Number of elements you want it to be able to hold to start with.
    ~CircBuf();
    size_t size();
    size_t capacity();

    char retChar(char);
    void resize(size_t);
    size_t chunkAlign(size_t);

    void insert(char);
    void insert(const char *, size_t sz);
    void insert(const std::string &);
    char get();
    std::string get(size_t);
    std::string flush(); // Returns a string with all the characters, AND shrinks the buffer to zero.
    std::string examine();
    void shrink(); // Reduces the unused space in the buffer.
    void grow(size_t sz = CircBuf::CHUNK);
};
#endif
