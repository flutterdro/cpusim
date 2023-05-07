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
                while (buff != "|") {
                    instruction += buff + " ";
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
        while (fin >> buff) {
            first.push_back(buff);
        }
    }
    for (int i = 0; i < first.size(); i++) {
        if (adressInstructions.count(first[i])) {
            hexf += fmt::format("{}{:#04x} {:#04x}\n", instructions[first[i]], first[i + 1], first[i + 2]);
            i += 2;
        } else if (data1bInstructions.count(first[i])) {
            hexf += fmt::format("{}{:#04x}\n", instructions[first[i]], first[i + 1]);
            i += 1;
        }else if (data2bInstructions.count(first[i])) {
            hexf += fmt::format("{}{:#04x} {:#04x}\n", instructions[first[i]], first[i + 1], first[i + 2]);
            i += 2;
        } else {
            hexf += fmt::format("{}\n", instructions[first[i]]);
        }
    }
}

std::string Dissasembler::getResult() {
    return hexf;
}
