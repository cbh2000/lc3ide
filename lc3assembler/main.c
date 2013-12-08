#include <stdio.h>
#include <string.h>

// A carry over from Objective-C: Why type an extra letter and in all caps?
#define nil NULL

typedef unsigned short word;
typedef unsigned char byte;

typedef enum {
    ParsedTypeInstruction         = 0,
    ParsedTypeLabelDeclaration    = 1,
    ParsedTypeLabelUsage_9bit     = 2,
    ParsedTypeLabelUsage_6bit     = 3
} ParsedType;

typedef struct {
    byte type; // ParsedType
    word data; // The instruction, label string pos, etc.
} ParsedData;

// 65536 >= (instructions + labels + label usages)
// In other words, label declarations and label usages count towards
// the output limit.  Instructions do too, of course.
static ParsedData parsed_output[65536];
static int parsed_output_pos = 0;

static void labelFoundAt(word string_pos) {
    printf("out[%i] = label declared at %i (x%04X)\n", parsed_output_pos, string_pos, string_pos);

    // Save
    ParsedData parsed;
    parsed.type = ParsedTypeLabelDeclaration;
    parsed.data = string_pos;
    parsed_output[parsed_output_pos] = parsed;

    // Next
    parsed_output_pos += 1;
    printf("next()\n");
}

static void labelUsageFoundAt(word string_pos, word bit_size) {
    printf("out[%i] = label used at %i (x%04X)\n",
           parsed_output_pos,
           string_pos,
           string_pos);

    // We are inserting the label usage just before the instruction--move it
    ParsedData existing_instruction = parsed_output[parsed_output_pos];
    parsed_output[parsed_output_pos + 1] = existing_instruction;

    // Offset size
    ParsedData label_usage;
    if (bit_size == 6) label_usage.type = ParsedTypeLabelUsage_6bit;
    else if (bit_size == 9) label_usage.type = ParsedTypeLabelUsage_9bit;
    else printf("ERROR: Unsupported bit_size of %i", bit_size);

    // Data
    label_usage.data = string_pos;

    // Save
    parsed_output[parsed_output_pos] = label_usage;

    // Next
    parsed_output_pos += 1;
    printf("inserted()\n");
}

static void instructionOR(word data) {
    // Get
    ParsedData parsed = parsed_output[parsed_output_pos];

    // bitwise OR
    parsed.data |= data;

    // Put back
    parsed_output[parsed_output_pos] = parsed;
}

static void nextInstruction() {
    printf("out[%i] = instruction x%04X\n", parsed_output_pos, parsed_output[parsed_output_pos].data);

    // Next
    ++parsed_output_pos;
    printf("next()\n");

    // Initialize the new output's data (make it zero)
    ParsedData parsed = parsed_output[parsed_output_pos];
    parsed.type = ParsedTypeInstruction;
    parsed.data = 0;

    // Save
    parsed_output[parsed_output_pos] = parsed;
}

