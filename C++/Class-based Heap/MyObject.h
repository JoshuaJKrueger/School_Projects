//#include <iosfwd>
#include <string>
#include <iostream>
#include "Pool.h"

using std::ostream;
using std::string;

class MyObject
{
    int id;
    string name;
    inline static Pool<MyObject> pool;

    // Disallow copy, assign, and direct construction
    MyObject(const MyObject &);
    MyObject &operator=(const MyObject &);
    MyObject(int i, const string &nm)
        : name(nm)
    {
        id = i;
    }

    static void *operator new(size_t)
    {
        return pool.allocate();
    }

public:
    static void operator delete(void *p)
    {
        pool.deallocate(p);
    }
    static MyObject *create(int id, const string &name) // Factory method
    {
        return new MyObject(id, name);
    }
    // friend ostream& operator<<(ostream&, const MyObject&);
    friend ostream &operator<<(ostream &os, const MyObject &o)
    {
        return os << '{' << o.id << ',' << o.name << '}';
    }

    static void profile()
    {
        pool.profile();
    }
};
