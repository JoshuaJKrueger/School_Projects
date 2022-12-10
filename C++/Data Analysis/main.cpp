#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

struct Pulse
{
    std::vector<int>::const_iterator start;
    std::vector<int>::const_iterator peak;
    bool is_piggyback;

    Pulse(std::vector<int>::const_iterator _start, std::vector<int>::const_iterator _peak, bool _is_piggyback = false)
    {
        this->start = _start;
        this->peak = _peak;
        this->is_piggyback = _is_piggyback;
    }
};

/*
    below_drop_ratio: The number of values less than drop_ratio
    drop_ratio: A number less than 1
    pulse_delta: The gap between pulses to look for piggybacks
    vt: voltage_threshold
    attributes listed in alphabetical order purposefully
*/
void find_pulse(int below_drop_ratio, double drop_ratio, int pulse_delta, int voltage_threshold, const std::vector<int> &data, std::vector<Pulse> &pulses);

bool is_numeric(std::string input_string);

int main(int argc, char *argv[])
{
    // Read parameters from ini given in command line arguments
    const std::vector<std::string> valid_parameters{"below_drop_ratio", "drop_ratio", "pulse_delta", "vt", "width"}; // sorted
    std::vector<std::pair<std::string, std::string>> raw_parameters;

    // Get parameters from file
    if (argc > 1)
    {
        if (argc > 2)
            std::cerr << "Too many arguments given\n";

        try
        {
            std::string file_name{argv[1]};
            std::ifstream parameters_file{file_name};

            if (!parameters_file.is_open())
                throw std::runtime_error("Could not open " + file_name);

            for (std::string line; std::getline(parameters_file, line);)
            {
                // Skip comment lines
                if (line.front() == '#')
                    continue;

                std::size_t delimiter = line.find_first_of('=', 1);

                // Split the line at the '=' and push pair into vector
                if (delimiter != std::string::npos)
                    raw_parameters.push_back(std::make_pair(line.substr(0, delimiter), line.substr(delimiter + 1)));
            }

            parameters_file.close();

            // Remove duplicates
            raw_parameters.erase(std::unique(raw_parameters.begin(), raw_parameters.end()), raw_parameters.end());
        }
        catch (const std::runtime_error &error)
        {
            std::cerr << error.what() << '\n';

            return -1;
        }

        // Verify parameters
        try
        {
            std::vector<std::string> parameter_names;

            // Makes working on just the fist object of the pair easier
            for (const std::pair<std::string, std::string> &parameter : raw_parameters)
                parameter_names.push_back(parameter.first);

            // set_difference expects sorted ranges
            std::sort(parameter_names.begin(), parameter_names.end());

            std::vector<std::string> extra_parameters{};
            std::vector<std::string> missing_parameters;

            // Find/Fill exta_parameters and missing_parameters
            std::set_difference(parameter_names.begin(), parameter_names.end(), valid_parameters.begin(), valid_parameters.end(), std::inserter(extra_parameters, extra_parameters.begin()));
            std::set_difference(valid_parameters.begin(), valid_parameters.end(), parameter_names.begin(), parameter_names.end(), std::inserter(missing_parameters, missing_parameters.begin()));

            // Handle missing parameters
            if (!missing_parameters.empty())
            {
                for (const std::string &parameter : missing_parameters)
                    std::cerr << "Missing " << parameter << '\n';

                throw std::runtime_error("");
            }

            // Handle extra parameters (don't exit, inform user and remove from raw_parameters)
            if (!extra_parameters.empty())
            {
                // Remove duplicates in extra_parameters
                extra_parameters.erase(std::unique(extra_parameters.begin(), extra_parameters.end()), extra_parameters.end());

                for (const std::string &parameter : extra_parameters)
                {
                    // Handle duplicate parameters (warn user, keep last instance)
                    // Removed pair duplicates above so these have the same name and different values
                    if (std::find(valid_parameters.begin(), valid_parameters.end(), parameter) != valid_parameters.end())
                    {
                        auto number_of_duplicates{std::count(parameter_names.begin(), parameter_names.end(), parameter) - 1};

                        std::cerr << "Parameter " << parameter << " has " << number_of_duplicates << " duplicate(s)" << '\n';

                        // Remove duplicates leaving the last instance
                        for (int iterator{0}; iterator < number_of_duplicates; ++iterator)
                            raw_parameters.erase(std::remove_if(raw_parameters.begin(), raw_parameters.end(), [&parameter](std::pair<std::string, std::string> raw_parameter)
                                                                { return raw_parameter.first == parameter; }));

                        // Remove from extra_parameters so last instance isn't removed
                        extra_parameters.erase(std::remove(extra_parameters.begin(), extra_parameters.end(), parameter));
                    }
                    else
                        std::cerr << "Parameter " << parameter << " should be removed" << '\n';
                }

                // Removes extra parameters from raw_parameters
                raw_parameters.erase(std::remove_if(raw_parameters.begin(), raw_parameters.end(), [&extra_parameters](std::pair<std::string, std::string> raw_parameter)
                                                    { return std::find(extra_parameters.begin(), extra_parameters.end(), raw_parameter.first) != extra_parameters.end(); }),
                                     raw_parameters.end());
            }

            // Handle invalid values
            for (const std::pair<std::string, std::string> &parameter : raw_parameters)
            {
                try
                {
                    if (!is_numeric(parameter.second))
                        throw std::invalid_argument(parameter.first + "'s value is not numeric");
                }
                catch (const std::invalid_argument &error)
                {
                    std::cerr << error.what() << '\n';

                    return -1;
                }
            }
        }
        catch (const std::runtime_error &error)
        {
            std::cerr << error.what() << '\n';

            return -1;
        }
    }
    else
    {
        std::cerr << "Must give name of ini file\n";

        return -1;
    }

    // Get data (all .dat in current dir) using std::filesystem
    for (const std::filesystem::directory_entry &directory_entry : std::filesystem::recursive_directory_iterator("./dat"))
        if (directory_entry.path().extension() == ".dat")
            try
            {
                std::ifstream data_file{directory_entry.path()};

                if (!data_file.is_open())
                    throw std::runtime_error("Could not open " + directory_entry.path().stem().string());

                std::vector<int> data;
                std::size_t line{0};

                while (!data_file.eof())
                {
                    std::string input{""};
                    data_file >> input;
                    line++;

                    // Skip whitespace
                    if (std::all_of(input.begin(), input.end(), [](char character)
                                    { return isspace(character); }))
                        continue;

                    try
                    {
                        data.push_back(std::stoi(input));
                    }
                    catch (const std::invalid_argument &error)
                    {
                        std::cerr << error.what() << '\n';
                        std::cerr << "Could not convert " << input << " to int; from " << directory_entry.path().stem().string() << " on line " << line << '\n';

                        return -1;
                    }
                }

                data_file.close();

                if (data.empty())
                    throw std::runtime_error("No data in file " + directory_entry.path().stem().string());

                // Negate data
                std::transform(data.begin(), data.end(), data.begin(), [](int number)
                               { return -number; });

                // Smooth data
                std::vector<int> smoothed_data;
                smoothed_data.reserve(data.size());
                // Add first 3 data points
                for (int iterator{0}; iterator < 3; ++iterator)
                    smoothed_data.push_back(data.at(iterator));
                // Add smoothed data
                for (std::vector<int>::iterator iterator{data.begin() + 3}; iterator != data.end() - 3; ++iterator)
                    smoothed_data.push_back((iterator[-3] + 2 * iterator[-2] + 3 * iterator[-1] + 3 * iterator[0] + 3 * iterator[1] + 2 * iterator[2] + iterator[3]) / 15);
                // Add last three
                for (int iterator{3}; iterator > 0; --iterator)
                    smoothed_data.push_back(*(data.end() - iterator));

                // Detect pulse
                std::sort(raw_parameters.begin(), raw_parameters.end());

                std::vector<Pulse> pulses;

                find_pulse(std::stoi(raw_parameters.at(0).second), std::stod(raw_parameters.at(1).second), std::stoi(raw_parameters.at(2).second), std::stoi(raw_parameters.at(3).second), smoothed_data, pulses);

                // Print findings
                std::cout << directory_entry.path().filename().string() << ":" << std::endl;

                // Calc area (sum of start to param with, or next start) (use un-smoothed data) use std::accumulate
                for (std::vector<Pulse>::iterator iterator{pulses.begin()}; iterator != pulses.end(); ++iterator)
                {
                    std::vector<int>::iterator start{data.begin() + (iterator->start - smoothed_data.begin())}; // Turn smoothed data iter into raw data iter
                    int width{std::stoi(raw_parameters.at(4).second)};
                    std::ptrdiff_t distance_to_next_peak_start{iterator + 1 != pulses.end() ? std::distance(iterator->start, (iterator + 1)->start) : PTRDIFF_MAX};
                    int area{std::accumulate(start, start + (width <= distance_to_next_peak_start ? width : distance_to_next_peak_start), 0)};

                    if (iterator->is_piggyback)
                        std::cout << "Piggyback found at " << iterator->start - smoothed_data.begin() << " (" << area << ")" << std::endl;
                    else
                        std::cout << iterator->start - smoothed_data.begin() << " (" << area << ")" << std::endl;
                }

                std::cout << std::endl;
            }
            catch (const std::runtime_error &error)
            {
                std::cerr << error.what() << '\n';

                return -1;
            }

    return 0;
}

