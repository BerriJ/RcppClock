fibonacci(n = rep(15:20, 10))
times
expect_contains(ls(as.environment(".GlobalEnv")), "times")
expect_true(all(!is.na(times)))
expect_gte(min(times$Microseconds), 0)
expect_gte(min(times$SD), 0)
expect_gt(min(times$Count), 0)

times_sc <- times

fibonacci_omp(n = rep(15:20, 10))
times

expect_contains(ls(as.environment(".GlobalEnv")), "times")
expect_true(all(!is.na(times)))
expect_gte(min(times$Microseconds), 0)
expect_gte(min(times$SD), 0)
expect_gt(min(times$Count), 0)
