//
//  registers.cpp
//  cpusim
//
//  Created by flutterdro on 10.05.2023.
//

#include "registers.h"

Registers::Registers()
:   a(0), bc(0), de(0), hl(0), flags(FlagRegister::Reset) {}

FlagRegister operator|(FlagRegister lhs, FlagRegister rhs) {
    return static_cast<FlagRegister>(
        static_cast<std::underlying_type_t<FlagRegister>>(lhs) |
        static_cast<std::underlying_type_t<FlagRegister>>(rhs)
    );
}

std::underlying_type_t<FlagRegister> operator&(FlagRegister lhs, FlagRegister rhs) {
    return  static_cast<std::underlying_type_t<FlagRegister>>(lhs) &
            static_cast<std::underlying_type_t<FlagRegister>>(rhs);
}

FlagRegister operator^(FlagRegister lhs, FlagRegister rhs) {
    return static_cast<FlagRegister>(
        static_cast<std::underlying_type_t<FlagRegister>>(lhs) ^
        static_cast<std::underlying_type_t<FlagRegister>>(rhs)
    );
}

FlagRegister operator|=(FlagRegister& lhs, FlagRegister rhs) {
    return lhs = static_cast<FlagRegister>(
        static_cast<std::underlying_type_t<FlagRegister>>(lhs) |
        static_cast<std::underlying_type_t<FlagRegister>>(rhs)
    );
}

FlagRegister operator&=(FlagRegister& lhs, FlagRegister rhs) {
    return lhs = static_cast<FlagRegister>(
        static_cast<std::underlying_type_t<FlagRegister>>(lhs) &
        static_cast<std::underlying_type_t<FlagRegister>>(rhs)
    );
}

FlagRegister operator^=(FlagRegister& lhs, FlagRegister rhs) {
    return lhs = static_cast<FlagRegister>(
        static_cast<std::underlying_type_t<FlagRegister>>(lhs) ^
        static_cast<std::underlying_type_t<FlagRegister>>(rhs)
    );
}

FlagRegister operator~(FlagRegister lhs) {
    return static_cast<FlagRegister>(
        ~static_cast<std::underlying_type_t<FlagRegister>>(lhs)
    );
}
