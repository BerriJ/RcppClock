# Start with a fresh global environment
rm(
  list = ls(envir = as.environment(".GlobalEnv")),
  envir = as.environment(".GlobalEnv")
)
# Test if the stop method returns a dataframe with the results
# And autoreturn disabled
expect_no_warning(test_return(autoreturn = FALSE))
expect_no_error(test_return(autoreturn = FALSE))
expect_visible(test_return(autoreturn = FALSE))
results <- test_return(autoreturn = FALSE)
expect_s3_class(results, "data.frame")
# Check if times it not present
expect_identical(ls(as.environment(".GlobalEnv")), character(0))

# Test if the stop method and autoreturn work in conjunction
expect_no_warning(test_return(autoreturn = TRUE))
expect_no_error(test_return(autoreturn = TRUE))
expect_visible(test_return(autoreturn = TRUE))
results <- test_return(autoreturn = TRUE)
expect_s3_class(results, "data.frame")
results_global <- as.environment(".GlobalEnv")$times
expect_s3_class(results_global, "data.frame")
expect_identical(results, results_global)
