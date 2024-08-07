# rcpptimer - Rcpp Tic-Toc Timer with OpenMP Support

This R Package provides Rcpp bindings for [cpptimer](https://github.com/BerriJ/cpptimer), a simple tic-toc timer class for benchmarking C++ code. It's not just simple, it's blazing fast! This sleek tic-toc timer class supports nested and overlapping timers as well as OpenMP parallelism. It boasts a microsecond-level time resolution. Results (with summary statistics) are automatically passed back to R as a data frame.

## Install

Install rcpptimer from CRAN.

```
install.packages("rcpptimer")
```

## Basic Usage with Rcpp::cppFunction

Here is a very simple example of using the `Rcpp::Timer` with Rcpp::cppFunction:

```r
Rcpp::cppFunction("
int mem()
{
  Rcpp::Timer timer;   // Create a timer object
  timer.tic();         // Start the timer
  std::string s;
  s.reserve(1048576);
  timer.toc();         // Stop the timer
  return(0);
}",
  depends = "rcpptimer"
)

mem()
```


The output is automatically written to your R environment as a data frame:

```r
print(times)

    Name Milliseconds SD Count
1 tictoc        0.005  0     1
```

Check out the [Documentation](https://rcpptimer.berrisch.biz/articles/rcpptimer.html) for:

- Setting up multiple, nested, and overlapping timers
- Using OpenMP parallelism
- Using rcpptimer with SourceCpp
- Adding rcpptimer to your package

## Limitations

Processes taking less than a microsecond cannot be timed.

Unmatched `.tic()` and `.toc()` calls do not raise errors at compile time. However, they throw warnings at runtime.

## Acknowledgments

This package (and the underlying [cpptimer](https://github.com/BerriJ/cpptimer) class) was inspired by [zdebruine](https://github.com/zdebruine)'s [RcppClock](https://github.com/zdebruine/RcppClock). I used that package a lot and wanted to add OpenMP support, alter the process of calculating summary statistics, and apply a series of other small adjustments. I hope you find it useful.