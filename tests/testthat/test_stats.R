test_that("Summary statistics are calculated correctly", {
  out <- test_stats(100, 10)

  means <- as.numeric(tapply(out[[1]], out[[2]], mean))
  sds <- as.numeric(tapply(out[[1]], out[[2]], sd))
  mins <- as.numeric(tapply(out[[1]], out[[2]], min))
  maxs <- as.numeric(tapply(out[[1]], out[[2]], max))

  # Unfortunately we cannot expect identical here, due to precision issues ...
  # Therefore, we added a small tolerance
  # C++ results are rounded to the nearest integer (to even in halfway cases)

  expect_equal(
    abs(times$Microseconds - as.numeric(round(means) * 1e-3)) < 2e-3,
    rep(TRUE, length(times$Microseconds))
  )

  expect_equal(
    abs(times$SD - as.numeric(round(sds) * 1e-3)) < 2e-3,
    rep(TRUE, length(times$SD))
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
