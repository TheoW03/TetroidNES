#include <gtest/gtest.h>
#include "../emulator/Computer.h"
// Demonstrate some basic assertions.
// TEST(HelloTest, BasicAssertions)
// {
//     // Expect two strings not to be equal.
//     EXPECT_STRNE("hello", "world");
//     CPU cpu = test_init("../games/Controllers.nes");
//     EXPECT_EQ(cpu.error_code, EXIT_SUCCESS);

//     // Expect equality.
//     EXPECT_EQ(7 * 6, 42);
// }
TEST(HelloTest, TestLDA)
{
    // EXPECT_STRNE("hello", "world");
    CPU cpu = test_init("../src/unitTests/TestRoms/LDATest.nes");
    EXPECT_EQ(cpu.error_code, EXIT_SUCCESS);
    EXPECT_EQ(cpu.A_Reg, 5);
    EXPECT_EQ(cpu.X_Reg, 5);
    EXPECT_EQ(cpu.Y_Reg, 5);
    // Expect equality
    // EXPECT_EQ(7 * 6, 42);
}
TEST(HelloTest, TestJMP){
    CPU cpu = test_init("../src/unitTests/TestRoms/JMPTest.nes");
    EXPECT_EQ(cpu.error_code, EXIT_SUCCESS);
    EXPECT_EQ(cpu.A_Reg, 1);
    EXPECT_EQ(cpu.X_Reg, 10);
    EXPECT_EQ(cpu.Y_Reg, 1);
}