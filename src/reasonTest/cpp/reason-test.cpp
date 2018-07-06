#include <gtest/gtest.h>

#include <sstream>

#include "reason.hpp"

using namespace echelon::diagnostics;

TEST(ReasonTest, NoReason) {
    Reason r;

    std::stringstream ss;
    r.replay(ss);

    ASSERT_EQ("Unknown reason", ss.str());
}

TEST(ReasonTest, AddReason) {
    Reason r;
    r.addReason("testing a single failure");

    std::stringstream ss;
    r.replay(ss);

    ASSERT_EQ("Reason: testing a single failure", ss.str());
}

TEST(ReasonTest, AddMultipleReasons) {
    Reason r;
    r.addReason("testing a low level failure");
    r.addReason("testing a higher level failure");

    std::stringstream ss;
    r.replay(ss);

    ASSERT_EQ("Reason: testing a higher level failure\n        testing a low level failure", ss.str());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
