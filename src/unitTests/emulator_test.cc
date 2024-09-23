#include <gtest/gtest.h>
#include <Emulator/Computer.h>

/**
 * @brief  tests the rom.PRG.
 * it must be 0x8000 - 0xffff
 *
 */
TEST(HelloTest, TestRom)
{
    std::vector<uint8_t> raw = file_tobyte_vector("./src/unitTests/TestRoms/LDATest.nes");
    Rom rom = load_rom(raw);
    EXPECT_EQ(rom.PRG.size() - 1, 0x7fff);
}
/**
 * @brief Make sure LDA works
 *
 */
TEST(HelloTest, TestLDA)
{
    CPU cpu = init("./src/unitTests/TestRoms/LDATest.nes");
    EXPECT_EQ(cpu.error_code, EXIT_SUCCESS);
    EXPECT_EQ(cpu.A_Reg, 5);
    EXPECT_EQ(cpu.X_Reg, 5);
    EXPECT_EQ(cpu.Y_Reg, 5);
    // Expect equality
    // EXPECT_EQ(7 * 6, 42);
}
/**
 * @brief tests jumping
 *
 */
TEST(HelloTest, TestJMP)
{
    CPU cpu = init("./src/unitTests/TestRoms/JMPTest.nes");
    EXPECT_EQ(cpu.error_code, EXIT_SUCCESS);
    printf(" \n");
    printCPU_stats(cpu);
    printf(" \n");

    EXPECT_EQ(cpu.X_Reg, 10);
    EXPECT_EQ(cpu.Y_Reg, 1);
    EXPECT_NEAR(cpu.bus.get_PC(), 0x8018, 7);
}
/**
 * @brief Tests bitwise
 *
 */
TEST(HelloTest, TestASL)
{
    CPU cpu = init("./src/unitTests/TestRoms/TestAsl.nes");
    EXPECT_EQ(cpu.error_code, EXIT_SUCCESS);
    printf(" \n");
    printCPU_stats(cpu);
    printf(" \n");

    EXPECT_EQ(cpu.A_Reg, 1);
}