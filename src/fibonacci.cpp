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
//' @export
//' @examples
//' fibonacci(n = rep(10*(1:4), 10))
//' # this function creates a global environment variable "times"
//' #   that is an S3 RcppClock object
//' times
//[[Rcpp::export]]
std::vector<int> fibonacci(std::vector<int> n)
{

  Rcpp::Clock clock;
  clock.tick("fib_body");
  std::vector<int> results = n;

  for (int i = 0; i < n.size(); ++i)
  {
    clock.tick("fib_" + std::to_string(n[i]));
    results[i] = fib(n[i]);
    clock.tock("fib_" + std::to_string(n[i]));
  }
  clock.tock("fib_body");
  return (results);
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
//' @export
//' @examples
//' fibonacci_omp(n = rep(10*(1:4), 10))
//' # this function creates a global environment variable "times"
//' #   that is an S3 RcppClock object
//' times
//[[Rcpp::export]]
std::vector<int> fibonacci_omp(std::vector<int> n)
{

  Rcpp::Clock clock;
  clock.tick("fib_body");
  std::vector<int> results = n;

#pragma omp parallel for
  for (int i = 0; i < n.size(); ++i)
  {
    clock.tick("fib_" + std::to_string(n[i]));
    results[i] = fib(n[i]);
    clock.tock("fib_" + std::to_string(n[i]));
  }
  clock.tock("fib_body");
  return (results);
}