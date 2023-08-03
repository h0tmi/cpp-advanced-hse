# Create library.
cc_library(
    name = "chromo",
    srcs = ["time.cpp"],
    hdrs = ["time.hpp"],
    visibility = [
        # Make `chromo` visible for all targets from `clocks`.
        "//clocks:__pkg__",

        # Now `chromo` is visible from tests also.
        "//tests:__pkg__",
    ],
)
