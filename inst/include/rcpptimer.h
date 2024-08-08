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
    std::string name = "times"; // Name of R object to return
    bool autoreturn = true;

    Timer() : CppTimer() {}
    Timer(const char *name) : CppTimer() { this->name = name; }
    Timer(bool verbose) : CppTimer(verbose) {}
    Timer(const char *name, bool verbose) : CppTimer(verbose) { this->name = name; }

    // Pass data to R / Python
    DataFrame stop()
    {
      aggregate();

      // Output Objects
      std::vector<std::string> out_tags;
      std::vector<unsigned long int> out_counts;
      std::vector<double> out_means, out_sd;

      for (auto const &entry : data)
      {
        // Save tag
        out_tags.push_back(entry.first);

        // Get count, mean and variance
        unsigned long int count = std::get<2>(entry.second);
        double mean = std::get<0>(entry.second);
        double variance = std::get<1>(entry.second);

        // Convert to microseconds and round to 3 decimal places
        out_means.push_back(std::round(mean) * 1e-3);
        out_sd.push_back(std::round(std::sqrt(variance * 1e-6) * 1e+3) * 1e-3);
        out_counts.push_back(count);
      }

      DataFrame results = DataFrame::create(
          Named("Microseconds") = out_means,
          Named("SD") = out_sd,
          Named("Count") = out_counts);
      results.attr("class") = CharacterVector({"rcpptimer", "data.frame"});
      results.attr("row.names") = out_tags;
      if (autoreturn)
      {
        Environment env = Environment::global_env();
        env[name] = results;
      }

      return results;
    }

    // Destructor
    ~Timer()
    {
      if (autoreturn)
        stop();
    }
  };
}
#endif
