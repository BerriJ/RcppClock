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
  void remove_duplicates(std::vector<T> &vec)
  {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  }

  namespace sc = std::chrono;

  class Clock
  {
    using tp = sc::high_resolution_clock::time_point;
    using keypair = std::pair<std::string, unsigned int>;
    using timesmap = std::map<keypair, tp>;

  private:
    std::string name;                      // Name of R object to return
    timesmap tickmap;                      // Map of start times
    std::vector<std::string> tags,         // Vector of identifiers
        unique_tags;                       // Vector of unique identifiers
    std::vector<unsigned long int> counts; // Count occurence of identifiers
    std::vector<double> means, vars;       // Output vecs of mean and sd
    std::vector<unsigned long long int>    // Observed durations
        timers;

    // Summary object: Tag,
    std::map<std::string, std::tuple<double, double, unsigned long int>> timers_new;

  public:
    // Init - Set name of R object
    Clock() : name("times") {}
    Clock(std::string name) : name(name) {}

    // start a timer - save time
    void tick(std::string &&tag)
    {
      keypair key(std::move(tag), omp_get_thread_num());

#pragma omp critical
      tickmap[key] = sc::high_resolution_clock::now();
    }

    // stop a timer - calculate time difference and save key
    void
    tock(std::string &&tag)
    {
      keypair key(std::move(tag), omp_get_thread_num());

#pragma omp critical
      {
        timers.push_back(
            sc::duration_cast<sc::microseconds>(
                sc::high_resolution_clock::now() -
                tickmap[key])
                .count());
        tags.push_back(std::move(key.first));
      }
    }

    // Pass data to R / Python
    void aggregate()
    {
      // Create copy of tags called unique_names
      unique_tags = tags;
      remove_duplicates(unique_tags);

      for (unsigned int i = 0; i < unique_tags.size(); i++)
      {
        unsigned long int count = 0;
        double mean = 0, M2 = 0;

        for (unsigned long int j = 0; j < tags.size(); j++)
        {
          if (tags[j] == unique_tags[i])
          {
            // Welford's online algorithm for mean and variance
            count++;
            double delta = timers[j] - mean;
            mean += delta / count;
            M2 += delta * (timers[j] - mean);
          }
        }

        // Save count
        counts.push_back(count);

        // Save average, round to 3 decimal places
        means.push_back(mean);

        // Calculate sample variance
        double variance = M2 / (count);
        // Save standard deviation, round to 3 decimal places
        vars.push_back(variance);
      }
    }

    // Pass data to R / Python
    void stop()
    {
      aggregate();

      std::vector<unsigned long int> out_counts;
      std::vector<double> out_means, out_sds; // Output vecs of mean and sd

      for (unsigned int i = 0; i < counts.size(); i++)
      {
        out_counts.push_back(counts[i]);
        out_means.push_back(std::round(means[i]) * 1e-3);
        out_sds.push_back(std::round(std::sqrt(vars[i] * 1e-6) * 1e+3) * 1e-3);
      }

      DataFrame df = DataFrame::create(
          Named("Name") = unique_tags,
          Named("Milliseconds") = out_means,
          Named("SD") = out_sds,
          Named("Count") = counts);

      Environment env = Environment::global_env();
      env[name] = df;
    }

    // Destructor
    ~Clock()
    {
      stop();
    }
  };
}
#endif