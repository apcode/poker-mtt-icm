cc_library(
    name = "tournament",
    hdrs = [ "tournament.h", "structure.h" ],
    srcs = [ "tournament.cpp", "structure.cpp", ]
)

cc_binary(
    name = "mtt_icm",
    srcs = ["mtt_icm.cpp"],
    deps = [
        ":tournament",
        "//third_party/gflags",
    ],
)
