#ifndef CHAMELEON
#define CHAMELEON

#include <Rcpp.h>

namespace chameleon
{
    inline void warn(std::string msg)
    {
        Rcpp::warning(msg.c_str());
    }
};

#endif