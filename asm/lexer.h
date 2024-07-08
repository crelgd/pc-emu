#ifndef EMU_ASM_LEXER_H
#define EMU_ASM_LEXER_H

typedef enum {
    MOV,
    ADD,
    CMP,
    JMP,
    JIZ,
    JINZ,
    JIE,
    OUT,
    R1,
    R2,
    R3,
    UNDEFINED
} TokenType;

typedef struct {
    TokenType CMD; // mov, add, cmp
    TokenType VAL1; // reg, val
    TokenType VAL2; // reg, val
    int value;
} Token;

class Lexer {
public:
    Lexer(const char* line);
    ~Lexer();

    void next_char();
    void skip_space();

    Token cmd_check();

private:
    const char* _line;
    int pos;
    char cur_char;

    const char* keywords[] = {
        "mov",
        "add",
        "cmp",
        "jz",
        "jnz",
        "je",
        "jmp",
        "out",
        // reg
        "r1",
        "r2",
        "r3"
    };

    const size_t keywords_v = sizeof(keywords) / sizeof(keywords[0]);
};

#endif 
