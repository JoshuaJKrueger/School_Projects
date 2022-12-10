#include <cstddef>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <string>

#define WRAPLENGTH 9

struct StrictWeakOrder
{
    bool operator()(const std::string &a, const std::string &b) const
    {
        std::string temp_a{a};
        std::string temp_b{b};

        for (char &c : temp_a)
            c = toupper(c);

        for (char &c : temp_b)
            c = toupper(c);

        if (temp_a == temp_b)
            return a < b;
        return temp_a < temp_b;
    }
};

void check_word(std::string word, std::size_t &line_count, std::map<std::string, std::map<std::size_t, std::size_t>, StrictWeakOrder> &words_count)
{
    std::regex word_pattern{"[[:alpha:]]([[:alpha:]]|-|')*", std::regex_constants::optimize};
    std::smatch match;

    if (std::regex_search(word, match, word_pattern))
    {
        std::map<std::string, std::map<std::size_t, std::size_t>, StrictWeakOrder>::iterator word_in_map{words_count.find(word)};

        if (word_in_map == words_count.end())
            words_count[match[0]][line_count] = 1;
        else
        {
            std::map<std::size_t, std::size_t>::iterator word_line{word_in_map->second.find(line_count)};

            if (word_line == word_in_map->second.end())
                word_in_map->second.emplace(line_count, 1);
            else
                word_line->second++;
        }
    }

    return;
}

int main(int argc, char *argv[])
{
    std::string file_name;
    std::map<std::string, std::map<std::size_t, std::size_t>, StrictWeakOrder> words_count;

    if (argc > 1)
    {
        if (argc > 2)
            std::cerr << "Too many arguments given" << std::endl;
        else
            file_name = argv[1];
    }
    else
    {
        std::cout << "Please enter name of file to read:" << std::endl;
        std::cin >> file_name;
    }

    try
    {
        std::ifstream input_file{file_name};
        std::size_t line_count{0};

        if (!input_file.is_open())
            throw std::runtime_error("Could not open " + file_name);

        for (std::string line; std::getline(input_file, line);)
        {
            ++line_count;

            std::size_t position{0};

            while ((position = line.find(' ')) != std::string::npos)
            {
                check_word(line.substr(0, position), line_count, words_count);
                line.erase(0, position + 1);
            }

            if (line.find(' ') == std::string::npos && line.length() > 0)
                check_word(line, line_count, words_count);
        }

        input_file.close();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    try
    {
        std::ofstream output_file{"out.txt", std::ofstream::trunc};

        if (!output_file.is_open())
            throw std::runtime_error("Could not open file to write output");

        std::size_t longest_word_length{0};

        for (const std::pair<const std::string, std::map<std::size_t, std::size_t>> &element : words_count)
            longest_word_length = element.first.length() > longest_word_length ? element.first.length() : longest_word_length;

        std::size_t values_printed{0};

        for (const std::pair<const std::string, std::map<std::size_t, std::size_t>> &element : words_count)
        {
            output_file << element.first << std::string(longest_word_length - element.first.length(), ' ') << " : ";
            std::cout << element.first << std::string(longest_word_length - element.first.length(), ' ') << " : ";

            for (std::map<std::size_t, std::size_t>::const_iterator iterator{element.second.cbegin()}; iterator != element.second.cend(); ++iterator)
            {
                output_file << iterator->first << ':' << iterator->second;
                std::cout << iterator->first << ':' << iterator->second;

                if (std::next(iterator) != element.second.end())
                {
                    output_file << ", ";
                    std::cout << ", ";
                }

                if (++values_printed % WRAPLENGTH == 0)
                {
                    output_file << '\n'
                                << std::string(longest_word_length, ' ') << " : ";
                    std::cout << '\n'
                              << std::string(longest_word_length, ' ') << " : ";
                }
            }

            output_file << '\n';
            std::cout << '\n';
            values_printed = 0;
        }

        output_file.close();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
