expect_warning(times <- test_update())

t1 <- times[[1]]
t2 <- times[[2]]

expect_equal(row.names(t1), "t2")
expect_equal(row.names(t2), c("t1", "t2"))
expect_equal(t2[row.names(t2) == "t2", "Count"], 2)
