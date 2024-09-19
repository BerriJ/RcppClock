#include <rcpptimer.h>

// a simple timing example
long int fib(long int n)
{
  return ((n <= 1) ? n : fib(n - 1) + fib(n - 2));
}

//' Simple rcpptimer example
//'
//' Time the computation of Fibonacci numbers
//'
//' @details
//' The function being timed is the following:
//'
//' \code{int fib(int n) { return ((n <= 1) ? n : fib(n - 1) + fib(n - 2)); }}
//'
//' Runtime for computations less than \code{n = 15} is nearly unmeasurable.
//'
//' @param n vector giving integers for which to compute the Fibonacci sum
//' @return vector of integers giving the Fibonacci sum for each element in
//' \code{n}
//' @export
//' @examples
//' \donttest{
//' fibonacci(n = rep(20:25, 10))
//' # this function creates a global environment variable "times"
//' times
//' }
//[[Rcpp::export]]
std::vector<long int> fibonacci(std::vector<long int> n)
{
  Rcpp::Timer timer;

  // This scoped timer measures the total execution time of 'fibonacci'
  Rcpp::Timer::ScopedTimer scpdtmr(timer, "fib_body");

  std::vector<long int> results = n;

  for (unsigned int i = 0; i < n.size(); ++i)
  {
    timer.tic("fib_" + std::to_string(n[i]));
    results[i] = fib(n[i]);
    timer.toc("fib_" + std::to_string(n[i]));
  }

  return (results);
}

//' Simple rcpptimer example using OpenMP
//'
//' Time the multithreaded computation of Fibonacci numbers
//'
//' @details
//' The function being timed is the following:
//'
//' \code{int fib(int n) { return ((n <= 1) ? n : fib(n - 1) + fib(n - 2)); }}
//'
//' Runtime for computations less than \code{n = 15} is nearly unmeasurable.
//'
//' @param n vector giving integers for which to compute the Fibonacci sum
//' @return vector of integers giving the Fibonacci sum for each element in
//' \code{n}
//' @export
//' @examples
//' \donttest{
//' fibonacci_omp(n = rep(20:25, 10))
//' # this function creates a global environment variable "times"
//' times
//' }
//[[Rcpp::export]]
std::vector<long int> fibonacci_omp(std::vector<long int> n)
{

  Rcpp::Timer timer;

  // This scoped timer measures the total execution time of 'fibonacci'
  Rcpp::Timer::ScopedTimer scpdtmr(timer, "fib_body");

  std::vector<long int> results = n;

#pragma omp parallel for
  for (unsigned int i = 0; i < n.size(); ++i)
  {
    timer.tic("fib_" + std::to_string(n[i]));
    results[i] = fib(n[i]);
    timer.toc("fib_" + std::to_string(n[i]));
  }

  return (results);
}
