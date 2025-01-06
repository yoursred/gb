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
    state = DBG_RUNNING;

}

void Debugger::debug_thread() {
    auto t0 = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> dt;
    std::chrono::duration<double, std::micro> frame = std::chrono::microseconds(16742);
   
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
                if (ppu.LY == 0 && ppu.x == 0) {
                    t0 = std::chrono::high_resolution_clock::now();
                // if (lines < 47932) {
                //     if (cpu.tcycles == 0)
                //         doctor_log << cpu.log();
                //     lines++;
                // }
                // if (ppu.LY == 0 && ppu.ticks == 0)
                //     t0 = std::chrono::high_resolution_clock::now();
                cpu.tick();
                ppu.tick();
                // if (cpu.log_lines == 0)
                //     state = DBG_END;
                if (ppu.LY == 152 && ppu.ticks == 455) {
                    t1 = std::chrono::high_resolution_clock::now();
                    dt = t1 - t0;
                    // std::this_thread::sleep_for(frame - dt);
                    // TODO: add speedup
                }
                // for (i = 0; i < cpu.current_tcycles; i++) {
                //     }
                // }
            }
        }
    }
}

void Debugger::event_thread() {

    // out << doctor_log.rdbuf();
    // out.close();
}

void Debugger::render_thread() {
    sf::Transform scale;

    sf::Font font("meslolgs.ttf");
    sf::Text debug_text(font);
    std::stringstream objs;

    sf::RectangleShape green({160 * 4, 144 * 4});
    green.setFillColor(sf::Color::Green);

    bool show_overlay = false;
    
    debug_text.setFont(font);
    debug_text.setCharacterSize(12);
    debug_text.setFillColor(sf::Color::Magenta);
    scale.scale(sf::Vector2f(4, 4));
    if (!window.setActive(true)) {

    }
    while (window.isOpen()) {
        if (state == DBG_END) {
            window.close();
        }
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                state = DBG_END;
            }
            if (event->is<sf::Event::KeyReleased>() &&  event->getIf<sf::Event::KeyReleased>()->code == sf::Keyboard::Key::Space)
                show_overlay = !show_overlay;
            // TODO: Add screenshots
        }

        window.clear();
        objs.str(std::string());
        if (mem.btns.polled && !mem.btns.delivered) {
            mem.btns.update(sf::Keyboard::isKeyPressed);
        }

        objs << mem.btns.str() << std::endl;

        texture.update(ppu.render_buffer);
        sf::Sprite sprite(texture);
        window.draw(green);
        window.draw(sprite, scale);
        int j = 5;
        if (show_overlay) {
            for (int i = 0; i < 40; i ++) {
                // TODO: pack this madness into a function of the struct `obj`
                sf::RectangleShape obj_rect0({32, 32});
                sf::RectangleShape obj_rect1({32, 32});
                
                obj_rect0.setFillColor(sf::Color::Transparent);
                obj_rect1.setFillColor(sf::Color::Transparent);
                obj_rect0.setPosition(
                    sf::Vector2f(ppu.OAM_T[i].x - 8, ppu.OAM_T[i].y - 16) * 4.f
                );
                obj_rect1.setPosition(
                    sf::Vector2f(ppu.OAM_T[i].x - 8, ppu.OAM_T[i].y - 8) * 4.f
                );
                obj_rect0.setOutlineColor(sf::Color::Green);
                obj_rect1.setOutlineColor(sf::Color::Yellow);
                obj_rect0.setOutlineThickness(1);
                obj_rect1.setOutlineThickness(1);

                if (ppu.OAM_T[i].visible()) {
                    objs << ppu.OAM_T[i].str();
                    objs << std::endl;

                    sf::Vertex line[2] = {
                        {{ppu.OAM_T[i].str().size() * 7.f, j * 15.f}, sf::Color::Magenta, {0, 0}},
                        {{(ppu.OAM_T[i].x - 8) * 4.f, (ppu.OAM_T[i].y - 16) * 4.f}, sf::Color::Blue, {0, 0}}
                    };

                    obj_rect0.setOutlineColor(sf::Color::Blue);
                    obj_rect1.setOutlineColor(sf::Color::Cyan);
                    window.draw(line, 2, sf::PrimitiveType::Lines);
                    j++;
                }

                window.draw(obj_rect0);
                // window.draw(obj_rect1); // TODO: Keep track of LCDC.obj16_enable to properly outline them
            }
            debug_text.setString(objs.str());
            window.draw(debug_text);
        }
        window.display();
    }
}

void Debugger::debug_main(int argc, const char* argv[]) {

    std::string cmd;
    std::vector<std::string> args;


    output << "boygame deboogger, version " << VERSION << std::endl;
    output << "Built with " << __VERSION__ << std::endl;

    output << "Size of struct obj: " << sizeof(obj) << std::endl;
    output << "Size of struct dmg_plt: " << sizeof(dmg_plt) << std::endl;
    output << "Size of struct lcdc: " << sizeof(lcdc) << std::endl;
    output << "Size of struct lcd_stat: " << sizeof(lcd_stat) << std::endl;


    window.create(
        sf::VideoMode(sf::Vector2u(160 * 4, 144 * 4)), 
        "boygame debooger - version " VERSION, 
        sf::Style::Close | sf::Style::Titlebar
        );
    bool resized = texture.resize(sf::Vector2u(160, 144));
    if (!resized) {
        window.setSize(sf::Vector2u(160, 144));
    }
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);
    window.setPosition({80, 80});
    bool handoff = window.setActive(false);

    if (handoff) {
        std::thread render(&Debugger::render_thread, this);
        std::thread main_thread(&Debugger::debug_thread, this);

        while (state != DBG_END) {
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
    } else {
        std::cerr << "Could not handoff window to render thread, aborting..." << std::endl;
        std::exit(-1);
    }
}
