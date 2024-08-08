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