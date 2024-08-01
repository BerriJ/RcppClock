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
rm(times, envir = as.environment(".GlobalEnv"))

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
expect_false("times" %in% ls(as.environment(".GlobalEnv")))

# Test that the object can be named something other than times
Rcpp::cppFunction('
void autoreturn_custom_name()
{
  Rcpp::Timer timer("my_own_name");
  Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
  timer.tic("t2");
  std::string s;
  s.reserve(1048576);
  timer.toc("t2");
}',
  depends = "rcpptimer"
)

expect_no_warning(autoreturn_custom_name())
expect_no_error(autoreturn_custom_name())
expect_invisible(autoreturn_custom_name())

expect_contains(ls(as.environment(".GlobalEnv")), "my_own_name")
rm(my_own_name, envir = as.environment(".GlobalEnv"))

# Test the above using another constructor:
Rcpp::cppFunction('
void autoreturn_custom_name()
{
  Rcpp::Timer timer("my_own_name", true); // Providing name and verbosity
  Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
  timer.tic("t2");
  std::string s;
  s.reserve(1048576);
  timer.toc("t2");
}',
  depends = "rcpptimer"
)

expect_no_warning(autoreturn_custom_name())
expect_no_error(autoreturn_custom_name())
expect_invisible(autoreturn_custom_name())

expect_contains(ls(as.environment(".GlobalEnv")), "my_own_name")
rm(my_own_name, envir = as.environment(".GlobalEnv"))
