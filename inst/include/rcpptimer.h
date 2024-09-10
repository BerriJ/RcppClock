#ifndef rcpptimer
#define rcpptimer

#ifdef _OPENMP
#include <omp.h>
#endif

#include <Rcpp.h>
#include <cpptimer/cpptimer.h>
#include <string>
#include <vector>
#include <cfenv>

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

    // Print warnings:
    void print_warnings()
    {
      // Warn about all timers not being started
      for (auto const &tag : missing_tics)
      {
        std::string msg;
        msg += "Timer \"" + tag + "\" not started yet. \n";
        msg += "Use tic(\"" + tag + "\") to start the timer.";
        Rcpp::warning(msg);
      }

      // All remaining timers in `tics` are not stopped
      for (auto const &tic : tics)
      {
        std::string msg;
        msg += "Timer \"" + tic.first.first + "\" not stopped yet. \n";
        msg += "Use toc(\"" + tic.first.first + "\") to stop the timer.";
        Rcpp::warning(msg);
      }
    }

    // Pass data to R / Python
    DataFrame stop()
    {
      aggregate();

      std::fesetround(FE_TONEAREST);

      // Output Objects
      std::vector<std::string> out_tags;
      std::vector<unsigned long int> out_counts;
      std::vector<double> out_mean, out_sd, out_min, out_max;

      for (auto const &entry : data)
      {
        out_tags.push_back(entry.first);

        auto [mean, sst, min, max, count] = entry.second;

        // round to the nearest integer and to even in halfway cases and
        // convert to microseconds
        out_mean.push_back(std::nearbyint(mean) * 1e-3);
        // Bessels' correction
        double variance = sst / std::max(double(count - 1), 1.0);
        out_sd.push_back(std::nearbyint(std::sqrt(variance * 1e-6) * 1e+3) * 1e-3);
        out_min.push_back(std::nearbyint(min) * 1e-3);
        out_max.push_back(std::nearbyint(max) * 1e-3);
        out_counts.push_back(count);
      }

      DataFrame results = DataFrame::create(
          Named("Microseconds") = out_mean,
          Named("SD") = out_sd,
          Named("Min") = out_min,
          Named("Max") = out_max,
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
      if (verbose)
        print_warnings();
    }
  };
}
#endif
