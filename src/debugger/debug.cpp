#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

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
    auto t0 = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> dt;
    std::chrono::duration<double, std::micro> frame = std::chrono::microseconds(16742);

    // std::ofstream out("doctor_log.txt");
    // std::stringstream doctor_log;
    // size_t lines = 0;

    // while (state != DBG_END && window.isOpen()) {
    while (state != DBG_END) {
        if (state == DBG_RUNNING) {

            for (auto &&b : breakpoints) {
                if (b.match(cpu)) {
                    state = DBG_PAUSED;
                    output << std::endl << "Paused execution @ " << COUT_HEX_WORD_DS(b.str()) << std::endl;
                    output << print_instruction(cpu);
                }
            }
            if (state == DBG_RUNNING) {
                if (ppu.LY == 0 && ppu.x == 0)
                    t0 = std::chrono::high_resolution_clock::now();
                // if (lines < 47932) {
                //     doctor_log << cpu.log();
                //     lines++;
                // }
                cpu.step();
                for (i = 0; i < cpu.current_cycles; i++) {
                    if (ppu.LY == 0 && ppu.ticks == 0)
                        t0 = std::chrono::high_resolution_clock::now();
                    ppu.tick();
                    if (ppu.LY == 152 && ppu.ticks == 455) {
                        t1 = std::chrono::high_resolution_clock::now();
                        dt = t1 - t0;
                        std::this_thread::sleep_for(frame - dt);
                    }
                }
            }
        }
    }

    // out << doctor_log.rdbuf();
    // out.close();
}

void Debugger::render_thread() {
    sf::Transform scale;
    scale.scale(4, 4);
    window.setActive(true);
    while (window.isOpen()) {
        if (state == DBG_END) {
            window.close();
        }
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        texture.update(ppu.buffer);
        sf::Sprite sprite(texture);
        window.draw(sprite, scale);
        window.display();
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

    window.create(
        sf::VideoMode(640, 576), 
        "boygame debooger - version " VERSION, 
        sf::Style::Close || sf::Style::Titlebar
        );
    texture.create(160, 144);
    window.setFramerateLimit(60);
    window.setActive(false);


    std::thread render(&Debugger::render_thread, this);
    std::thread main_thread(&Debugger::debug_thread, this);

    

    while (state != DBG_END) {
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
    
    main_thread.join();
    render.join();

    // texture.copyToImage().saveToFile("fb.png");
    // std::ofstream fbout("fb.bin", std::ios::out | std::ios::binary);
    // fbout.write((char*) ppu.buffer, 160 * 144 * 4);
}
