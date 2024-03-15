rcpptimer 1.1.0
==============

## Improvements
* Warn about timers that are not stopped when aggregate is called (no matching `toc()` statement).
* Warn about timers for which no matching `tic()` statement was found.
* Add `verbose` parameter to the `Timer` class to control whether above warnings should be printed or not (defaults to `true`).

## Fixes
* Fixed cases where only `toc()` was called without matching `tic()`.
  * This led to a segfault in the previous version. 
* Fix `reset()` method wich was not working properly in some cases where timers spread out over multiple methods.

rcpptimer 1.0.0
==============

This is the initial release of `rcpptimer`. It is based on `RcppClock` but contains a number of improvements:
- OpenMP support
- Auomatically returns results to R as soon as the C++ Object goes out of scope
- Fast computation of Mean and Standard Deviation of the results in C++
- Uses `tic` and `toc` instead of `tick` and `tock` to be consistent with R's `tictoc` package
- Allways reports microseconds resolution
- Many more performance improvements
