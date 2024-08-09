#include <rcpptimer.h>
using namespace Rcpp;

//[[Rcpp::export]]
double test_default(double &x, double &y)
{
  Rcpp::Timer timer;
  Rcpp::Timer::ScopedTimer scoped_timer(timer);
  timer.tic();
  double z = x + y;
  timer.toc();
  return (z);
}

//[[Rcpp::export]]
List test_update()
{
  Rcpp::Timer timer;
  timer.autoreturn = false;
  List L = List::create();
  double z;
  {
    Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
    timer.tic("t2");
    std::string s1;
    double x = rnorm(1, 1)[0];
    double y = rnorm(1, 1)[0];
    timer.toc("t2");
    DataFrame results1 = timer.stop();
    L.push_back(results1);
    timer.tic("t2");
    z = x + y;
    timer.toc("t2");
  }
  DataFrame results2 = timer.stop();
  L.push_back(results2);
  L.push_back(z);
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

//[[Rcpp::export]]
DataFrame test_return(const bool autoreturn = true)
{
  Rcpp::Timer timer;
  {
    Rcpp::Timer::ScopedTimer scoped_timer(timer, "t1");
    timer.autoreturn = autoreturn;
    timer.tic("t2");
    std::string s;
    s.reserve(1048576);
    timer.toc("t2");
  }
  DataFrame results = timer.stop();
  return (results);
}