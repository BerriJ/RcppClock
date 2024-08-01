# Test if autoreturn works (default)
Rcpp::cppFunction('
void autoreturn_default()
{
  Rcpp::Timer timer;
  Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
  timer.tic("t2");
  std::string s;
  s.reserve(1048576);
  timer.toc("t2");
}',
  depends = "rcpptimer"
)

expect_no_warning(autoreturn_default())
expect_no_error(autoreturn_default())
expect_invisible(autoreturn_default())
expect_contains(ls(as.environment(".GlobalEnv")), "times")

rm(times, autoreturn_default)

# Test that no output is returned when autoreturn is set to false
Rcpp::cppFunction('
void autoreturn_false()
{
  Rcpp::Timer timer;
  timer.autoreturn = false;
  Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
  timer.tic("t2");
  std::string s;
  s.reserve(1048576);
  timer.toc("t2");
}',
  depends = "rcpptimer"
)

expect_no_warning(autoreturn_false())
expect_no_error(autoreturn_false())
expect_equal(ls(), "autoreturn_false")
