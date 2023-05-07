//
//  dissasemble.h
//  cpusim
//
//  Created by flutterdro on 07.05.2023.
//

#ifndef dissasemble_h
#define dissasemble_h

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <fstream>
#include <cinttypes>
#include <fmt/format.h>

class Dissasembler {
public:
    Dissasembler();
    void dissasemble(const char* filepath);
    std::string getResult();
    
private:
    std::string hexf;
    
    std::unordered_set<uint8_t> adressInstructions;
    std::unordered_set<uint8_t> data1bInstructions;
    std::unordered_set<uint8_t> data2bInstructions;
    std::unordered_map<uint8_t, std::string> instructions;
    
    void loadInstructions();
};

#endif /* dissasemble_h */