bool is_numeric(std::string input_string)
{
    const std::vector<char> valid_characters{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};
    std::size_t decimal_point_count{0};

    for (const char &character : input_string)
    {
        // Make sure all chars are valid
        if (std::find(valid_characters.begin(), valid_characters.end(), character) == valid_characters.end())
            return false;

        // There can only be one '.'
        if (character == '.')
            if (++decimal_point_count > 1)
                return false;
    }

    return true;
}

void find_pulse(int below_drop_ratio, double drop_ratio, int pulse_delta, int voltage_threshold, const std::vector<int> &data, std::vector<Pulse> &pulses)
{

    std::vector<int>::const_iterator pulse_start;
    bool in_pulse = false;

    // Find pulse start to peak
    for (std::vector<int>::const_iterator iterator{data.begin()}; iterator != data.end() - 2; ++iterator)
    {
        // Find start of pulse
        if (*(iterator + 2) - *iterator > voltage_threshold && !in_pulse)
        {
            pulse_start = iterator;
            in_pulse = true;
            iterator += 2;
        }
        else if (*(iterator + 1) < *iterator && in_pulse)
        {
            // Find peak of pulse
            pulses.push_back(Pulse(pulse_start, iterator));
            in_pulse = false;
        }
    }

    // Check for piggyback pulses
    for (std::vector<Pulse>::iterator iterator{pulses.begin()}; iterator != pulses.end() - 1; ++iterator)
    {
        // Check distance between pulses vs pulse_delta
        if (std::distance(iterator->start, (iterator + 1)->start) <= pulse_delta)
        {
            // Check for points that are < drop_ratio * peak_height from first pulse peak to
            //   the start of the second pulse; non-inclusive
            // count_if is [) inclusivity
            int peak_height = *iterator->peak;
            if (std::count_if(iterator->peak + 1, (iterator + 1)->start, [&drop_ratio, &peak_height](int value)
                              { return value < drop_ratio * peak_height; }) > below_drop_ratio)
                // Shouldn't be possible to remove last element, so omitting checks
                iterator->is_piggyback = true;
        }
    }
}
