test_that("Check wether the print method of the rcpp timer scales correctly", {
  fibonacci(n = rep(1:2, 10))

  expect_s3_class(times, "rcpptimer")
  expect_s3_class(times, "data.frame")

  # Set all timings to 0.5 Microseconds
  times$Microseconds <- c(0.5, 0.5, 0.5)

  # We expect that the print method scales the timings to nanoseconds
  expect_output(p_out <- print(times))

  testthat::expect_contains(colnames(p_out), "Nanoseconds")
  expect_equal(p_out$Nanoseconds, c(500, 500, 500))

  # Spread timings between nanoseconds and seconds
  times$Microseconds <- c(2e+9, 1e-2, 1)

  # We do not expect scaling here
  expect_output(p_out <- print(times))
  testthat::expect_contains(colnames(p_out), "Microseconds")
  expect_equal(p_out$Microseconds, c(2e+9, 1e-2, 1))

  # Set all timings to 5 Milliseconds
  times$Microseconds <- c(5e+3, 5e+3, 5e+3)

  # We expect that the print method scales the timings to milliseconds
  expect_output(p_out <- print(times))
  testthat::expect_contains(colnames(p_out), "Milliseconds")
  expect_equal(p_out$Milliseconds, c(5, 5, 5))

  # Set all timings to 5 Seconds
  times$Microseconds <- c(5e+6, 5e+6, 5e+6)

  # We expect that the print method scales the timings to seconds
  expect_output(p_out <- print(times))
  testthat::expect_contains(colnames(p_out), "Seconds")
  expect_equal(p_out$Seconds, c(5, 5, 5))

  # Set all timings to 5 Minutes
  times$Microseconds <- c(5 * 60 * 1e+6, 5 * 60 * 1e+6, 5 * 60 * 1e+6)

  # We expect that the print method scales the timings to minutes
  expect_output(p_out <- print(times))
  testthat::expect_contains(colnames(p_out), "Minutes")
  expect_equal(p_out$Minutes, c(5, 5, 5))

  # Set all timings to 5 Hours
  times$Microseconds <- c(
    5 * 60 * 60 * 1e+6, 5 * 60 * 60 * 1e+6, 5 * 60 * 60 * 1e+6
  )

  # We expect that the print method scales the timings to hours
  expect_output(p_out <- print(times))
  testthat::expect_contains(colnames(p_out), "Hours")
  expect_equal(p_out$Hours, c(5, 5, 5))

  # Set all timings to 5 Days
  times$Microseconds <- c(
    5 * 24 * 60 * 60 * 1e+6, 5 * 24 * 60 * 60 * 1e+6, 5 * 24 * 60 * 60 * 1e+6
  )

  # We expect that the print method scales the timings to hours
  expect_output(p_out <- print(times))
  testthat::expect_contains(colnames(p_out), "Hours")
  expect_equal(p_out$Hours, c(120, 120, 120))

  # Check if setting the scale argument to FALSE works

  # Downscaling:

  # Set all timings to 0.5 Microseconds
  times$Microseconds <- c(0.5, 0.5, 0.5)

  # We expect that the print method does not scale if scale = FALSE
  expect_output(p_out <- print(times, scale = FALSE))

  testthat::expect_contains(colnames(p_out), "Microseconds")
  expect_equal(p_out$Microseconds, c(0.5, 0.5, 0.5))

  # Set all timings to 5 Milliseconds
  times$Microseconds <- c(5e+3, 5e+3, 5e+3)

  # We expect that the print method does not scale if scale = FALSE
  expect_output(p_out <- print(times, FALSE))
  testthat::expect_contains(colnames(p_out), "Microseconds")
  expect_equal(p_out$Microseconds, c(5e+3, 5e+3, 5e+3))

  expect_no_condition(out <- test_misc(tic = FALSE, toc = FALSE, scoped_timer = FALSE))

  expect_warning(
    print(out),
    "This object does not contain any timings yet."
  )
})
