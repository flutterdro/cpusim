#ifndef registers_h
#define registers_h


class Registers {
public:
    //oh forgive me lord
    union {
        uint16_t bc;
        struct {
            uint8_t b;
            uint8_t c;
        };
    };
    union {
        uint16_t de;
        struct {
            uint8_t d;
            uint8_t e;
        };
    };
    union {
        uint16_t hl;
        struct {
            uint8_t h;
            uint8_t l;
        };
    };
};

#endif

