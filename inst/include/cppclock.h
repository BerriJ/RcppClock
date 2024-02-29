#ifndef cppclock_h
#define cppclock_h

#ifdef _OPENMP
#include <omp.h>
#endif

#include <chrono>
#include <string>
#include <vector>
#include <map>

#ifndef _OPENMP
inline int omp_get_thread_num() { return 0; }
#endif

template <typename T>
std::vector<T> remove_duplicates(std::vector<T> vec)
{
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    return (vec);
}

namespace sc = std::chrono;

class CppClock
{
    using hr_clock = sc::high_resolution_clock;
    using keypair = std::pair<std::string, unsigned int>;

private:
    std::map<keypair, hr_clock::time_point> ticks; // Map of start times
    std::vector<std::string> tags;                 // Vector of identifiers
    std::vector<unsigned long long int> durations; // Vector of durations

protected:
    // Data to be returned: Tag, Mean, SD, Count
    std::map<std::string, std::tuple<double, double, unsigned long int>> data;

public:
    std::string name; // Name of R object to return

    // Init - Set name of R object
    CppClock() : name("times") {}
    CppClock(std::string name) : name(name) {}

    // start a timer - save time
    void tick(std::string &&tag)
    {
        keypair key(std::move(tag), omp_get_thread_num());

#pragma omp critical
        ticks[key] = hr_clock::now();
    }

    // stop a timer - calculate time difference and save key
    void
    tock(std::string &&tag)
    {
        keypair key(std::move(tag), omp_get_thread_num());

#pragma omp critical
        {
            durations.push_back(
                sc::duration_cast<sc::microseconds>(
                    hr_clock::now() - ticks[key])
                    .count());
            tags.push_back(std::move(key.first));
        }
    }

    // Pass data to R / Python
    void aggregate()
    {

        // Vector of unique identifiers
        std::vector<std::string> unique_tags = remove_duplicates(tags);

        for (unsigned int i = 0; i < unique_tags.size(); i++)
        {

            std::string tag = unique_tags[i];

            unsigned long int count;
            double mean, M2;

            // Init
            if (data.count(tag) == 0)
            {
                count = 0;
                mean = 0;
                M2 = 0;
            }
            else
            {
                mean = std::get<0>(data[tag]);
                M2 = std::get<1>(data[tag]);
                count = std::get<2>(data[tag]);
            }

            // Update
            for (unsigned long int j = 0; j < tags.size(); j++)
            {
                if (tags[j] == tag)
                {
                    // Welford's online algorithm for mean and variance
                    count++;
                    double delta = durations[j] - mean;
                    mean += delta / count;
                    M2 += delta * (durations[j] - mean);
                }
            }

            // Save
            data[tag] = std::make_tuple(mean, M2, count);
        }

        ticks.clear();
        tags.clear();
        durations.clear();
    }

    void reset()
    {
        data.clear();
    }
};

#endif