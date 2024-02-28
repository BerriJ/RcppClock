fibonacci(n = (1:4) * 10, reps = 10)

expect_false(is.null(times))
expect_true(all(!is.na(times)))
expect_gte(min(times$Milliseconds), 0)
expect_gte(min(times$SD), 0)
expect_gt(min(times$Count), 0)

times_sc <- times

fibonacci_omp(n = (1:4) * 10, reps = 10)

expect_false(is.null(times))
expect_true(all(!is.na(times)))
expect_gte(min(times$Milliseconds), 0)
expect_gte(min(times$SD), 0)
expect_gt(min(times$Count), 0)

times_mc <- times

# We test if there is at least 50% speedup (to test if OMP works)
test_that("Skip on MacOS", {
    skip_on_os("mac")
    expect_gt(times_sc$Milliseconds[1], times_mc$Milliseconds[1] * 1.5)
})

rm(list = ls())

test <- new(Test)

test$wait(reps = 10, ms = 1)

test$get_times()

times

testthat::expect_equal(times$Milliseconds[2], 1, tolerance = 0.1)

test$wait(reps = 10, ms = 1)

test$get_times()

testthat::expect_identical(times$Count[2], 20)

test$wait_omp(reps = 10, ms = 1)

test$get_times()

times

testthat::expect_identical(nrow(times), 4L)

test$reset()

test$get_times()

testthat::expect_identical(nrow(times), 0L)
