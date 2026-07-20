#pragma once
#include "sql/lexer.h"

typedef struct {
    char keyword[32];
    TokenType type;
    UT_hash_handle hh;
} Keyword;

extern Keyword* keywords;

void add_keyword(char* keyword, TokenType type);
void init_keywords();

TokenType getType(char* keyword);
