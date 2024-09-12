#include <Emulator/Computer.h>
#include <fstream>

void cpu_to_log(std::string path)
{
    std::ofstream outFile(path);
}
void program_success(CPU cpu)
{
    printCPU_stats(cpu);
    std::cout << "" << std::endl;
    std::cout << "\033[92mProgram has successfully ended" << std::endl;
    std::cout << "exit code 0 \033[0m" << std::endl;
    // exit(EXIT_SUCCESS);
}
void program_failure(std::string reason, CPU cpu, int exit_code)
{
    std::cout << reason << std::endl;
    printCPU_stats(cpu);
    printf("\n");
    std::cout << "\033[91mProgram unsuccessfully ended" << std::endl;
    std::cout << "exit code 1\033[0m" << std::endl;
    // exit(EXIT_FAILURE);
}