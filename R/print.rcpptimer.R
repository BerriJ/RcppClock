#' Print method for rcpptimer output
#'
#' Prints the times object and scales the timings if appropriate.
#' If all timings are smaller than 1 microsecond, the timings are
#' printed in nanoseconds. If the smallest timing is higher than
#' a Millisecond / Seconds / Minutes / Hours, the timings are printed in
#' the unit of that threshold. This behavior can be disabled by setting
#' scale = FALSE.
#' @param x Object of class rcpptimer
#' @param scale Scale the timings and statistics to a more human readable format
#' @param ...  further arguments are ignored
#' @rdname print.rcpptimer
#' @export
print.rcpptimer <- function(x, scale = TRUE, ...) {
  if (nrow(x) == 0) {
    warning("This object does not contain any timings yet. \nPlace .tic() and .toc() statements in your c++ code to add timings.")
    return(invisible(x))
  }

  if (!scale) {
    print.data.frame(x, row.names = TRUE)
    return(invisible(x))
  }
  cols_to_scale <- c("Microseconds", "SD", "Min", "Max")
  scaling_factors <- c(
    "Nanoseconds" = 1e-3,
    "Microseconds" = 1,
    "Milliseconds" = 1e3,
    "Seconds" = 1e6,
    "Minutes" = 60 * 1e6,
    "Hours" = 60 * 60 * 1e6
  )
  scale_up <- max(which(min(x$Microseconds) > scaling_factors))
  scale_down <- max(which(max(x$Microseconds) > scaling_factors))

  if (scale_down == 1) { # All timings smaller than 1 microsecond
    scale_to <- scale_down
  } else if (scale_up > 1) { # All timings bigger than threshold
    scale_to <- scale_up
  } else {
    print.data.frame(x, row.names = TRUE)
    return(invisible(x))
  }

  scale_to <- scaling_factors[scale_up]
  x[, cols_to_scale] <-
    x[, cols_to_scale] |>
    {
      \(x) round(x / scale_to, 3)
    }()
  names(x)[names(x) == "Microseconds"] <- names(scale_to)
  print.data.frame(x, row.names = TRUE)
  # Remove `rcpptimer` class to avoid problems with printing the output
  class(x) <- c("data.frame")
  return(invisible(x))
}
