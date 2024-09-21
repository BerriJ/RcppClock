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

using namespace std;

namespace Rcpp
{
  // This class inherits its main functionality from CppClock
  // It justs extends it with a stop method that passes the data to R and
  // a destructor that calls stop for convenience.
  class Timer : public CppTimer
  {

  public:
    string name = "times"; // Name of R object to return
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
        string msg;
        msg += "Timer \"" + tag + "\" not started yet. \n" +
               "Use tic(\"" + tag + "\") to start the timer.";
        Rcpp::warning(msg);
      }

      // All entries in tics which have max() assigned are fine
      for (auto const &tic : tics)
      {
        if (tic.second != tic.second.max())
        {
          string msg;
          msg += "Timer \"" + tic.first.first + "\" not stopped yet. \n" +
                 "Use toc(\"" + tic.first.first + "\") to stop the timer.";
          Rcpp::warning(msg);
        }
      }

      // Warn about superfluous toc calls
      for (auto const &tag : needless_tocs)
      {
        string msg;
        msg += "Timer \"" + tag + "\" stopped more than once. \n" +
               "Only the first .toc(\"" + tag + "\") was considered. \n";
        Rcpp::warning(msg);
      }
      missing_tics.clear(), needless_tocs.clear();
    }

    // Pass data to R / Python
    DataFrame stop()
    {
      aggregate(), fesetround(FE_TONEAREST);

      // Output Objects
      vector<string> out_tags;
      vector<unsigned long int> out_counts;
      vector<double> out_mean, out_sd, out_min, out_max;

      for (auto const &entry : data)
      {
        out_tags.push_back(entry.first);

        auto [mean, sst, min, max, count] = entry.second;

        // round to the nearest integer and to even in halfway cases and
        // convert to microseconds
        out_mean.push_back(nearbyint(mean) * 1e-3);
        // Bessels' correction
        double variance = sst / std::max(double(count - 1), 1.0);
        out_sd.push_back(nearbyint(std::sqrt(variance * 1e-6) * 1e+3) * 1e-3);
        out_min.push_back(nearbyint(min) * 1e-3);
        out_max.push_back(nearbyint(max) * 1e-3);
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
