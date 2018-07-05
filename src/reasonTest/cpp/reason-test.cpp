#include <gtest/gtest.h>

#include "reason.hpp"

TEST(ReasonTest, AddReason) {
    echelon::Reason r;
    r.addReason("a reason");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
