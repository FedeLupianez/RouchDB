#include "keywords.h"
#include "sql/lexer.h"
#include <ctype.h>
#include <string.h>

Keyword* keywords = NULL;
void add_keyword(char* keyword, TokenType type)
{
    Keyword* k = malloc(sizeof(Keyword));
    int klen = strlen(keyword);
    char* upperk = malloc(klen + 1);
    for (int i = 0; i < klen; i++)
        upperk[i] = toupper((unsigned char)keyword[i]); // make keyword always upper
    upperk[klen] = '\0';
    strcpy(k->keyword, upperk);
    free(upperk);
    k->type = type;
    HASH_ADD_STR(keywords, keyword, k);
}

/*
 * Init all keywords of database
 * */
void init_keywords()
{
    add_keyword("SELECT", SELECT);
    add_keyword("FROM", FROM);
    add_keyword("WHERE", WHERE);
    add_keyword("IDENTIFIER", IDENTIFIER);
    add_keyword("INT", NUMBER);
    add_keyword("FLOAT", NUMBER);
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
    return IDENTIFIER;
}
