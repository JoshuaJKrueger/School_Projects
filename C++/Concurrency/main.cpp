#include <array>
#include <atomic>
#include <cstddef>
#include <fstream>
#include <limits>
#include <mutex>
#include <random>
#include <string>
#include <thread>

#include "Pipeline.h"

Pipeline::MuxQueue Pipeline::rand_ints;
Pipeline::MuxQueue Pipeline::filtered_ints;

std::mutex Pipeline::write;
std::mutex Pipeline::print;

std::atomic_size_t Pipeline::generators(gen_cnt);
std::atomic_size_t Pipeline::filterers(fil_cnt);

std::random_device Pipeline::seed;
std::mt19937_64 Pipeline::generator(seed());
std::uniform_int_distribution<long long int> Pipeline::dist(LLONG_MIN, LLONG_MAX);

std::array<std::atomic_size_t, Pipeline::grp_cnt> Pipeline::grp_cnts;
std::array<std::ofstream, Pipeline::grp_cnt> Pipeline::oss;

int main()
{
    for (std::size_t i{0}; i < Pipeline::grp_cnt; ++i)
        Pipeline::oss.at(i) = std::ofstream(std::to_string(i));

    std::array<std::thread, Pipeline::gen_cnt> gen_threads;
    std::array<std::thread, Pipeline::fil_cnt> fil_threads;
    std::array<std::thread, Pipeline::grp_cnt> grp_threads;

    for (std::size_t i{0}; i < Pipeline::gen_cnt; ++i)
        gen_threads.at(i) = std::thread(&Pipeline::generate_ints);
    for (std::size_t i{0}; i < Pipeline::fil_cnt; ++i)
        fil_threads.at(i) = std::thread(&Pipeline::filter_ints);
    for (std::size_t i{0}; i < Pipeline::grp_cnt; ++i)
        grp_threads.at(i) = std::thread(&Pipeline::group_ints, i);

    for (auto &gen_thread : gen_threads)
        gen_thread.join();
    for (auto &fil_thread : fil_threads)
        fil_thread.join();
    for (auto &grp_thread : grp_threads)
        grp_thread.join();

    return 0;
}
