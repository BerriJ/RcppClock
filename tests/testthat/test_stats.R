foo <- test_stats(100, 10)

times

tapply(foo[[1]], foo[[2]], mean)
tapply(foo[[1]], foo[[2]], sd)

times$Microseconds * 1e3 == round(tapply(foo[[1]], foo[[2]], mean))
times$SD * 1e3 == round(tapply(foo[[1]], foo[[2]], sd))
