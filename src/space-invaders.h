//
//  space-invaders.hpp
//  cpusim
//
//  Created by flutterdro on 19.05.2023.
//

#ifndef space_invaders_h
#define space_invaders_h


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <chrono>
#include <thread>


#include "cpu/cpu.h"
#include "io/screen.h"
#include "io/keyboard.h"
#include "cpu/disassemble.h"

class ShiftRegister {
public:
    void setAndShift(uint8_t acc);
    uint8_t getReg();
    void setShift(uint8_t shift);
private:
    int shift_offset;
    union {
        uint16_t both;
        struct {
            uint8_t high;
            uint8_t low;
        };
    };
};

class Game {
public:
    Game();
    ~Game();
    std::vector<uint8_t>::iterator vramB;
    std::vector<uint8_t>::iterator vramE;
    void run();
    uint8_t getPort1();
    uint8_t getPort2();
private:
    CPU m_intel8080;
    ShiftRegister m_cust_reg;
    void setupGl();
    void processInput();
    void update();
    void draw();
    void execution();
    template<class RandomIterator>
    void transpose(RandomIterator first, RandomIterator last, int m);
    std::string loadShaderSrc(const char* filepath);
    std::mutex m;
private:
    Screen screen;
    GLuint shader;
    GLuint texture;
    GLuint VAO;
    uint8_t read_port1;
    uint8_t read_port2;
    std::array<uint8_t, 224*256> m_image;
};


#endif /* space_invaders_hpp */
