#include "Employee.h"

#include <string>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>
#include <ctype.h>

Employee::Employee(std::string _name,
                   int _id,
                   std::string _address,
                   std::string _city,
                   std::string _state,
                   std::string _country,
                   std::string _phone,
                   double _salary)
{
    Employee::name = _name;
    Employee::address = _address;
    Employee::city = _city;
    Employee::state = _state;
    Employee::country = _country;
    Employee::phone = _phone;
    Employee::salary = _salary;
    Employee::id = _id;
}

void Employee::display(std::ostream &os) const
{
    os << "id: " << id << '\n';
    os << "name: " << name << '\n';
    os << "address: " << address << '\n';
    os << "city: " << city << '\n';
    os << "state: " << state << '\n';
    os << "country: " << country << '\n';
    os << "phone: " << phone << '\n';
    os << "salary: " << salary << '\n'
       << '\n';
    os.flush();

    return;
}

void Employee::write(std::ostream &os) const
{
    EmployeeRec rec;
    rec.id = id;
    stobuf(name, rec.name, sizeof(rec.name) - 1);
    stobuf(address, rec.address, sizeof(rec.address) - 1);
    stobuf(city, rec.city, sizeof(rec.city) - 1);
    stobuf(state, rec.state, sizeof(rec.state) - 1);
    stobuf(country, rec.country, sizeof(rec.country) - 1);
    stobuf(phone, rec.phone, sizeof(rec.phone) - 1);
    rec.salary = salary;

    os.write(reinterpret_cast<const char *>(&rec), sizeof rec);

    return;
}

void Employee::store(std::iostream &ios) const
{
    std::unique_ptr<Employee> emp(retrieve(ios, id));

    if (emp)
    {
        ios.clear();
        ios.seekg(static_cast<int>(ios.tellg()) - static_cast<int>(sizeof(EmployeeRec)));
    }

    write(ios);

    return;
}

void Employee::toXML(std::ostream &os) const
{
    os << "<Employee>" << '\n';
    os << '\t' << "<Name>" << name << "</Name>" << '\n';
    os << '\t' << "<ID>" << id << "</ID>" << '\n';
    if (!address.empty())
        os << '\t' << "<Address>" << address << "</Address>" << '\n';
    if (!city.empty())
        os << '\t' << "<City>" << city << "</City>" << '\n';
    if (!state.empty())
        os << '\t' << "<State>" << state << "</State>" << '\n';
    if (!country.empty())
        os << '\t' << "<Country>" << country << "</Country>" << '\n';
    if (!phone.empty())
        os << '\t' << "<Phone>" << phone << "</Phone>" << '\n';
    if (salary > 0)
        os << '\t' << "<Salary>" << salary << "</Salary>" << '\n';
    os << "</Employee>" << '\n'
       << '\n';
    os.flush();

    return;
}

Employee *Employee::read(std::istream &is)
{
    EmployeeRec rec;
    is.read(reinterpret_cast<char *>(&rec), sizeof rec);

    if (is)
        return new Employee(std::string(rec.name), rec.id, std::string(rec.address), std::string(rec.city), std::string(rec.state), std::string(rec.country), std::string(rec.phone), rec.salary);

    // End of file
    return nullptr;
}

Employee *Employee::retrieve(std::istream &is, int _id)
{
    is.clear();
    is.seekg(0, std::ios_base::beg);

    // Search ids to find correct record
    int i{0};
    while (!is.eof())
    {
        // Gets converted into a unique_ptr
        Employee *emp{read(is)};

        i++;

        if (emp)
            if (emp->id == _id)
                return emp;
    }

    // Set is to just before end of file
    is.clear();
    is.seekg(sizeof(EmployeeRec) * (i - 1));

    return nullptr;
}

