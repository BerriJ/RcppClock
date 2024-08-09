# rcpptimer <a href="https://rcpptimer.berrisch.biz"><img src="man/figures/rcpptimer_hex.png" align="right" height="138" /></a>

This R Package provides Rcpp bindings for [cpptimer](https://github.com/BerriJ/cpptimer), a simple tic-toc timer class for benchmarking C++ code. It's not just simple, it's blazing fast! This sleek tic-toc timer class supports nested and overlapping timers and OpenMP parallelism. It boasts a nanosecond-level time resolution. Results (with summary statistics) are automatically passed back to R as a data frame.

## Install

Install rcpptimer from CRAN.

```
install.packages("rcpptimer")
```

## Basic Usage with Rcpp::cppFunction

Here is a straightforward example of using the `Rcpp::Timer` with Rcpp::cppFunction:

```r
Rcpp::cppFunction("
double demo_rnorm()
{
  Rcpp::Timer timer;
  timer.tic();
  double x = rnorm(1, 1)[0];
  timer.toc();
  return(x);
}",
  depends = "rcpptimer"
)

demo_rnorm()
```

The timer object will automatically write its result to the R environment:

```r
print(times)
       Microseconds SD Count
tictoc         5.48  0     1
```

Check out the [Documentation](https://rcpptimer.berrisch.biz/articles/rcpptimer.html) for:

- Setting up multiple, nested, and overlapping timers
- Using OpenMP parallelism
- Using rcpptimer with SourceCpp
- Adding rcpptimer to your package

## Limitations

Processes taking less than a nanosecond cannot be timed.

Unmatched `.tic()` and `.toc()` calls do not raise errors at compile time. However, they throw warnings at runtime.

## Acknowledgments

This package (and the underlying [cpptimer](https://github.com/BerriJ/cpptimer) class) was inspired by [zdebruine](https://github.com/zdebruine)'s [RcppClock](https://github.com/zdebruine/RcppClock). I used that package a lot and wanted to add OpenMP support, alter the process of calculating summary statistics, and apply a series of other small adjustments. I hope you find it useful.