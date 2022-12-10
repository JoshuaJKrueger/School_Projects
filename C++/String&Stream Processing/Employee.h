#ifndef EMPLOYEE
#define EMPLOYEE

#include <string>
#include <iostream>
#include <fstream>
#include <stddef.h>

class Employee
{
private:
    std::string name, // Required
        address,
        city,
        state,
        country,
        phone;
    double salary;
    int id; // Required

    struct EmployeeRec
    {
        int id;
        char name[31];
        char address[26];
        char city[21];
        char state[21];
        char country[21];
        char phone[21];
        double salary;
    };

    struct Tag
    {
        std::string name;
        bool isClosing;
        std::string value;

        Tag(std::string _name, bool _isClosing)
        {
            name = _name;
            isClosing = _isClosing;
        }
    };

    static Tag *getNextTag(std::istream &);

    // Copied from example code employeedb2.cpp
    static void stobuf(const std::string &s, char *buf, size_t limit);

public:
    Employee(std::string _name,
             int _id,
             std::string _address = "",
             std::string _city = "",
             std::string _state = "",
             std::string _country = "",
             std::string _phone = "",
             double _salary = 0.0);

    void display(std::ostream &) const;             // Write a readable Employee representation to a stream
    void write(std::ostream &) const;               // Write a fixed-length record to current file position
    void store(std::iostream &) const;              // Overwrite (or append) record in (to) file
    void toXML(std::ostream &) const;               // Write XML record for Employee
    static Employee *read(std::istream &);          // Read record from current file position
    static Employee *retrieve(std::istream &, int); // Search file for record by id
    static Employee *fromXML(std::istream &);       // Read the XML record from a stream

    void setSalary(double);
    double getSalary();
};

#endif
