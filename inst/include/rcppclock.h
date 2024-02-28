#ifndef clock_h
#define clock_h

#ifdef _OPENMP
#include <omp.h>
#endif

#include <Rcpp.h>
#include <chrono>
#include <string>
#include <vector>
#include <map>

#ifndef _OPENMP
inline int omp_get_thread_num() { return 0; }
#endif

namespace Rcpp
{
  template <typename T>
  std::vector<T> remove_duplicates(std::vector<T> vec)
  {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    return (vec);
  }

  namespace sc = std::chrono;

  class Clock
  {
    using hr_clock = sc::high_resolution_clock;
    using keypair = std::pair<std::string, unsigned int>;

  private:
    // Data to be returned: Tag, Mean, SD, Count
    std::map<std::string, std::tuple<double, double, unsigned long int>> data;
    std::map<keypair, hr_clock::time_point> ticks; // Map of start times
    std::vector<std::string> tags;                 // Vector of identifiers
    std::vector<unsigned long long int> durations; // Vector of durations

  public:
    std::string name; // Name of R object to return

    // Init - Set name of R object
    Clock() : name("times") {}
    Clock(std::string name) : name(name) {}

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

      tags.clear();
      durations.clear();
      ticks.clear();
    }

    // Pass data to R / Python
    void stop()
    {
      aggregate();

      // Output Objects
      std::vector<std::string> out_tags;
      std::vector<unsigned long int> out_counts;
      std::vector<double> out_means, out_sd;

      for (auto const &ent1 : data)
      {
        // Save tag
        out_tags.push_back(ent1.first);

        unsigned long int count = std::get<2>(ent1.second);
        double mean = std::get<0>(ent1.second);
        double variance = std::get<1>(ent1.second) / count;

        // Convert to milliseconds and round to 3 decimal places
        out_means.push_back(std::round(mean) * 1e-3);
        out_sd.push_back(std::round(std::sqrt(variance * 1e-6) * 1e+3) * 1e-3);
        out_counts.push_back(count);
      }

      DataFrame df = DataFrame::create(
          Named("Name") = out_tags,
          Named("Milliseconds") = out_means,
          Named("SD") = out_sd,
          Named("Count") = out_counts);

      Environment env = Environment::global_env();
      env[name] = df;
    }

    void reset()
    {
      data.clear();
    }

    // Destructor
    ~Clock()
    {
      stop();
    }
  };
}
#endif