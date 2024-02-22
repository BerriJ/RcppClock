#include <rcppclock.h>

// a simple timing example
int fib(int n)
{
  return ((n <= 1) ? n : fib(n - 1) + fib(n - 2));
}

//' Simple RcppClock example
//'
//' Time the computation of fibonacci numbers
//'
//' @details
//' The function being timed is the following:
//'
//' \code{int fib(int n) { return ((n <= 1) ? n : fib(n - 1) + fib(n - 2)); }}
//'
//' Runtime for computations less than \code{n = 25} is nearly unmeasurable.
//'
//' @param n vector giving integers for which to compute the fibonacci sum
//' @param reps number of replicates for timing
//' @export
//' @examples
//' fibonacci(n = 10*(1:4), reps = 10)
//' # this function creates a global environment variable "times"
//' #   that is an S3 RcppClock object
//' times
//[[Rcpp::export]]
void fibonacci(std::vector<int> n, int reps = 10)
{
  Rcpp::Clock clock;

  clock.tick("fib");

  for (int i = 0; i < reps; ++i)
  {
    for (auto number : n)
    {
      clock.tick("fib" + std::to_string(number));
      fib(number);
      clock.tock("fib" + std::to_string(number));
    }
  }
  clock.tock("fib");
}

//' Simple RcppClock example using OpenMP
//'
//' Time the multithreaded computation of fibonacci numbers
//'
//' @details
//' The function being timed is the following:
//'
//' \code{int fib(int n) { return ((n <= 1) ? n : fib(n - 1) + fib(n - 2)); }}
//'
//' Runtime for computations less than \code{n = 25} is nearly unmeasurable.
//'
//' @param n vector giving integers for which to compute the fibonacci sum
//' @param reps number of replicates for timing
//' @export
//' @examples
//' fibonacci_omp(n = 10*(1:4), reps = 10)
//' # this function creates a global environment variable "times"
//' #   that is an S3 RcppClock object
//' times
//[[Rcpp::export]]
void fibonacci_omp(std::vector<int> n, int reps = 10)
{
  Rcpp::Clock clock;

  clock.tick("fib");

#pragma omp parallel for collapse(2)
  for (int i = 0; i < reps; ++i)
  {
    for (auto number : n)
    {
      clock.tick("fib" + std::to_string(number));
      fib(number);
      clock.tock("fib" + std::to_string(number));
    }
  }
  clock.tock("fib");
}