//
//  space-invaders.cpp
//  cpusim
//
//  Created by flutterdro on 19.05.2023.
//

#include "space-invaders.h"


Game::Game() {
    m_intel8080.loadMemory("assets/invaders/invaders");
    vramB = m_intel8080.getMemoryIt(0x2400);
    vramE = m_intel8080.getMemoryIt(0x4000);
    m_intel8080.connectOutPort(4, [&](uint8_t a){m_cust_reg.setAndShift(a);});
    m_intel8080.connectOutPort(2, [&](uint8_t a){m_cust_reg.setShift(a);});
    m_intel8080.connectInPort (3, [&](){return m_cust_reg.getReg();});
    m_intel8080.connectInPort (1, [this](){return getPort1();});
    m_intel8080.connectInPort (2, [this](){return getPort2();});
    m_image.fill(0);
    setupGl();
}

Game::~Game() {
    glDeleteProgram(shader);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &VAO);
}

void Game::processInput() {
    read_port1 &= 0;
    read_port1 |= Keyboard::key         (GLFW_KEY_0);
    read_port1 |= Keyboard::key         (GLFW_KEY_ENTER) << 2;
    read_port1 |= Keyboard::key         (GLFW_KEY_EQUAL) << 1;
    read_port1 |= Keyboard::key         (GLFW_KEY_SPACE) << 4;
    read_port1 |= Keyboard::key         (GLFW_KEY_LEFT)  << 5;
    read_port1 |= Keyboard::key         (GLFW_KEY_RIGHT) << 6;
    read_port2 &= 0;
    read_port2 |= Keyboard::keyWentDown (GLFW_KEY_RIGHT_SHIFT) << 4;
    read_port2 |= Keyboard::key         (GLFW_KEY_A)  << 5;
    read_port2 |= Keyboard::key         (GLFW_KEY_W) << 6;
}

void Game::update() {
    for (int i = 0; i < 224; i++) {
        for (int j = 0; j < 32; j++) {
            uint8_t temp = *(vramB + 32*i + j + 1);
            for (int k = 0; k < 8; k++, temp = temp >> 1){
                m_image[i*32*8 + j*8 + k] = 0xFF * (temp & 0x01);
            }
        }
    }
    //God bless you that one dude from stack overflow
    transpose(m_image.begin(), m_image.end(), 256);
}

void Game::draw() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 224, 256, GL_RED, GL_UNSIGNED_BYTE, m_image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    screen.newFrame();
    screen.update();
}

uint8_t Game::getPort1() {
    return read_port1;
}

uint8_t Game::getPort2() {
    return read_port2;
}

void ShiftRegister::setShift(uint8_t shift) {
    shift_offset = shift&0x7;
}

void ShiftRegister::setAndShift(uint8_t acc) {
    high = low;
    low = acc;
}

uint8_t ShiftRegister::getReg() {
    uint16_t ret = (both >> (8 - shift_offset));
    return ret;
}

void Game::execution() {
    while (!screen.shouldClose()) {
        processInput();
        m.lock();
        m_intel8080.executeNextInstruction();
        m.unlock();
        std::this_thread::sleep_for(std::chrono::nanoseconds(500));
    }
}

void Game::run() {
    bool lastd = 0;
    std::thread t1([this](){execution();});
    while (!screen.shouldClose()) {
        m.lock();
        m_intel8080.interupt(0xd7 * lastd + 0xcf * !(lastd));
        m.unlock();
        lastd = !lastd;
        glViewport(0, 0, 4*224, 4*256);
        update();
        draw();
    }
    t1.join();
}

template<class RandomIterator>
void Game::transpose(RandomIterator first, RandomIterator last, int m) {
    const int mn1 = (last - first - 1);
    const int n   = (last - first) / m;
    std::vector<bool> visited(last - first);
    RandomIterator cycle = first;
    while (++cycle != last) {
        if (visited[cycle - first])
            continue;
        int a = cycle - first;
        do  {
            a = a == mn1 ? mn1 : (n * a) % mn1;
            std::swap(*(first + a), *cycle);
            visited[a] = true;
        } while ((first + a) != cycle);
    }
}

void Game::setupGl() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if (!screen.init())
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    screen.setParameters();
    glfwSwapInterval(1); // Enable vsync

    GLuint vs, fs;
    int  success;
    char infoLog[512];
    std::string stringVs = loadShaderSrc("assets/display.vs");
    const char *vsSrc = stringVs.c_str();
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSrc, nullptr);
    glCompileShader(vs);
    glGetShaderiv(vs , GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, nullptr, infoLog);
        std::cout << "failed to compile a shader: " << infoLog << std::endl;
    }
    std::string stringFs = loadShaderSrc("assets/display.fs");
    const char *fsSrc = stringFs.c_str();
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSrc, nullptr);
    glCompileShader(fs);
    glGetShaderiv(fs , GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, nullptr, infoLog);
        std::cout << "failed to compile a shader: " << infoLog << std::endl;
    }
    shader = glCreateProgram();
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    glUseProgram(shader);
    std::array<float, 16> vertices {
         1.0f,  1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,  1.0f
    };
    
    std::array<unsigned int, 6> indices {
        0, 1, 3,
        1, 2, 3
    };
    GLuint VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 224, 256, 0, GL_RED, GL_UNSIGNED_BYTE, m_image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

std::string Game::loadShaderSrc(const char* filepath) {
    std::fstream fin;
    fin.open(filepath);
    std::stringstream sstream;
    if (fin.is_open()) {
        sstream << fin.rdbuf();
        fin.close();
    } else {
        std::cout << "failed to open a file" << std::endl;
    }
    return sstream.str();
}
