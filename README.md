# rcppclock

[![](https://cranlogs.r-pkg.org/badges/grand-total/rcppclock)](https://cran.r-project.org/package=RcppML)
[![](https://www.r-pkg.org/badges/version-last-release/rcppclock)](https://cran.r-project.org/package=RcppML)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)

rcppclock is a simple wrapper for `std::chrono::high_resolution_clock` that makes benchmarking Rcpp code easy.

Install rcppclock from CRAN.

```
install.packages("rcppclock")
library(rcppclock)
?rcppclock
```

## The Rcpp side of things

Load the rcppclock header into your R session using `library(rcppclock)`, link it in your `DESCRIPTION` file or with `//[[Rcpp::depends(rcppclock)]]`, and load the header library into individual `.cpp` files with `#include <rcppclock.h>`:

```
//[[Rcpp::depends(rcppclock)]]
#include <rcppclock.h>
#include <thread>

//[[Rcpp::export]]
void sleepy(){
  Rcpp::Clock clock;
  
  clock.tick("both_naps");
  
  clock.tick("short_nap");
  std::this_thread::sleep_for(std::chrono::milliseconds(10));  
  clock.tock("short_nap");
  
  clock.tick("long_nap");
  std::this_thread::sleep_for(std::chrono::milliseconds(100));  
  clock.tock("long_nap");

  clock.tock("both_naps");
  
  // send the times to the R global environment variable, named "naptimes"
  clock.stop("naptimes");
}
```

`.tick(std::string)` starts a new timer. Provide a name to record what is being timed.

`.tock(std::string)` stops a timer. It is important to use the same name as declared in `.tick()`.

`.stop(std::string)` calculates the duration between all `.tick()` and `.tock()` timing results, and creates an object in the R environment with the name provided.

## The R side of things

On the R end, we can now do stuff with the "naptimes" variable that was created in the above Rcpp function:

```{R}
sleepy()
# global variable "naptimes" is now created in the environment
naptimes
```

```{R}
summary(naptimes, units = "us")
```

```{R}
plot(naptimes)
```

## Timing multiple replicates

If a `.tick()` with the same name is called multiple times, rcppclock automatically groups the results.

The following code reproduces the `?fibonacci` function example included in the rcppclock package:

```
int fib(int n) {
  return ((n <= 1) ? n : fib(n - 1) + fib(n - 2));
}

//[[Rcpp::export]]
void fibonacci(std::vector<int> n, int reps = 10) {
  Rcpp::Clock clock;
  
  while(reps-- > 0){
    for(auto number : n){
      clock.tick("fib" + std::to_string(number));
      fib(number);
      clock.tock("fib" + std::to_string(number));
    }
  }
  clock.stop("clock");
}
```

On the R end, we'll get an object named "clock":

```{R}
fibonacci(n = 25:35, reps = 10)
# global variable "clock" is created in the R global environment
clock
```

```{R}
plot(clock)
```

## Limitations

* Not compatible with OpenMP parallelization.
* Processes taking less than a microsecond cannot be reliably timed on some platforms.
