#include <rcpptimer.h>
using namespace Rcpp;

//[[Rcpp::export]]
List test_update()
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
  return (L);
}

//[[Rcpp::export]]
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
  return (L);
}

//[[Rcpp::export]]
void test_missings(const bool tic = true,
                   const bool toc = true,
                   const bool verbose = true)
{
  Rcpp::Timer timer(verbose);
  Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
  if (tic)
    timer.tic("t2");
  std::string s;
  s.reserve(1048576);
  if (toc)
    timer.toc("t2");
}