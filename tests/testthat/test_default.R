test_that("Check defaults", {
  test_default()

  # We expect, despite that warning, times is still returned
  expect_identical(ls(as.environment(".GlobalEnv")), "times")

  expect_identical(row.names(times), c("scoped", "tictoc"))
  expect_true(all(!is.na(times)))
  expect_gte(min(times$Microseconds), 0)
  expect_gte(min(times$SD), 0)
  expect_gt(min(times$Count), 0)
})
