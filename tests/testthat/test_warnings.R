# Test if a warning is thrown when verbose = true (default) and toc is missing
Rcpp::cppFunction('
void missing_toc()
{
  Rcpp::Timer timer;
  Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
  timer.tic("t2");
  std::string s;
  s.reserve(1048576);
}',
  depends = "rcpptimer"
)

# We expect that a warning is thrown
expect_warning(test_missings(toc = FALSE),
  'Timer "t2" not stopped yet.',
  ignore.case = FALSE
)

# We expect, despite that warning, times is still returned
expect_contains(ls(as.environment(".GlobalEnv")), "times")
rm(times, envir = as.environment(".GlobalEnv"))

# Test if a warning is thrown when verbose = true (default) and tic is missing

# We expect, despite that warning, times is still returned
expect_warning(test_missings(tic = FALSE),
  'Timer "t2" not started yet.',
  ignore.case = FALSE
)

# We expect, despite that warning, times is still returned
expect_contains(ls(as.environment(".GlobalEnv")), "times")
rm(times, envir = as.environment(".GlobalEnv"))

# Test if no warning is thrown when verbose = false and toc is missing

expect_no_warning(test_missings(toc = FALSE, verbose = FALSE))

# We expect, despite that warning, times is still returned
expect_contains(ls(as.environment(".GlobalEnv")), "times")
rm(times, envir = as.environment(".GlobalEnv"))

# Test no a warning is thrown when verbose = false and tic is missing
Rcpp::cppFunction('
void missing_tic()
{
  Rcpp::Timer timer(false);
  Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
  std::string s;
  s.reserve(1048576);
  timer.toc("t2");
}',
  depends = "rcpptimer"
)

expect_no_warning(test_missings(tic = FALSE, verbose = FALSE))

# We expect, despite that warning, times is still returned
expect_contains(ls(as.environment(".GlobalEnv")), "times")
rm(times, envir = as.environment(".GlobalEnv"))