word strings_are_same(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

typedef enum {
    InstructionTypeUnknown = 0,
    InstructionTypeSetProgramCounter,
    InstructionTypeAND,
    InstructionTypeADD,
    InstructionTypeTRAP,
    InstructionTypeSTR,
    InstructionTypeNOT,
    InstructionTypeLD
} InstructionType;

typedef struct {
    word mem_location;
} SetProgramCounterInstruction;

char keywordIsValueLiteral(const char *keyword) {
    return keyword[0] == '#';
}

word keywordLiteralValue(const char *keyword) {
    word value = 0;
    int pos = 1;
    char c = keyword[pos];
    char is_negative = (c == '-');
    if (is_negative) {
        ++pos;
        c = keyword[pos];
    }

    while (c) {
        value *= 10;
        value += c - '0';

        ++pos;
        c = keyword[pos];
    }

    if (is_negative) {
        value = -value;
    }

    return value;
}

word keywordRegisterToWord(const char *reg_string) {
    return reg_string[1] - '0';
}

word keywordHexToWord(const char *keyword) {
    int len = strlen(keyword);
    int keyword_pos = len - 1;
    int output_pos = 0;
    word output = 0x0000;
    word c = keyword[keyword_pos];
    while (keyword_pos > 0) {
        if (c >= '0' && c <= '9') {
            c -= '0';
        } else if (c >= 'A' && c <= 'F') {
            c -= 'A';
            c += 10;
        } else if (c >= 'a' && c <= 'f') {
            c -= 'a';
            c += 10;
        }

        c <<= 4 * (output_pos);
        output |= c;
        ++output_pos;

        --keyword_pos;
        c = keyword[keyword_pos];
    }

    return output;
}

char addKeyword_ProgramCounterInstruction(const char *keyword, int index) {
    if (index == 1) {
        word offset = keywordHexToWord(keyword);
        instructionOR(offset);
    }

    return 1 - index;
}

char addKeyword_AND(const char *keyword, int index) {
    if (index == 0) {
        instructionOR(0b0101 << 12);
    } else if (index == 1) {
        word reg_num = keywordRegisterToWord(keyword);
        instructionOR(reg_num << 9);
    } else if (index == 2) {
        word reg_num = keywordRegisterToWord(keyword);
        instructionOR(reg_num << 6);
    } else if (index == 3) {
        if (keywordIsValueLiteral(keyword)) {
            word literalVal = keywordLiteralValue(keyword);

            // SRET (Sign RETract) to 5 bits
            literalVal &= 0x001F;

            // Set the "I am a literal bit"
            literalVal |= 0x0020;
        } else {
            word reg_num = keywordRegisterToWord(keyword);
            instructionOR(reg_num);
        }
    }

    return 3 - index;
}

char addKeyword_ADD(const char *keyword, int index) {
    if (index == 0) {
        instructionOR(0b0001 << 12);
    } else if (index == 1) {
        word reg_num = keywordRegisterToWord(keyword);
        instructionOR(reg_num << 9);
    } else if (index == 2) {
        word reg_num = keywordRegisterToWord(keyword);
        instructionOR(reg_num << 6);
    } else if (index == 3) {
        if (keywordIsValueLiteral(keyword)) {
            word literalVal = keywordLiteralValue(keyword);

            // If the literal is negative, SRET (sign retract)
            literalVal &= 0x001F;

            // Set the "I am a literal" bit
            literalVal |= 0x0020;

            instructionOR(literalVal);
        } else {
            word reg_num = keywordRegisterToWord(keyword);
            instructionOR(reg_num);
        }
    }

    return 3 - index;
}

char addKeyword_TRAP(const char *keyword, int index) {
    if (index == 0) {
        instructionOR(0b1111 << 12);
    } else if (index == 1) {
        word hexVal = keywordHexToWord(keyword);
        instructionOR(hexVal);
    }

    return 1 - index;
}

char addKeyword_STR(const char *keyword, int index) {
    if (index == 0) {
        instructionOR(0b0111 << 12);
    } else if (index == 1) {
        word source_reg_num = keywordRegisterToWord(keyword);
        instructionOR(source_reg_num << 9);
    } else if (index == 2) {
        word source_reg_num = keywordRegisterToWord(keyword);
        instructionOR(source_reg_num << 9);
    } else if (index == 3) {
        word dest_reg_num = keywordRegisterToWord(keyword);
        instructionOR(dest_reg_num << 6);
    } else if (index == 4) {
        word offset_six = keywordLiteralValue(keyword);
        offset_six &= 0b111111;
        instructionOR(offset_six);
    }

    return 3 - index;
}

char addKeyword_NOT(const char *keyword, int index) {
    if (index == 0) {
        instructionOR(0b1001 << 12);
        instructionOR(0b111111); // Filler
    } else if (index == 1) {
        word reg_num = keywordRegisterToWord(keyword);
        instructionOR(reg_num << 9);
    } else if (index == 2) {
        word reg_num = keywordRegisterToWord(keyword);
        instructionOR(reg_num << 6);
    }

    return 2 - index;
}

char addKeyword_LD(const char *keyword, int index, int keyword_string_pos) {
    if (index == 0) {
        printf("Adding x%04X to %i\n", 0x1000, parsed_output_pos);
        instructionOR(0b0010 << 12);
    } else if (index == 1) {
        word reg_num = keywordRegisterToWord(keyword);
        instructionOR(reg_num << 9);
    } else if (index == 2) {
        labelUsageFoundAt(keyword_string_pos, 9);
    }

    return 2 - index;
}

// Returns 0 when finished (time to parse a new instruction)
char addKeywordWithIndex_toInstructionWithType(const char *keyword,
                                               int keyword_index,
                                               int keyword_string_pos,
                                               InstructionType type) {
    switch (type) {
    case InstructionTypeUnknown:
        return 0;
    case InstructionTypeSetProgramCounter:
        return addKeyword_ProgramCounterInstruction(keyword, keyword_index);
    case InstructionTypeAND:
        return addKeyword_AND(keyword, keyword_index);
    case InstructionTypeADD:
        return addKeyword_ADD(keyword, keyword_index);
    case InstructionTypeTRAP:
        return addKeyword_TRAP(keyword, keyword_index);
    case InstructionTypeSTR:
        return addKeyword_STR(keyword, keyword_index);
    case InstructionTypeNOT:
        return addKeyword_NOT(keyword, keyword_index);
    case InstructionTypeLD:
        return addKeyword_LD(keyword, keyword_index, keyword_string_pos);
    }

    return 0;
}

InstructionType instructionTypeForKeyword(const char *keyword) {
    if (!keyword) {
        return InstructionTypeUnknown;
    }

    if (strings_are_same(keyword, ".ORIG"))
        return InstructionTypeSetProgramCounter;
    else if (strings_are_same(keyword, "AND"))
        return InstructionTypeAND;
    else if (strings_are_same(keyword, "ADD"))
        return InstructionTypeADD;
    else if (strings_are_same(keyword, "TRAP"))
        return InstructionTypeTRAP;
    else if (strings_are_same(keyword, "STR"))
        return InstructionTypeSTR;
    else if (strings_are_same(keyword, "NOT"))
        return InstructionTypeNOT;
    else if (strings_are_same(keyword, "LD"))
        return InstructionTypeLD;

    return InstructionTypeUnknown;
}

char should_ignore(char c, char *is_comment) {
    if (*is_comment) {
        *is_comment = (c != '\n');
        return 1;
    } else if (c == ';') {
        *is_comment = 1;
        return 1;
    }

    return c == ' ' || c == '\n' || c == '\t' || c == ',' || c == ';';
}

const char *firstKeywordInString(const char *string, int *ending_offset) {
    int string_pos = 0;
    int buf_pos = 0;
    char is_comment = 0;

    static char buf[255];
    char c = string[string_pos];

    while (c) {
        if (!should_ignore(c, &is_comment)) {
            buf[buf_pos] = c;
            ++buf_pos;
        } else if (buf_pos > 0) {
            buf[buf_pos] = 0;
            break;
        }

        ++string_pos;
        c = string[string_pos];
    }

    if (buf_pos <= 0) { return nil; }

    if (ending_offset) { *ending_offset = string_pos; }
    buf[buf_pos] = 0;
    buf[buf_pos + 1] = 0;

    return buf;
}

// Returns size of output, in 2-byte LC-3 words
void parse(const char *string) {

    // Position in string
    int string_pos = 0;

    // Keyword index for each instruction (in `AND R1,R2,R3`, `R1` is index 0).
    char keyword_index = 0;

    // The current instruction.
    word instruction;
    InstructionType type = InstructionTypeUnknown;

    const char *label = nil;

    // Parse
    int string_length = strlen(string);
    while (string_pos < string_length) {

        // Extract a keyword
        int keyword_offset; // distance between string_pos and start of keyword
        int keyword_string_pos = string_pos; // keyword position in string
        const char *keyword;
        keyword = firstKeywordInString(string + string_pos, &keyword_offset);
        string_pos += keyword_offset;
        printf("Keyword `%s`\n", keyword);

        // Now, let's do something with the keyword

        if (!type) {
            // This keyword is a instruction, label, etc.
            type = instructionTypeForKeyword(keyword);
            printf("Type: %i\n", (int)type);
            keyword_index = 0; // Reset index
        }

        if (type == InstructionTypeUnknown && keyword) {
            // is a label
            label = keyword;
            labelFoundAt(keyword_string_pos);
        } else {
            printf("For %i at %i\n", (int)type, (int)keyword_index);

            // 'more' will be non-zero when the instruction wants more keywords
            // when 'more' is zero, we are done
            char more;
            more = addKeywordWithIndex_toInstructionWithType(keyword,
                                                             keyword_index,
                                                             keyword_string_pos,
                                                             type);
            printf("%i more\n", (int)more);

            if (more <= 0 && keyword) {
                nextInstruction();

                // Next loop will check type
                type = InstructionTypeUnknown;
            }
        }

        ++keyword_index;
        printf("\n");
    }
}

const char *binary_desc(word w) {
    static char buf[17];
    buf[16] = '\0';
    word filter = 1;
    word index = 15;
    while ((filter << 1) > filter) {
        filter <<= 1;
        index -= 1;
    }

    for (word filter = 1, index = 15;
         (filter << 1) > filter;
         filter <<= 1, --index)
    {
        if (w & filter) {
            buf[index] = '1';
        } else {
            buf[index] = '0';
        }
    }
    return buf;
}

int main(void) {
    const char *input =
            ".ORIG x3000;\n"
            "START ADD R1 R2\nR3\n"
            "LD R7,START\n";

    /**************************************************************************/
    //                            First Pass
    //              (create instructions and locate labels)
    /**************************************************************************/
    parse(input);
    printf("<< Parsed >>\n\n");
    for (int i = 0; i < parsed_output_pos; ++i) {
        ParsedData parsed = parsed_output[i];
        printf("%i: 0x%04X (%s)\n", parsed.type, parsed.data, binary_desc(parsed.data));
    }
    printf("\n<< Parsed >>\n");

    /**************************************************************************/
    //                            Second Pass
    //                    (resolve label positions)
    /**************************************************************************/
    printf("\n\n<< Resolved >>\n\n");
    for (int label_index = 0; label_index < parsed_output_pos; ++label_index) {
        ParsedData parsed_label = parsed_output[label_index];
        if (parsed_label.type == ParsedTypeLabelDeclaration) {
            const char *label = firstKeywordInString(input + parsed_label.data, nil);
            printf("Found label decl. `%s` at out[%i]\n", label, label_index);

            for (int usage_index = 0; usage_index < parsed_output_pos; ++usage_index) {
                ParsedData possible_usage = parsed_output[usage_index];
                if (possible_usage.type == ParsedTypeLabelUsage_9bit) {
                    const char *usage = firstKeywordInString(input + possible_usage.data, nil);
                    if (strings_are_same(label, usage)) {
                        printf("\tFound label usage for `%s` at out[%i]\n", usage, usage_index);
                        printf("\tPCOffset%i: %i\n", possible_usage.type == ParsedTypeLabelUsage_9bit ? 9 : 6, label_index - usage_index - 1);
                    }
                }
            }
        }
    }
    printf("\n<< Resolved >>\n");

    return 0;
}
