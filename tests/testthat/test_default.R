# Test if everything works with default settings
Rcpp::cppFunction("
void everything_default()
{
  Rcpp::Timer timer;
  Rcpp::Timer::ScopedTimer scoped_timer(timer);
  timer.tic();
  std::string s;
  timer.toc();
  s.reserve(1048576);
}",
  depends = "rcpptimer"
)

everything_default()

# We expect, despite that warning, times is still returned
expect_contains(ls(as.environment(".GlobalEnv")), "times")

expect_true(all(times$Name == c("scoped", "tictoc")))
expect_true(all(!is.na(times)))
expect_gte(min(times$Milliseconds), 0)
expect_gte(min(times$SD), 0)
expect_gt(min(times$Count), 0)
