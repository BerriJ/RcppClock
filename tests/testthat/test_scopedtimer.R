Rcpp::cppFunction('
void scoped_timer()
{
  Rcpp::Timer timer; // Providing name and verbosity
  Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
  std::string s;
  s.reserve(1048576);
}',
  depends = "rcpptimer"
)

expect_no_warning(scoped_timer())
expect_no_error(scoped_timer())
expect_invisible(scoped_timer())

expect_contains(ls(as.environment(".GlobalEnv")), "times")
expect_true(all(!is.na(times)))
expect_gte(min(times$Microseconds), 0)
expect_gte(min(times$SD), 0)
expect_gt(min(times$Count), 0)
expect_true(nrow(times) == 1)
