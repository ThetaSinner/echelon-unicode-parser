# Intended to run from the project root
# See https://github.com/google/googletest/blob/master/googletest/README.md

export GTEST_DIR=/development/googletest-release-1.8.0/googletest

g++-7.3.0 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
ar -rv libgtest.a gtest-all.o


