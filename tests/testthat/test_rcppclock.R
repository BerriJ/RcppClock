test_that("Testing Rcpp::Clock", {
  fibonacci(n = c(25, 30, 35), reps = 10)

  expect_false(is.null(clock))
})
