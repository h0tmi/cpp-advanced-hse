# Create test.
cc_test(
    name = "test_chromo",
    srcs = ["test_chromo.cpp"],
    deps = [
        "//chromo:chromo",

        # `@googletest` is another "repository" -- hence @.
        "@googletest//:gtest_main",
    ]
)
