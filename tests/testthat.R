Sys.setenv("OMP_THREAD_LIMIT" = 2)

library(testthat)
library(rcpptimer)

test_check("rcpptimer")
