fibonacci(n = (1:3) * 10, reps = 10)

expect_false(is.null(times))
expect_true(all(!is.na(times)))
expect_true(all(times$Milliseconds >= 0))
expect_true(all(times$SD >= 0))
expect_true(all(times$Count > 0))
