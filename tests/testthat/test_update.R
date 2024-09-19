test_that("Updating works", {
  expect_warning(times <- test_update())

  t1 <- times[[1]]
  t2 <- times[[2]]

  expect_gt(t1$Microseconds, 0)
  expect_equal(t1$Microseconds, t1$Min)
  expect_equal(t1$Microseconds, t1$Max)
  expect_equal(t1$SD, 0)
  expect_equal(row.names(t1), "t2")

  expect_equal(row.names(t2), c("t1", "t2", "t3"))
  expect_equal(t2[row.names(t2) == "t2", "Count"], 2)

  expect_gte(t2["t2", "Microseconds"], t2["t2", "Min"])
  expect_lte(t2["t2", "Microseconds"], t2["t2", "Max"])
})
