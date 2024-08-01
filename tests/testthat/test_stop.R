# Test if the stop method return a dataframe with the results

Rcpp::cppFunction('
DataFrame manual_return()
{
  Rcpp::Timer timer;
  {
  Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
  timer.autoreturn = false;
  timer.tic("t2");
  std::string s;
  s.reserve(1048576);
  timer.toc("t2");
  }
  DataFrame results = timer.stop();
  return(results);
}',
  depends = "rcpptimer"
)

expect_no_warning(manual_return())
expect_no_error(manual_return())
expect_visible(manual_return())
results <- manual_return()
expect_s3_class(results, "data.frame")

# Test if the stop method and autoreturn work in conjunction

Rcpp::cppFunction('
DataFrame manual_return()
{
  Rcpp::Timer timer;
  {
  Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
  timer.autoreturn = true;
  timer.tic("t2");
  std::string s;
  s.reserve(1048576);
  timer.toc("t2");
  }
  DataFrame results = timer.stop();
  return(results);
}',
  depends = "rcpptimer"
)

expect_no_warning(manual_return())
expect_no_error(manual_return())
expect_visible(manual_return())
results <- manual_return()
expect_s3_class(results, "data.frame")
results_global <- as.environment(".GlobalEnv")$times
expect_s3_class(results_global, "data.frame")
expect_identical(results, results_global)
