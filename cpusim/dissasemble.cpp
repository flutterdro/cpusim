//
//  dissasemble.cpp
//  cpusim
//
//  Created by flutterdro on 07.05.2023.
//

#include "dissasemble.h"

Dissasembler::Dissasembler() {
    loadInstructions();
}

void Dissasembler::loadInstructions() {
    if (std::ifstream load{"assets/instructionset.in"}) {
        for (int i = 0x0; i <= 0xF; i++) {
            for (int j = 0x0; j <= 0xF; j++) {
                std::string instruction;
                std::string buff;
                int c = 0;
                while (buff != "|") {
                    if (c == 0) {
                        instruction += fmt::format("{:<7}", buff);
                        c++;
                    } else {
                        instruction += fmt::format("{:<8}", buff);
                    }
                    load >> buff;
                    if (buff == "a16") {
                        adressInstructions.insert(i*0x10 + j);
                        load >> buff;
                    }
                    if (buff == "d16") {
                        data2bInstructions.insert(i*0x10 + j);
                        load >> buff;
                    }
                    if (buff == "d8") {
                        data1bInstructions.insert(i*0x10 + j);
                        load >> buff;
                    }
                }
                instructions[i*0x10 + j] = instruction;
            }
        }
    }
}

void Dissasembler::dissasemble(const char *filepath) {
    std::vector<uint8_t> first;
    if (std::ifstream fin{filepath, std::ios::hex }) {
        uint8_t buff;
        while (fin >> std::noskipws >> buff) {
            first.push_back(buff);
        }
    }
    for (int i = 0; i <= first.size(); i++) {
        if (adressInstructions.count(first[i])) {
            hexf += fmt::format("{3:#06x}:    {0}{2:#04x}{1:02x}\n", instructions[first[i]], first[i + 1], first[i + 2], i);
            i += 2;
        } else if (data1bInstructions.count(first[i])) {
            hexf += fmt::format("{2:#06x}:    {0}{1:#04x}\n", instructions[first[i]], first[i + 1], i);
            i += 1;
        }else if (data2bInstructions.count(first[i])) {
            hexf += fmt::format("{3:#06x}:    {0}{2:#04x}{1:02x}\n", instructions[first[i]], first[i + 1], first[i + 2], i);
            i += 2;
        } else {
            hexf += fmt::format("{1:#06x}:    {0}\n", instructions[first[i]], i);
        }
    }
}

std::string Dissasembler::getResult() {
    return hexf;
}