Employee *Employee::fromXML(std::istream &is)
{
    const std::vector<std::string> validTags{"name", "id", "address", "city", "state", "country", "phone", "salary"};
    std::vector<std::unique_ptr<Tag>> tags;
    bool hasSearchedEmployee{false};

    // Get fist tag and make sure it's employee
    tags.push_back(std::unique_ptr<Tag>(getNextTag(is)));

    if (!tags.back())
        return nullptr;

    if (tags.back()->name != "employee")
        throw std::runtime_error("First tag is not employee");

    tags.erase(tags.begin());

    while (!hasSearchedEmployee)
    {
        tags.push_back(std::unique_ptr<Tag>(getNextTag(is)));

        // Stops loop when closing employee tag is found
        if (tags.back()->name == "employee" && tags.back()->isClosing)
        {
            tags.pop_back();
            hasSearchedEmployee = true;
            break;
        }

        // Checks for valid tags
        if (std::find(validTags.begin(), validTags.end(), tags.back()->name) == validTags.end())
            throw std::runtime_error("Invalid tag: " + tags.back()->name);

        // If opening tag, get following value
        if (!tags.back()->isClosing)
        {
            char c;

            while (is.peek() != '<')
            {
                is.get(c);
                tags.back()->value += c;
            }
        }

        if (is.eof())
            throw std::runtime_error("Missing closing employee tag");
    }

    // Every open tag is followed by the appropriate close
    for (auto it{tags.begin() + 1}; it != tags.end() - 1; it += 2)
    {
        if (!(*it)->isClosing || (*(it - 1))->isClosing)
            throw std::runtime_error("Mismatch of opening and closing tags between " + (*(it - 1))->name + " and " + (*it)->name);
        if ((*it)->name != (*(it - 1))->name)
            throw std::runtime_error((*it)->name + " opening and closing tag names don't match");
    }

    // Remove closing tags
    for (auto it{tags.begin()}; it != tags.end();)
    {
        if ((*it)->isClosing)
            tags.erase(it);
        else
            ++it;
    }

    // Check for duplicate tags
    for (auto it{tags.begin()}; it != tags.end(); ++it)
        for (auto it1{tags.begin()}; it1 != tags.end(); ++it1)
        {
            if (it != it1 && (*it)->name == (*it1)->name)
                throw std::runtime_error("Multiple " + (*it)->name + " tags");
        }

    // Put all the tags in order to create the object
    std::vector<std::string> params;

    for (const auto &name : validTags)
    {
        // Replacement for std::find
        auto inTags = [&]()
        {
            for (auto it{tags.begin()}; it != tags.end(); ++it)
            {
                if ((*it)->name == name)
                    return it;
            }

            return tags.end();
        };

        auto res = inTags();

        if (res == tags.end())
        {
            // Tag doesn't exist

            if (name == "name")
                throw std::runtime_error("Missing name tag");
            else if (name == "id")
                throw std::runtime_error("Missing id tag");

            params.push_back("");
        }
        else
            params.push_back((*res)->value);
    }

    return new Employee(params[0], std::stoi(params[1]), params[2], params[3], params[4], params[5], params[6], std::stod(params[7]));
}

Employee::Tag *Employee::getNextTag(std::istream &is)
{
    std::string name{""};
    bool isClosing = false;
    bool foundTransitionChar{false};
    char c;

    // Find opening tag '<' and append to tag also check if closing tag
    while (!foundTransitionChar)
    {
        is.get(c);

        if (c == '<')
        {
            if (is.peek() == '/')
            {
                isClosing = true;
                is.get(c);
            }

            foundTransitionChar = true;
            break;
        }

        if (is.eof())
            return nullptr;
    }

    foundTransitionChar = false;

    // Store tag name and find closing tag '>' and add them to tag
    while (!foundTransitionChar)
    {
        is.get(c);

        if (c == '>')
        {
            foundTransitionChar = true;
            break;
        }
        else
            name += static_cast<char>(tolower(c));

        if (is.eof())
            throw std::runtime_error("Missing tag closing character '>'");
    }

    return new Tag(name, isClosing);
}

void Employee::stobuf(const std::string &s, char *buf, size_t limit)
{
    std::size_t nchars{std::min(limit - 1, s.size())};

    s.copy(buf, nchars);
    buf[nchars] = '\0';

    return;
}

void Employee::setSalary(double d) { salary = d; }
double Employee::getSalary() { return salary; }
