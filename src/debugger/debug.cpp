#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

#include "debugger/debug.h"
#include "include.h"
#include "cpu/cpu.h"
#include "ppu/ppu.h"


Debugger::Debugger(Memory& mem, CPU& cpu, PPU& ppu):
    mem(mem), cpu(cpu), ppu(ppu)
{
    state = DBG_STOPPED;

}

void Debugger::debug_thread() {
    while (state != DBG_END && window.isOpen()) {
    // while (state != DBG_END) {
        if (state == DBG_RUNNING) {
            for (auto &&b : breakpoints) {
                if (b.match(cpu)) {
                    state = DBG_PAUSED;
                    output << std::endl << "Paused execution @ " << COUT_HEX_WORD_DS(b.str()) << std::endl;
                    output << print_instruction(cpu);
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

void Debugger::render_thread() {
    window.setActive(true);
    while (window.isOpen() && state != DBG_END) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        texture.update(ppu.buffer);
        sf::Sprite sprite(texture);
        window.draw(sprite);
        window.display();
    }
    if (window.isOpen()) {
        window.close();
    }
}

void Debugger::debug_main(int argc, const char* argv[]) {
    // byte state = DBG_STOPPED;
    // std::vector<Breakpoint> b;

    std::string cmd;
    std::vector<std::string> args;
    // std::stringstream cmd_ss;
    // std::string token;
    // word x, y;
    // int i;


    output << "boygame deboogger, version " << VERSION << std::endl;
    output << "Built with " << __VERSION__ << std::endl; 

    // std::thread cpu_thread(cpu_only_thread, std::ref(cpu), std::ref(state), std::ref(b));
    // std::thread cpu_thread(ppu_thread, std::ref(cpu), std::ref(ppu), std::ref(state), std::ref(b));

    window.create(sf::VideoMode(160, 144), "boygame debooger - version ");
    texture.create(160, 144);
    window.setActive(false);


    std::thread render(&Debugger::render_thread, this);
    std::thread main_thread(&Debugger::debug_thread, this);

    

    while (state != DBG_END && window.isOpen()) {
    // while (state != DBG_END) {
        output << "> ";
        std::getline(std::cin, cmd);
        if (cmd.size()) {
            args = split_command(cmd);
            cmd = args.front();
            args.erase(args.begin());

            if (!str(cmd, commands)) {
                output << "Command `" << cmd << "` not found." << std::endl;
            } else if(simple_commands_ptr.count(cmd) && args.empty()) {
                // TODO: eliminate this evil and use `std::function` instead
                (this->*(simple_commands_ptr.at(cmd)))(); // black magic
            } else if (commands_ptr.count(cmd)) {
                (this->*(commands_ptr.at(cmd)))(args); // black magic
            } else {
                output << "Too many arguments for command `" << cmd << "`." << std::endl;
            }
        }
    }
    
    render.join();
    main_thread.join();

    texture.copyToImage().saveToFile("fb.png");
    std::ofstream fbout("fb.bin", std::ios::out | std::ios::binary);
    fbout.write((char*) ppu.buffer, 160 * 144 * 4);

    // while (state != DBG_END) { // main loop
    //     // TODO: Implement similar command-syntax generation to yoursred/showdown-analysis-bot
    //     std::cout << "> ";
    //     std::getline(std::cin, cmd);
    //     cmd_ss = std::stringstream(cmd);
    //     cmd_ss >> token;

    //     if (token == "break" || token == "b") { 
    //         // Set breakpoint
    //         if (cmd_ss >> std::hex >> x) {
    //             if (cmd_ss >> token) {
    //                 if (token == "pc" || token == "af" || token == "bc" || token == "de" || token == "hl" ||
    //                     token == "a" || token == "b" || token == "c" || token == "d" || token == "e" || 
    //                     token == "h" || token == "l") 
    //                 {
    //                     b.push_back(Breakpoint(token, x));
    //                 } else {
    //                     std::cout << "Syntax error in break! Unknown register `" << token << "`" << std::endl;
    //                 }
    //             } else {
    //                 b.push_back(Breakpoint("pc", x));
    //             }
    //         }
    //         else {
    //             std::cout << "Syntax error in break! Invalid hex value `" << COUT_HEX_WORD(x) << "`" << std::endl;
    //         }
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "clear") { // Delete all breakpoints
    //         b.clear();
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "continue" || token == "c") { // Resume execution
    //         state = DBG_RUNNING;
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "cycles") { // Print out cycles ex
    //         std::cout << std::dec << cpu.cycles << std::endl;
    //     }
    //     //--------------------------------------------------------------
    //     if (token == "delete" || token == "d") { // Delete breakpoint
    //         if (cmd_ss >> x) {
    //             if (x < b.size()) {
    //                 b.erase(b.begin() + x);
    //             }
    //             else {
    //                 std::cout << "Index out of range!" << std::endl;
    //             }
    //         }
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "dump") { // Memory dump
    //         if ((cmd_ss >> std::hex >> x) && (cmd_ss >> std::hex >> y)) {
    //             dumphex(mem, x, y);
    //         }
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "exit") { // Exit program
    //         state = DBG_END;
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "flags") { // Print out cpu flags
    //         cpu.R.print_flags();
    //     } 
    //     //--------------------------------------------------------------
    //     if (token == "ic") { // Print out instruction count
    //         std::cout << std::dec << cpu.instructions << std::endl;
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "list" || token == "lb" || token == "l") { // List breakpoints
    //         for (word i = 0; i < b.size(); i++) {
    //             int b_count = b.size();
    //             int p_width = 0;
    //             for (;b_count > 0; b_count /= 10) p_width++;
    //             std::cout << std::string(p_width, ' ') <<  i << " - "  << b[i].str() << std::endl;
    //         }
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "opc") { // Print out current instruction
    //         std::cout << print_instruction(cpu) << std::endl;
    //     } else
    //     if (token == "pause") {
    //         state = DBG_PAUSED;
    //         std::cout << "Paused execution" << std::endl;
    //     } else
    //     if (token == "ppu") {
    //         ppu.print_ppu_registers();
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "registers") { // Print out cpu registers
    //         cpu.R.print_regs();
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "state") {
    //         switch (state) {
    //             case DBG_STOPPED: std::cout << "DBG_STOPPED" << std::endl; break;
    //             case DBG_RUNNING: std::cout << "DBG_RUNNING" << std::endl; break;
    //             case DBG_PAUSED: std::cout << "DBG_PAUSED" << std::endl; break;
    //             case DBG_END: std::cout << "DBG_END" << std::endl; break;
    //         }
    //     } else
    //     //--------------------------------------------------------------
    //     if (token == "step" || token == "s") { // Advance one instruction
    //         if (state == DBG_PAUSED) {
    //             cpu.step();
    //             for (i = 0; i < cpu.current_cycles; i++) {
    //                 ppu.tick();
    //             }
    //             print_instruction(cpu);
    //         }
    //     }
    // }

}
