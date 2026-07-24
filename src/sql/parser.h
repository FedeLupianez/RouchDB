#pragma once

#include "common/types.h"
#include "sql/lexer.h"

typedef enum {
    SELECT_Q,
    INSERT_Q,
    CREATE_Q,
    UPDATE_Q
} QueryType;

typedef struct {
    int col_count;
    QueryType type;
    char columns[10][STR_LENGHT];
    char table[STR_LENGHT];
    Conditional condition;
} Query;

Query parse_tokens(Token* tokens);
