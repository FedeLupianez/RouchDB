#include "parser.h"
#include "common/functions.h"
#include "common/types.h"
#include "sql/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

static TypeID type_from_name(const char* name)
{
    if (strcasecmp(name, "INT") == 0) {
        return TYPE_INT;
    }
    if (strcasecmp(name, "FLOAT") == 0) {
        return TYPE_FLOAT;
    }
    return TYPE_STR;
}

Query parse_tokens(Token* tokens)
{
    int count = 0;
    int is_where = 0;
    int is_table = 0;
    Token* actual_token;

    Query query;
    memset(&query, 0, sizeof(Query));
    do {
        actual_token = (tokens + count);

        switch (actual_token->type) {
        case (SELECT):
            query.type = SELECT_Q;
            break;
        case (INSERT):
            query.type = INSERT_Q;
            break;
        case (CREATE):
            query.type = CREATE_Q;
            break;
        case (FROM):
            break;
        case (TABLE):
            is_table = 1;
            break;
        case (IDENTIFIER):
            if (is_where) {
                snprintf(query.data.select.condition.column, STR_LENGHT, "%s", actual_token->value);
            } else if (is_table) {
                snprintf(query.table, STR_LENGHT, "%s", actual_token->value);
                is_table = 0;
            } else {
                if (query.type == SELECT_Q) {
                    SelectQuery* sq = &query.data.select;
                    if (sq->col_count < MAX_COLUMNS) {
                        snprintf(sq->columns[sq->col_count], STR_LENGHT, "%s", actual_token->value);
                        sq->col_count++;
                    }
                } else if (query.type == CREATE_Q) {
                    CreateTableQuery* ct = &query.data.create;
                    if (ct->col_count < MAX_COLUMNS) {
                        snprintf(ct->columns[ct->col_count].name, STR_LENGHT, "%s", actual_token->value);
                        // El tipo llega en el siguiente token (STRING/NUMBER); no incrementar aún.
                    }
                }
            }
            break;
        case (WHERE):
            is_where = 1;
            break;
        case (OPERATOR):
            if (!is_where || query.data.select.condition.op != NULL) {
                break;
            };
            query.data.select.condition.op = malloc(STR_LENGHT);
            snprintf(query.data.select.condition.op, STR_LENGHT, "%s", actual_token->value);
            break;
        case (STRING):
            if (query.type == CREATE_Q && !is_where) {
                CreateTableQuery* ct = &query.data.create;
                if (ct->col_count < MAX_COLUMNS) {
                    ct->columns[ct->col_count].type = type_from_name(actual_token->value);
                    ct->col_count++;
                }
            } else {
                query.data.select.condition.value.active = TYPE_STR;
                snprintf(query.data.select.condition.value.value.s, STR_LENGHT, "%s", actual_token->value);
                is_where = 0;
            }
            break;
        case (NUMBER):
            if (query.type == CREATE_Q && !is_where) {
                CreateTableQuery* ct = &query.data.create;
                if (ct->col_count < MAX_COLUMNS) {
                    ct->columns[ct->col_count].type = type_from_name(actual_token->value);
                    ct->col_count++;
                }
            } else {
                query.data.select.condition.value.active = TYPE_INT;
                query.data.select.condition.value.value.i = 0;
                is_where = 0;
            }
            break;
        default:
            break;
        };

        count++;
    } while (actual_token->type != TEOF);
    printf("QUERY TYPE : %d\n", query.type);
    printf("QUERY TABLE: %s\n", query.table);
    if (query.type == SELECT_Q) {
        printf("QUERY COLS:\n");
        for (int i = 0; i < query.data.select.col_count; i++) {
            printf("Col: %s\n", query.data.select.columns[i]);
        }
        if (query.data.select.condition.op)
            printf("CONDITION: %s\n", query.data.select.condition.op);
    } else if (query.type == CREATE_Q) {
        printf("QUERY COLS:\n");
        for (int i = 0; i < query.data.create.col_count; i++) {
            printf("Col: %s Type: %d\n", query.data.create.columns[i].name, query.data.create.columns[i].type);
        }
    }

    return query;
}
