test_that("Check reset method", {
  testthat::expect_no_error(times <- test_reset())

  expect_equal(row.names(times[[1]]), c("t1", "t2"))
  expect_equal(row.names(times[[2]]), c("t3"))
})
