# This test files checks wether the print method of the rcpp timer scales the timings correctly and if the scale argument works.

fibonacci(n = rep(1:2, 10))

expect_s3_class(times, "rcpptimer")
expect_s3_class(times, "data.frame")

# Set all timings to 0.5 Microseconds
times$Microseconds <- c(0.5, 0.5, 0.5)

# We expect that the print method scales the timings to nanoseconds
p_out <- print(times)

testthat::expect_contains(colnames(p_out), "Nanoseconds")
expect_true(all(p_out$Nanoseconds == 500))

# Spread timings between nanoseconds and seconds
times$Microseconds <- c(2e+9, 1e-2, 1)

# We do not expect scaling here
p_out <- print(times)
testthat::expect_contains(colnames(p_out), "Microseconds")
expect_true(all(p_out$Nanoseconds == c(2e+9, 1e-2, 1)))

# Set all timings to 5 Milliseconds
times$Microseconds <- c(5e+3, 5e+3, 5e+3)

# We expect that the print method scales the timings to milliseconds
p_out <- print(times)
testthat::expect_contains(colnames(p_out), "Milliseconds")
expect_true(all(p_out$Milliseconds == 5))

# Set all timings to 5 Seconds
times$Microseconds <- c(5e+6, 5e+6, 5e+6)

# We expect that the print method scales the timings to seconds
p_out <- print(times)
testthat::expect_contains(colnames(p_out), "Seconds")
expect_true(all(p_out$Seconds == 5))

# Set all timings to 5 Minutes
times$Microseconds <- c(5 * 60 * 1e+6, 5 * 60 * 1e+6, 5 * 60 * 1e+6)

# We expect that the print method scales the timings to minutes
p_out <- print(times)
testthat::expect_contains(colnames(p_out), "Minutes")
expect_true(all(p_out$Minutes == 5))

# Set all timings to 5 Hours
times$Microseconds <- c(5 * 60 * 60 * 1e+6, 5 * 60 * 60 * 1e+6, 5 * 60 * 60 * 1e+6)

# We expect that the print method scales the timings to hours
p_out <- print(times)
testthat::expect_contains(colnames(p_out), "Hours")
expect_true(all(p_out$Hours == 5))

# Set all timings to 5 Days
times$Microseconds <- c(5 * 24 * 60 * 60 * 1e+6, 5 * 24 * 60 * 60 * 1e+6, 5 * 24 * 60 * 60 * 1e+6)

# We expect that the print method scales the timings to hours
p_out <- print(times)
testthat::expect_contains(colnames(p_out), "Hours")
expect_true(all(p_out$Hours == 120))

# Check if setting the scale argument to FALSE works

# Downscaling:

# Set all timings to 0.5 Microseconds
times$Microseconds <- c(0.5, 0.5, 0.5)

# We expect that the print method does not scale if scale = FALSE
p_out <- print(times, scale = FALSE)

testthat::expect_contains(colnames(p_out), "Microseconds")
expect_true(all(p_out$Nanoseconds == c(0.5, 0.5, 0.5)))

# Set all timings to 5 Milliseconds
times$Microseconds <- c(5e+3, 5e+3, 5e+3)

# We expect that the print method does not scale if scale = FALSE
p_out <- print(times, FALSE)
testthat::expect_contains(colnames(p_out), "Microseconds")
expect_true(all(p_out$Milliseconds == c(5e+3, 5e+3, 5e+3)))
