#include<iostream>
#include<stdio.h>

#include "cpu/cpu.h"
#include "cpu/insset.h"


CPU::CPU() {
    CPU::R = CPU::Registers();
    instructions = 0;
    // CPU::registers.mem = 

    // Registers();
}

byte* CPU::mem_at(word* address) {
    return &mem[*address];
}

byte* CPU::mem_at(word address) {
    return &mem[address];
}

byte CPU::fetch_instruction() {
    CPU::opcode = mem[*R.pc];
    byte length;
    if (opcode != 0xCB){
        length = get_length(CPU::opcode);
        CPU::current_cycles = get_cycles(CPU::opcode);
    } else {
        length = get_length_prefixed(CPU::opcode); // Wholly redundant, it's always =2
        CPU::current_cycles = get_cycles_prefixed(CPU::opcode);
    }
    // printf("opc=0x%02X, length=%d\n", CPU::opcode, length);
    switch (length) { // maybe we don't really need this?
        case 3:
            working_word = mem[*R.pc + 1];
            working_word |= (word) mem[*R.pc + 2] << 8;
            break;
        case 2:
            working_byte = mem[*R.pc + 1];
            break;
    }
    // printf("wb = 0x%02X, ww = 0x%04X\n", working_byte, working_word);
    return length;
}

void CPU::step() {
    new_pc += fetch_instruction();
    switch (opcode) {
        case (0x00): break;
        case (0xCB): decode_prefixed(); break;
        default: decode();
    }
    instructions  += 1;
    cycles += current_cycles;
    *R.pc = new_pc;
}