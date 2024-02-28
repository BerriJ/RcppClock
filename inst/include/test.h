#ifndef test_h
#define test_h

#include <chrono>
#include <thread>
#include <Rcpp.h>
#include <rcppclock.h>

class Test
{
public:
    // Initializer list is optional but recommended as it sets the initial size
    // of the map. If not set, map size will be determined by whatever is left
    // in memory.
    Rcpp::Clock clock;

    Test() // constructor
    {
        clock.tick("lifetime");
        Rcpp::Rcout << "Constructor called" << std::endl;
    };

    int wait(int reps = 10, int ms = 1)
    {
        clock.tick("wait body");
        for (int i = 0; i < reps; ++i)
        {
            clock.tick("wait loop");
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
            clock.tock("wait loop");
        }
        clock.tock("wait body");
        return (0);
    }

    int wait_omp(int reps = 10, int ms = 1)
    {
        clock.tick("waitomp body");
#pragma omp parallel for
        for (int i = 0; i < reps; ++i)
        {
            clock.tick("waitomp loop");
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
            clock.tock("waitomp loop");
        }
        clock.tock("waitomp body");
        return (0);
    }

    int get_times()
    {
        clock.stop();
        return (0);
    }

    int reset()
    {
        clock.reset();
        return (0);
    }

    ~Test() // Destructor
    {
        clock.tock("lifetime");
        Rcpp::Rcout << "Destructor called" << std::endl;
    };
};

#endif