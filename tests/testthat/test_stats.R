test_that("Summary statistics are calculated correctly", {
  foo <- test_stats(100, 10)

  means <- as.numeric(tapply(foo[[1]], foo[[2]], mean))
  sds <- as.numeric(tapply(foo[[1]], foo[[2]], sd))
  mins <- as.numeric(tapply(foo[[1]], foo[[2]], min))
  maxs <- as.numeric(tapply(foo[[1]], foo[[2]], max))

  # Unfortunately we cannot expect identical here, due to precision issues ...
  # Therefore, we added a small tolerance

  expect_equal(
    times$Microseconds,
    as.numeric(round(means) * 1e-3),
    tolerance = 2e-5
  )

  expect_equal(
    times$SD,
    as.numeric(round(sds) * 1e-3),
    tolerance = 2e-5
  )

  expect_identical(
    times$Max,
    as.numeric(maxs) * 1e-3
  )

  expect_identical(
    times$Min,
    as.numeric(mins) * 1e-3
  )
})
