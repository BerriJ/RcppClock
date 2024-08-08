# Test if results are updated after calling stop
Rcpp::cppFunction('
List update()
{
  Rcpp::Timer timer;
  timer.autoreturn = false;
  List L = List::create();
  {
    Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
    timer.tic("t2");
    std::string s1;
    s1.reserve(1048576);
    timer.toc("t2");
    DataFrame results1 = timer.stop();
    L.push_back(results1);
    timer.tic("t2");
    std::string s2;
    s2.reserve(1048576);
    timer.toc("t2");
  }
  DataFrame results2 = timer.stop();
  L.push_back(results2);
  return(L);
}',
  depends = "rcpptimer"
)

expect_warning(times <- update())

t1 <- times[[1]]
t2 <- times[[2]]

expect_equal(row.names(t1), "t2")
expect_equal(row.names(t2), c("t1", "t2"))
expect_equal(t2[row.names(t2) == "t2", "Count"], 2)
