fibonacci(n = rep(10 * (1:4), 10))

expect_false(is.null(times))
expect_true(all(!is.na(times)))
expect_gte(min(times$Milliseconds), 0)
expect_gte(min(times$SD), 0)
expect_gt(min(times$Count), 0)

times_sc <- times

fibonacci_omp(n = rep(10 * (1:4), 10))

expect_false(is.null(times))
expect_true(all(!is.na(times)))
expect_gte(min(times$Milliseconds), 0)
expect_gte(min(times$SD), 0)
expect_gt(min(times$Count), 0)

times_mc <- times

# We test if there is at least 50% speedup (to test if OMP works)
test_that("Skip on MacOS", {
    skip_on_os("mac")
    expect_gt(times_sc$Milliseconds[5], times_mc$Milliseconds[5] * 1.5)
})
