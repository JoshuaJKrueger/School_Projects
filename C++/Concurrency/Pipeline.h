#include <array>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>

class Pipeline
{
private:
    struct MuxQueue
    {
        std::queue<long long int> q;
        std::condition_variable cond;
        std::mutex sync;
    };

    static MuxQueue rand_ints;
    static MuxQueue filtered_ints;

    static std::mutex write;
    static std::mutex print;

    static std::atomic_size_t generators;
    static std::atomic_size_t filterers;

    static std::random_device seed;
    static std::mt19937_64 generator;
    static std::uniform_int_distribution<long long int> dist;

public:
    static const std::size_t gen_cnt = 4;
    static const std::size_t fil_cnt = 3;
    static const std::size_t grp_cnt = 10;
    static const std::size_t int_cnt = static_cast<std::size_t>(1000);

    static std::array<std::atomic_size_t, grp_cnt> grp_cnts;
    static std::array<std::ofstream, grp_cnt> oss;

    static void generate_ints()
    {
        for (std::size_t i{0}; i < int_cnt; ++i)
        {
            long long int n{dist(generator)};

            std::unique_lock<std::mutex> int_q_lock{rand_ints.sync};
            rand_ints.q.push(n);
            int_q_lock.unlock();
            rand_ints.cond.notify_one();
        }

        --generators;
        rand_ints.cond.notify_all();
    }

    static void filter_ints()
    {
        for (;;)
        {
            std::unique_lock<std::mutex> int_q_lock{rand_ints.sync};
            rand_ints.cond.wait(int_q_lock, []()
                                { return !rand_ints.q.empty() || !generators; });

            if (rand_ints.q.empty())
                break;

            auto item{rand_ints.q.front()};
            rand_ints.q.pop();
            int_q_lock.unlock();

            if (item % 3LL == 0)
                continue;

            std::unique_lock<std::mutex> filtered_q_lock{filtered_ints.sync};
            filtered_ints.q.push(item);
            filtered_q_lock.unlock();
            filtered_ints.cond.notify_all();
        }

        --filterers;
        filtered_ints.cond.notify_all();
    }

    static void group_ints(std::size_t id)
    {
        for (;;)
        {
            std::unique_lock<std::mutex> filtered_q_lock{filtered_ints.sync};
            filtered_ints.cond.wait(filtered_q_lock, []()
                                    { return !filtered_ints.q.empty() || !filterers; });

            if (filtered_ints.q.empty())
                break;

            auto item{filtered_ints.q.front()};
            if (std::abs(item % 10) == id)
            {
                filtered_ints.q.pop();
                filtered_q_lock.unlock();

                grp_cnts.at(id)++;

                oss.at(id) << item << std::endl;
            }
            else
            {
                filtered_q_lock.unlock();
                filtered_ints.cond.notify_all();
                continue;
            }
        }

        std::lock_guard<std::mutex> p_lock(print);
        std::cout << "Group " << id << " has " << grp_cnts.at(id) << " numbers" << std::endl;
    }
};
