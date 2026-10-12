#pragma once

typedef enum {
    SELECT,
    FROM,
    WHERE,
    IDENTIFIER,
    NUMBER,
    STRING,
    OPERATOR,
    CREATE,
    TABLE,
    INSERT,
    VALUES,
    DELIMITER,
    INIT_BRACKET,
    END_BRACKET,
    UNKNOWN,
    TEOF
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int last_pos;
} Token;

typedef struct {
    const char* input;
    int pos;
    TokenType last_type;
} Lexer;

Token* lex_input(const char* input);
