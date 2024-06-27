#include "lexer.h"
#include <string.h>

Token GetTokens(const char* line) {
    Token token = {UNDEFINED, UNDEFINED, UNDEFINED};
    int line_size = strlen(line);

    const char* mov = "mov ";
    const char* add = "add ";
    const char* cmp = "cmp ";
    const char* jmp = "jmp ";
    const char* jz = "jz ";
    const char* jnz = "jnz ";
    const char* je = "je ";

    // CMD
    if (line_size >= 4 && strncmp(line, mov, 4) == 0)
        token.CMD = MOV;
    else if (line_size >= 4 && strncmp(line, add, 4) == 0)
        token.CMD = ADD;
    else if (line_size >= 4 && strncmp(line, cmp, 4) == 0) 
        token.CMD = CMP;
    else if (line_size >= 4 && strncmp(line, jmp, 4) == 0)
        token.CMD = JMP;
    else if (line_size >= 3 && strncmp(line, jz, 3) == 0) 
        token.CMD = JIZ;
    else if (line_size >= 4 && strncmp(line, jnz, 4) == 0) 
        token.CMD = JINZ;
    else if (line_size >= 3 && strncmp(line, je, 3) == 0) 
        token.CMD = JIE;
    else return token;

    return token;
}
