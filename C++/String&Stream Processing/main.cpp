#include "Employee.h"

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <memory>
#include <string>

int main(int argc, char *argv[])
{
    // 1) Obtain the name of an XML file to read from the command line (argv[1]). Print an error message and halt
    // the program if there is no command-line argument provided, or if the file does not exist.
    std::string file_name{""};

    try
    {
        if (argc < 2)
            throw std::out_of_range("File name not given");

        file_name = argv[1];
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    std::ifstream file;

    try
    {
        file.open(file_name);

        if (!file.is_open())
            throw std::invalid_argument("Could not open file");
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    // 2) Read each XML record in the file by repeatedly calling Employee::fromXML, which creates an Employee
    // object on-the-fly, and store it in a vector (I recommend using unique_ptrs in the vector).
    std::vector<std::unique_ptr<Employee>> employees;

    try
    {
        while (!file.eof())
        {
            employees.push_back(std::unique_ptr<Employee>(Employee::fromXML(file)));

            if (!employees.back())
            {
                if (employees.size() > 1)
                {
                    employees.pop_back();
                    break;
                }
                else
                    throw std::runtime_error("Missing start of tag character '<'");
            }
        }

        file.close();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    // 3) Loop through your vector and print to cout the Employee data for each object (using the display member
    // function).
    for (const auto &employee : employees)
        employee->display(std::cout);

    // 4) The next step is to create a new file of fixed-length Employee records. This is explained below. Write the
    // records for each employee to your new file (call it “employee.bin”) in the order they appear in your vector.
    // Open this file as an fstream object with both read and write capability, and in binary format.
    std::fstream binFile;

    try
    {
        binFile.open("employee.bin", std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);

        if (!binFile.is_open())
            throw std::runtime_error("Couldn't create employee.bin");

        for (const auto &employee : employees)
            employee->write(binFile);
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    // 5) Clear your vector in preparation for the next step.
    employees.clear();

    // 6) Traverse the file by repeated calls to Employee::read, filling your newly emptied vector with Employee
    // pointers for each record read.
    binFile.clear();
    binFile.seekg(0, std::ios_base::beg);

    while (!binFile.eof())
        employees.push_back(std::unique_ptr<Employee>(Employee::read(binFile)));

    employees.pop_back();

    // 7) Loop through your vector and print to cout an XML representation of each Employee using
    // Employee::toXML.
    for (const auto &employee : employees)
        employee->toXML(std::cout);

    // 8) Search the file for the Employee with id 12345 using Employee::retrieve.
    int findID{12345};
    std::unique_ptr<Employee> emp;

    try
    {
        emp = std::unique_ptr<Employee>(Employee::retrieve(binFile, findID));

        if (!emp)
            throw std::runtime_error("Couldn't find employee with id: " + std::to_string(findID));
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    // 9) Change the salary for the retrieved object to 150000.00
    double newSalary = 150000.00;
    emp->setSalary(newSalary);

    // 10) Write the modified Employee object back to file using Employee::store
    emp->store(binFile);

    // 11) Retrieve the object again by id (12345) and print its salary to verify that the file now has the updated salary.
    try
    {
        emp = std::unique_ptr<Employee>(Employee::retrieve(binFile, findID));

        if (!emp)
            throw std::runtime_error("Couldn't find employee with id: " + std::to_string(findID));

        std::cout << emp->getSalary() << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    // 12) Create a new Employee object of your own with a new, unique id, along with other information.
    int newID = 97381;
    Employee newEmp("Joshua Krueger", newID, "123 S", "Orem", "Utah", "Mars", "888-888-8888", 999999999.9);

    // 13) Store it in the file using Employee::store.
    newEmp.store(binFile);

    // 14) Retrieve the record with Employee::retrieve and display it to cout
    try
    {
        emp = std::unique_ptr<Employee>(Employee::retrieve(binFile, newID));

        if (!emp)
            throw std::runtime_error("Couldn't find employee with id: " + std::to_string(newID));

        emp->display(std::cout);
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    binFile.close();

    return 0;
}
