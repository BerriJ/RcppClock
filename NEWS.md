rcpptimer 1.0.0
==============

This is the initial release of `rcpptimer`. It is based on `RcppClock` but contains a number of improvements:
- OpenMP support
- Auomatically returns results to R as soon as the C++ Object goes out of scope
- Fast computation of Mean and Standard Deviation of the results in C++
- Uses `tic` and `toc` instead of `tick` and `tock` to be consistent with R's `tictoc` package
- Allways reports microseconds resolution
- Many more performance improvements
