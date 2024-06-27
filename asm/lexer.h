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
    R1,
    R2,
    R3,
    UNDEFINED
} TokenType;

typedef struct {
    TokenType CMD; // mov, add, cmp
    TokenType VAL1; // reg, val
    TokenType VAL2; // reg, val
} Token;

#ifdef __cplusplus
extern "C" {
#endif

Token GetTokens(const char* line);

#ifdef __cplusplus
}
#endif 

#endif 