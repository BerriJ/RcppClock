#ifndef rcpptimer
#define rcpptimer

#ifdef _OPENMP
#include <omp.h>
#endif

#include <Rcpp.h>
#include <cpptimer/cpptimer.h>
#include <string>
#include <vector>

namespace Rcpp
{
  // This class inherits its main functionality from CppClock
  // It justs extends it with a stop method that passes the data to R and
  // a destructor that calls stop for convenience.
  class Timer : public CppTimer
  {

  public:
    bool autoreturn = true;

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

    // Destructor
    ~Clock()
    {
      if (autoreturn)
        stop();
    }
  };
}
#endif