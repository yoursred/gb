#include "cpu/cpu.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>
#include <sys/stat.h>

#define MS chrono::duration_cast<std::chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count()

using namespace std;
int main(void) {
    struct stat result;
    CPU gb = CPU();
    
    // Loading the program into memory
    if (stat("pattern.bin", &result)) {
        printf("pattern.bin not found, comitting dead!\n");
        exit(-1);
    }
    ifstream rom;
    rom.open("pattern.bin", ios::in | ios::binary);
    rom.read((char*) gb.mem, result.st_size);
    rom.close();
    

    cout << "Loaded file successfully, size = " << result.st_size << endl;

    // Running the program
    uint64_t time_start = MS;
    while (*gb.R.pc != 0xFFFF) {
        gb.step();
        gb.R.print_regs();
        // fflush(stdout);
    }
    uint64_t time_end = MS;
    uint64_t duration = time_end - time_start;

    printf("\nTime: %lluns\n", duration);
    printf("Cycles: %lu\n", gb.cycles);
    printf("Instructions: %lu\n", gb.instructions);
    printf("Frequency: %.02lfMHz\n", (gb.cycles * 1000.0F) / duration);

    ofstream memory ("memory.bin", ios::binary);
    memory.write((char*) gb.mem, 0x10000);
    memory.close();

}