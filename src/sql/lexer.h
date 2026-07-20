#pragma once

typedef enum {
    TOKEN_SELECT,
    TOKEN_FROM,
    TOKEN_WHERE,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int last_pos;
} Token;

typedef struct {
    const char* input;
    int pos;
} Lexer;

Token* lex_input(const char* input);
