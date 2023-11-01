#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "debugger/debug.h"
#include "include.h"
#include "cpu/cpu.h"
#include "ppu/ppu.h"



void cpu_only_thread(CPU& cpu, byte& state, std::vector<Breakpoint>& breakpoints) {
    while (state != DBG_END) {
        if (state == DBG_RUNNING) {
            for (auto &&b : breakpoints) {
                if (b.match(cpu)) {
                    state = DBG_PAUSED;
                    std::cout << std::endl << "Paused execution @ " << COUT_HEX_WORD_DS(b.str()) << std::endl;
                    print_instruction(cpu);
                }
            }
            if (state == DBG_RUNNING)
                cpu.step();
        }
    }
}

void ppu_thread(CPU& cpu, PPU& ppu, byte& state, std::vector<Breakpoint>& breakpoints) {
    int i;
    while (state != DBG_END) {
        if (state == DBG_RUNNING) {
            for (auto &&b : breakpoints) {
                if (b.match(cpu)) {
                    state = DBG_PAUSED;
                    std::stringstream out;
                    out << std::endl << "Paused execution @ PC=" << b.str() << std::endl;
                    out << print_instruction(cpu);
                    std::cout << out.str();
                }
            }
            if (state == DBG_RUNNING) {
                cpu.step();
                for (i = 0; i < cpu.current_cycles; i++) {
                   ppu.tick();
                }
            }
        }
    }
}

void dbg_main(Memory& mem, CPU& cpu, PPU& ppu) {
    byte state = DBG_STOPPED;
    std::vector<Breakpoint> b;

    std::string cmd;
    std::stringstream cmd_ss;
    std::string token;
    word x, y;
    int i;


    std::cout << "boygame deboogger, version ___" << std::endl;

    // std::thread cpu_thread(cpu_only_thread, std::ref(cpu), std::ref(state), std::ref(b));
    std::thread cpu_thread(ppu_thread, std::ref(cpu), std::ref(ppu), std::ref(state), std::ref(b));


    while (state != DBG_END) { // main loop
        // TODO: Implement similar command-syntax generation to yoursred/showdown-analysis-bot
        std::cout << "> ";
        std::getline(std::cin, cmd);
        cmd_ss = std::stringstream(cmd);
        cmd_ss >> token;

        if (token == "break" || token == "b") { 
            // Set breakpoint
            // TODO: break on not just PC
            if (cmd_ss >> std::hex >> x) {
                if (cmd_ss >> token) {
                    if (token == "pc" || token == "af" || token == "bc" || token == "de" || token == "hl" ||
                        token == "a" || token == "b" || token == "c" || token == "d" || token == "e" || 
                        token == "h" || token == "l") 
                    {
                        b.push_back(Breakpoint(token, x));
                    } else {
                        std::cout << "Syntax error in break! Unknown register `" << token << "`" << std::endl;
                    }
                } else {
                    b.push_back(Breakpoint("pc", x));
                }
            }
            else {
                std::cout << "Syntax error in break! Invalid hex value `" << COUT_HEX_WORD(x) << "`" << std::endl;
            }
        } else
        //--------------------------------------------------------------
        if (token == "clear") { // Delete all breakpoints
            b.clear();
        } else
        //--------------------------------------------------------------
        if (token == "continue" || token == "c") { // Resume execution
            state = DBG_RUNNING;
        } else
        //--------------------------------------------------------------
        if (token == "cycles") { // Print out cycles ex
            std::cout << std::dec << cpu.cycles << std::endl;
        }
        //--------------------------------------------------------------
        if (token == "delete" || token == "d") { // Delete breakpoint
            if (cmd_ss >> x) {
                if (x < b.size()) {
                    b.erase(b.begin() + x);
                }
                else {
                    std::cout << "Index out of range!" << std::endl;
                }
            }
        } else
        //--------------------------------------------------------------
        if (token == "dump") { // Memory dump
            if ((cmd_ss >> std::hex >> x) && (cmd_ss >> std::hex >> y)) {
                dumphex(mem, x, y);
            }
        } else
        //--------------------------------------------------------------
        if (token == "exit") { // Exit program
            state = DBG_END;
        } else
        //--------------------------------------------------------------
        if (token == "flags") { // Print out cpu flags
            cpu.R.print_flags();
        } 
        //--------------------------------------------------------------
        if (token == "ic") { // Print out instruction count
            std::cout << std::dec << cpu.instructions << std::endl;
        } else
        //--------------------------------------------------------------
        if (token == "list" || token == "lb" || token == "l") { // List breakpoints
            for (word i = 0; i < b.size(); i++) {
                std::cout << i << " - @ PC=" << COUT_HEX_WORD_DS(b[i].value16) << std::endl;
            }
        } else
        //--------------------------------------------------------------
        if (token == "opc") { // Print out current instruction
            print_instruction(cpu);
        } else
        if (token == "ppu") {
            ppu.print_ppu_registers();
        } else
        //--------------------------------------------------------------
        if (token == "registers") { // Print out cpu registers
            cpu.R.print_regs();
        } else
        //--------------------------------------------------------------
        if (token == "state") {
            switch (state) {
                case DBG_STOPPED: std::cout << "DBG_STOPPED" << std::endl; break;
                case DBG_RUNNING: std::cout << "DBG_RUNNING" << std::endl; break;
                case DBG_PAUSED: std::cout << "DBG_PAUSED" << std::endl; break;
                case DBG_END: std::cout << "DBG_END" << std::endl; break;
            }
        } else
        //--------------------------------------------------------------
        if (token == "step" || token == "s") { // Advance one instruction
            if (state == DBG_PAUSED) {
                cpu.step();
                for (i = 0; i < cpu.current_cycles; i++) {
                    ppu.tick();
                }
                print_instruction(cpu);
            }
        }
    }

}
