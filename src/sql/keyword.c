#include "keywords.h"
#include "sql/lexer.h"

Keyword* keywords = NULL;
void add_keyword(char* keyword, TokenType type)
{
    Keyword* k = malloc(sizeof(Keyword));
    strcpy(k->keyword, keyword);
    k->type = type;
    HASH_ADD_STR(keywords, keyword, k);
}

void init_keywords()
{
    add_keyword("SELECT", SELECT);
    add_keyword("FROM", FROM);
    add_keyword("WHERE", WHERE);
    add_keyword("IDENTIFIER", IDENTIFIER);
    add_keyword("NUMBER", NUMBER);
    add_keyword("STRING", STRING);
    add_keyword("OPERATOR", OPERATOR);
    add_keyword("AND", OPERATOR);
    add_keyword("OR", OPERATOR);
    add_keyword("CREATE", CREATE);
    add_keyword("TABLE", TABLE);
    add_keyword("INSERT", INSERT);
    add_keyword("VALUES", VALUES);
}

TokenType getType(char* keyword)
{
    Keyword* k;
    HASH_FIND_STR(keywords, keyword, k);
    if (k)
        return k->type;
    return UNKNOWN;
}
