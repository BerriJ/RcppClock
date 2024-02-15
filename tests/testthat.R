Sys.setenv("OMP_THREAD_LIMIT" = 2)

library(testthat)
library(rcppclock)

test_check("rcppclock")
