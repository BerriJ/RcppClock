fibonacci(n = (1:4) * 10, reps = 10)

expect_false(is.null(times))
expect_true(all(!is.na(times)))
expect_true(all(times$Milliseconds >= 0))
expect_true(all(times$SD >= 0))
expect_true(all(times$Count > 0))

times_sc <- times

fibonacci_omp(n = (1:4) * 10, reps = 10)

expect_false(is.null(times))
expect_true(all(!is.na(times)))
expect_true(all(times$Milliseconds >= 0))
expect_true(all(times$SD >= 0))
expect_true(all(times$Count > 0))

times_mc <- times

# We expect at least 90% speedup with OpenMP using 2 cores
expect_true((times_sc$Milliseconds[1] >= times_mc$Milliseconds[1] * 2))
