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
    char name[STR_LENGHT];
    TypeID type;
    int length;
    bool is_nullable;
    bool is_primary_key;
    bool is_unique;
    bool is_auto_increment;
} ColumnDefinition;

typedef struct {
    int col_count;
    ColumnDefinition columns[MAX_COLUMNS];
} CreateTableQuery;

typedef struct {
    int col_count;
    char columns[MAX_COLUMNS][STR_LENGHT];
    Conditional condition;
} SelectQuery;

typedef struct {
    char table[STR_LENGHT];
    QueryType type;
    union {
        CreateTableQuery create;
        SelectQuery select;
    } data;
} Query;

Query parse_tokens(Token* tokens);
