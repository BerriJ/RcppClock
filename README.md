# RcppTimeR - Rcpp Tic-Toc Timer with OpenMP Support

This R Package provides Rcpp bindings for [cpptimer](https://github.com/BerriJ/cpptimer), a simple tic-toc timer class for benchmarking C++ code. It's not just simple, it's blazing fast! This sleek tic-toc timer class supports overlapping timers as well as OpenMP parallelism. It boasts a microsecond-level time resolution. We did not find any overhead of the timer itself at this resolution. Results (with summary statistics) are automatically passed back to R as a data frame.


## Install

Install rcpptimer from CRAN.

```
install.packages("rcpptimer")
```

## The Rcpp side of things

Link it in your `DESCRIPTION` file or with `//[[Rcpp::depends(rcpptimer)]]`, and load the header library into individual `.cpp` files with `#include <rcpptimer.h>`. Then create an instance of the Rcpp::Clock class and use:

`.tic(std::string)` to start a new timer. `.toc(std::string)` to stop the timer.

```c++
//[[Rcpp::depends(rcpptimer)]]
#include <rcpptimer.h>

std::vector<int> fibonacci(std::vector<int> n)
{
  Rcpp::Timer timer; // Or Rcpp::Timer timer("my_name"); to assign a custom name
  // to the returned dataframe (default is 'times')
  timer.tic("fib_body"); // Start timer measuring the whole function
  std::vector<int> results = n;

  for (int i = 0; i < n.size(); ++i)
  {
    // Start a timer for each fibonacci number
    timer.tic("fib_" + std::to_string(n[i]));
    results[i] = fib(n[i]);
    // Stop the timer for each fibonacci number
    timer.toc("fib_" + std::to_string(n[i]));
  }
  // Stop the timer measuring the whole function
  timer.toc("fib_body");
  return (results);
}
```
Multiple timers with the same name (i.e. in a loop) will be grouped and we report the Mean and Standard Deviation for them. The results will be automatically passed to R as the `timer` instance goes out of scope. You don't need to worry about return statements.

## The R side of things

On the R end, we can now observe the `times` object that was silently passed to the global environment:

```r
[R] fibonacci(n = rep(10 * (1:4), 10))
[R] times
      Name Milliseconds    SD Count
1   fib_10        0.002 0.001    10
2   fib_20        0.048 0.011    10
3   fib_30        5.382 0.070    10
4   fib_40      658.280 1.520    10
5 fib_body     6637.259 0.000     1
```

## OpenMP Support

Since we added OpenMP support, we also have an OpenMP version of the `fibonacci` function:

```c++
std::vector<int> fibonacci_omp(std::vector<int> n)
{

  Rcpp::Timer timer;
  timer.tic("fib_body");
  std::vector<int> results = n;

#pragma omp parallel for
  for (int i = 0; i < n.size(); ++i)
  {
    timer.tic("fib_" + std::to_string(n[i]));
    results[i] = fib(n[i]);
    timer.toc("fib_" + std::to_string(n[i]));
  }
  timer.toc("fib_body");
  return (results);
}
```

Nothing has to be changed with respect to your `timer` instance. The timings show that the OpenMP version is significantly faster (fib_body):

```r
      Name Milliseconds     SD Count
1   fib_10        0.022  0.031    10
2   fib_20        0.132  0.057    10
3   fib_30        8.728  2.583    10
4   fib_40      779.942 91.569    10
5 fib_body      908.919  0.000     1
```

## Scoped Timer

We also added a new `Rcpp::CppTimer::ScopedTimer`. This can be used to time the lifespan of an object until it goes out of scope. This is useful for timing the duration of a function or a loop. Below is the `fibonacci` example from above. However, we replace the "fib_body" tic-toc timer with the scoped version.

```c++
std::vector<int> fibonacci(std::vector<int> n)
{
  Rcpp::Timer timer;

  // This scoped timer measures the total execution time of 'fibonacci'
  Rcpp::Timer::ScopedTimer scpdtmr(timer, "fib_body");

  std::vector<int> results = n;

  for (unsigned int i = 0; i < n.size(); ++i)
  {
    timer.tic("fib_" + std::to_string(n[i]));
    results[i] = fib(n[i]);
    timer.toc("fib_" + std::to_string(n[i]));
  }

  return (results);
}
```
Note that you can name your object (in this example `scpdtmr`) however you like. `Rcpp::CppTimer::ScopedTimer` acts as a wrapper, so it will call `.tic` upon construction and `.toc` will be called automatically upon destruction. 

`Rcpp::CppTimer::ScopedTimer` is useful to time the duration of a function or a loop.

## Limitations

Processes taking less than a microsecond cannot be timed.

## Acknowledgments

This package (and the underlying [cpptimer](https://github.com/BerriJ/cpptimer) class) was inspired by [zdebruine](https://github.com/zdebruine)'s [RcppClock](https://github.com/zdebruine/RcppClock). I used that package a lot and wanted to add OpenMP support, alter the process of calculating summary statistics, and apply a series of other small adjustments. I hope you find it useful.