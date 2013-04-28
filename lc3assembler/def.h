#ifndef DEF_H
#define DEF_H

#include <string.h>

#define LC3_WORD_BITS (16) // bits
#define LC3_WORD_TYPE unsigned short int // 16-bit max

#define LC3_OPERATOR_BITS (4)
#define LC3_OPERATOR_TYPE unsigned char // 8-bit max

enum Operator {
    AND = 0x7 // 0101
};

const char* op_and = "AND";

#define LC3_REGISTER_BITS (3)
#define LC3_REGISTER_TYPE unsigned char // 8-bit max

enum Register {
    NotRegister = -0x1, // Negative
    R0 = 0x0, // 000
    R1 = 0x1, // 001
    R2 = 0x2, // 010
    R3 = 0x3, // 011
    R4 = 0x4, // 100
    R5 = 0x5, // 101
    R6 = 0x6, // 110
    R7 = 0x7  // 111
};

const char* register_R0 = "R0";
const char* register_R1 = "R1";
const char* register_R2 = "R2";
const char* register_R3 = "R3";
const char* register_R4 = "R4";
const char* register_R5 = "R5";
const char* register_R6 = "R6";
const char* register_R7 = "R7";

// This identifier function is VERY picky (e.g., no extra spaces allowed)!
Register idRegister(const char* input) {
    if (strlen(input) < 1)
        return NotRegister;

    // To quote Lumetta, "Not fast, but who cares?"  ;-)
    if (strcasecmp(input, register_R0)) {
        return R0;
    } else if (strcasecmp(input, register_R1)) {
        return R1;
    } else if (strcasecmp(input, register_R2)) {
        return R2;
    } else if (strcasecmp(input, register_R3)) {
        return R3;
    } else if (strcasecmp(input, register_R4)) {
        return R4;
    } else if (strcasecmp(input, register_R5)) {
        return R5;
    } else if (strcasecmp(input, register_R6)) {
        return R6;
    } else if (strcasecmp(input, register_R7)) {
        return R7;
    }

    return NotRegister;
}

enum Preprocessor {
    NotPreprocessor,
    BLKW,
    END,
    FILL,
    ORIG,
    STRINGZ
};

const char* preprocessor_blkw = ".BLKW";
const char* preprocessor_end = ".END";
const char* preprocessor_fill = ".FILL";
const char* preprocessor_orig = ".ORIG";
const char* preprocessor_stringz = ".STRINGZ";

// This identifier function is VERY picky (e.g., no extra spaces allowed)!
Preprocessor idPreprocessor(const char* input) {
    if (strlen(input) < 1)
        return NotPreprocessor;

    if (strcasecmp(input, preprocessor_blkw)) {
        return BLKW;
    } else if (strcasecmp(input, preprocessor_end)) {
        return END;
    } else if (strcasecmp(input, preprocessor_fill)) {
        return FILL;
    } else if (strcasecmp(input, preprocessor_orig)) {
        return ORIG;
    } else if (strcasecmp(input, preprocessor_stringz)) {
        return STRINGZ;
    }

    return NotPreprocessor;
}

enum LabelStatus {
    ValidLabel,
    EmptyLabel,
    IllegalCharacter
};

// This function is VERY picky (e.g., no extra spaces allowed)!
LabelStatus isValidLabel(const char* input) {
    int len = strlen(input);
    if (len < 1)
        return EmptyLabel; // Empty string cannot be a label

    char testBase[2]; // Character and null-terminator
    testBase[1] = '\0';
    const char *allowedFirstCharacters = "abcdefghijklmnopqrstuvwxyz";
    const char *allowedMiddleCharacters = "abcdefghijklmnopqrstuvwxyz_0123456789";

    testBase[0] = input[0];
    if (!strcasestr(allowedFirstCharacters, input[0]))
        return IllegalCharacter; // Illegal first character

    // Start from 2nd character (1st already checked)
    for (char* ch = input + 1; *ch != '\0'; ++ch) {
        testBase[0] = *ch;
        if (!strcasestr(allowedMiddleCharacters, testBase))
            return IllegalCharacter; // Illegal character found
    }

    return ValidLabel;
}

#endif // DEF_H
