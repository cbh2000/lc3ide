#ifndef DEF_H
#define DEF_H

#define LC3_WORD_SIZE (16) // bits
#define LC3_WORD_TYPE unsigned short int // 16-bit max

#define OPERATOR_TYPE unsigned char // 8-bit max

enum Operator {
    AND = 0x7 // 0101
};

const char* op_and = "AND";

enum Register {
    R0 = 0x0, // 000
    R1 = 0x1, // 001
    R2 = 0x2, // 010
    R3 = 0x3, // 011
    R4 = 0x4, // 100
    R5 = 0x5, // 101
    R6 = 0x6, // 110
    R7 = 0x7  // 111
};

const char* reg_R0 = "R0";
const char* reg_R1 = "R1";
const char* reg_R2 = "R2";
const char* reg_R3 = "R3";
const char* reg_R4 = "R4";
const char* reg_R5 = "R5";
const char* reg_R6 = "R6";
const char* reg_R7 = "R7";

#endif // DEF_H
