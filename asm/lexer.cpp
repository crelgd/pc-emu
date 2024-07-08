// LEXER CLASS

#include "lexer.h"
#include <string>
#include <cctype>

// init
Lexer::Lexer(const char* line) {
    _line = line;
    pos = 0;
    cur_char = _line[pos];
}

Lexer::~Lexer() {

}

// Pos++
void Lexer::next_char() {
    pos++;
    if (pos > strlen(_line))
        cur_char = NULL;
    else 
        cur_char = _line[pos];
}

void Lexer::skip_space() {
    while (cur_char != NULL && cur_char == ' ') 
        next_char();
}

Token Lexer::cmd_check() {
    Token tok = {UNDEFINED, UNDEFINED, UNDEFINED, 0};

    char* result = new char;
    while (cur_char != NULL) {
        result += cur_char;
        next_char();
    }
    if (strcmp(result, keywords[0]) == 0)
        tok.CMD = MOV;
    else if (strcmp(result, keywords[1]) == 0)
        tok.CMD = ADD;
    else if (strcmp(result, keywords[2]) == 0)
        tok.CMD = CMP;
    else if (strcmp(result, keywords[3]) == 0) 
        tok.CMD = JIZ;
    else if (strcmp(result, keywords[4]) == 0) 
        tok.CMD = JINZ;
    else if (strcmp(result, keywords[5]) == 0) 
        tok.CMD = JIE;
    else if (strcmp(result, keywords[6]) == 0) 
        tok.CMD = JMP;
    else if (strcmp(result, keywords[7]) == 0) 
        tok.CMD = OUT;
    // check result == keywords[]
    for (int i = 0; i < keywords_v; i++) {
        if (strcmp(result, keywords[i]) == 0) 
            break;
    }
    delete result;
    return tok;
}