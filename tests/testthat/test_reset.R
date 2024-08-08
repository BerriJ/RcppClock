# Test if the reset method works
Rcpp::cppFunction('
List test_reset()
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
  DataFrame results1 = timer.stop();
  timer.reset();
  timer.tic("t3");
  List L = List::create();
  L.push_back(results1);
  timer.toc("t3");
  DataFrame results2 = timer.stop();
  L.push_back(results2);
  return(L);
}',
  depends = "rcpptimer"
)

testthat::expect_no_error(times <- test_reset())

expect_equal(row.names(times[[1]]), c("t1", "t2"))
expect_equal(row.names(times[[2]]), c("t3"))
