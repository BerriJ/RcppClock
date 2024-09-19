test_that("Warnings", {
  # We expect that a warning is thrown
  expect_warning(test_misc(toc = FALSE, autoreturn = FALSE),
    'Timer "t2" not stopped yet.',
    ignore.case = FALSE
  )

  # We expect, despite that warning, times is still returned
  expect_contains(ls(as.environment(".GlobalEnv")), "times")
  rm(times, envir = as.environment(".GlobalEnv"))

  # Test if a warning is thrown when verbose = true (default) and tic is missing

  # We expect, despite that warning, times is still returned
  expect_warning(test_misc(tic = FALSE),
    'Timer "t2" not started yet.',
    ignore.case = FALSE
  )

  # We expect, despite that warning, times is still returned
  expect_contains(ls(as.environment(".GlobalEnv")), "times")
  rm(times, envir = as.environment(".GlobalEnv"))

  # Test if no warning is thrown when verbose = false and toc is missing

  expect_no_warning(test_misc(toc = FALSE, verbose = FALSE))

  # We expect, despite that warning, times is still returned
  expect_contains(ls(as.environment(".GlobalEnv")), "times")
  rm(times, envir = as.environment(".GlobalEnv"))

  # Test no a warning is thrown when verbose = false and tic is missing
  expect_no_warning(test_misc(tic = FALSE, verbose = FALSE))

  # We expect, despite that warning, times is still returned
  expect_contains(ls(as.environment(".GlobalEnv")), "times")
  rm(times, envir = as.environment(".GlobalEnv"))

  # Test if warnings also work in parallelized code
  expect_warning(test_stats(5, 5, missing_tic = TRUE),
    'Timer "summary_2" not started yet.',
    ignore.case = FALSE
  )
  rm(times, envir = as.environment(".GlobalEnv"))

  expect_warning(test_stats(5, 5, missing_toc = TRUE),
    'Timer "summary_2" not stopped yet.',
    ignore.case = FALSE
  )
  rm(times, envir = as.environment(".GlobalEnv"))

  expect_warning(test_misc(extra_toc = TRUE),
    'Timer "t2" stopped more than once. ',
    ignore.case = FALSE
  )
  rm(times, envir = as.environment(".GlobalEnv"))
})
