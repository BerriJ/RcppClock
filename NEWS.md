rcpptimer 1.2.1
==============

## Fixes
* This release fixes a bug where calling `.toc()` on a timer that was already stopped would cause a signed integer overflow. This a minor change in `CppTimer` as well as `Rcpp::Timer`.

rcpptimer 1.2.0
==============

## Improvements

* The `stop()` method of `Rcpp::Timer` now returns a `data.frame` with the results. This is useful if you want to set auto return to false and manually handle the results. It is also possible to call `aggregate()` and access the public variable `data` of `Rcpp::Timer`. `data` is a map containing the results (Names, Mean, Standard Deviation, Count). Look into the implementation of `stop()` in "inst/rcpptimer.h" to see how this works.
* The tag arguments of `tic()`, `toc()` and `ScopedTimer()` have default values now.
* The existing vignette was updated, and we added four new vignettes to the package.
* rcpptimer now measures nanoseconds instead of microseconds.
* Adds `print.rcpptimer` method to print the results of a timer object. Timings will be scaled to a more readable unit (e.g. milliseconds, seconds, minutes, hours) when appropriate.
* Warn about timers for which `.toc()` was called more than once.
* The summary now includes the minimum and maximum times.

## Internal Changes
* Added lots of tests to cover 100% of the code.
* Public member "name" was moved from `CppTimer` to `Timer` as it is R-specific.
* Add a hex sticker to the package.
* The package now has a documentation entry (`?rcpptimer`).
* Various improvements to the underlying `CppTimer` class

rcpptimer 1.1.0
==============

## Improvements

* Add new `Rcpp::CppTimer::ScopedTimer` class. This can be used to time the lifespan of an object until it goes out of scope. This is useful for timing the duration of a function or a loop. The `fibonacci` example was updated to use this new class.
* Warn about timers that are not stopped when aggregate is called (no matching `toc()` statement).
* Warn about timers for which no matching `tic()` statement was found.
* Add `verbose` parameter to the `Timer` class to control whether above warnings should be printed or not (defaults to `true`).
* Add introductory vignette to the package.

## Fixes

* Fixed cases where only `toc()` was called without matching `tic()`.
  * This led to a segfault in the previous version. 
* Fix `reset()` method which was not working properly in some cases where timers spread out over multiple methods.
* Fix non-default constructors of the `Timer` class. They were not working properly in the previous version.

rcpptimer 1.0.0
==============

This is the initial release of `rcpptimer`. It is based on `RcppClock` but contains a number of improvements:

* OpenMP support
* Automatically returns results to R as soon as the C++ Object goes out of scope
* Fast computation of Mean and Standard Deviation of the results in C++
* Uses `tic` and `toc` instead of `tick` and `tock` to be consistent with R's `tictoc` package
* Always reports microseconds resolution
* Many more performance improvements
