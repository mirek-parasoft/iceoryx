#!/bin/bash

rm -fr cpptest_results.clog

bazel run @cpptest//:coverage --@cpptest//:target=//iceoryx_hoofs/test:hoofs_moduletests_vector  --@cpptest//:psrc_file=//:cpptestcc-bazel-psrc

bazel-out/k8-fastbuild/bin/iceoryx_hoofs/test/hoofs_moduletests_vector.elf

cpptestcov compute -map .cpptest -clog cpptest_results.clog -out .coverage

cpptestcov index .coverage