#include "parser.h"
#include "common/functions.h"
#include "common/types.h"
#include "sql/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Query parse_tokens(Token* tokens)
{
    int count = 0;
    int is_where = 0;
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
            snprintf(query.table, STR_LENGHT, "%s", actual_token->value);
            break;
        case (IDENTIFIER):
            if (is_where) {
                snprintf(query.condition.column, STR_LENGHT, "%s", actual_token->value);
            } else {
                snprintf(query.columns[query.col_count], STR_LENGHT, "%s", actual_token->value);
                query.col_count++;
            }
            break;
        case (WHERE):
            is_where = 1;
            break;
        case (OPERATOR):
            if (!is_where || query.condition.op != NULL) {
                break;
            };
            query.condition.op = malloc(STR_LENGHT);
            snprintf(query.condition.op, STR_LENGHT, "%s", actual_token->value);
            break;
        case (STRING):
            query.condition.value.active = TYPE_STR;
            snprintf(query.condition.value.value.s, STR_LENGHT, "%s", actual_token->value);
            is_where = 0;
            break;
        case (NUMBER):
            query.condition.value.active = TYPE_INT;
            query.condition.value.value.i = 0;
            is_where = 0;
            break;
        default:
            if (actual_token->type == TEOF)
                break;
            logging("ERROR", "Error with token type %d", actual_token->type);
            break;
        };

        count++;
    } while (actual_token->type != TEOF);
    printf("QUERY TYPE : %d\n", query.type);
    printf("QUERY TABLE: %s\n", query.table);
    printf("QUERY COLS:\n");
    for (int i = 0; i < query.col_count; i++) {
        printf("Col: %s\n", (char*)(query.columns + i));
    }
    if (query.condition.op)
        printf("CONDITION: %s\n", query.condition.op);

    return query;
}
