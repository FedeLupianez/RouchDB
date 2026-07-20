#pragma once
#include "lexer.h"
#include "common/functions.h"
#include <common/types.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Token read_word(const char* text, int pos)
{
    int start = pos;

    while (isalnum((unsigned char)(text[pos])) || text[pos] == '_') {
        pos++;
    }
    int lenght = pos - start;
    char* word = malloc(lenght + 1);
    memcpy(word, &text[start], lenght);
    word[lenght] = '\0';
    TokenType type = TOKEN_IDENTIFIER;
    if (strcmp(word, "SELECT") == 0) {
        type = TOKEN_SELECT;
    } else if (strcmp(word, "FROM") == 0) {
        type = TOKEN_FROM;
    } else if (strcmp(word, "WHERE") == 0) {
        type = TOKEN_WHERE;
    } else if (strcmp(word, "AND") == 0 || strcmp(word, "OR") == 0 || strcmp(word, "=") == 0) {
        type = TOKEN_OPERATOR;
    }
    return (Token) {
        .type = type,
        .value = word,
        .last_pos = pos
    };
}

Token read_number(const char* text, int pos)
{
    int start = pos;

    while (isdigit(text[pos]))
        pos++;
    int lenght = pos - start;
    char* number = strndup(&text[start], lenght);
    return (Token) {
        .type = TOKEN_NUMBER,
        .value = number,
        .last_pos = pos
    };
}

Token get_next_token(Lexer* lexer)
{
    while (isspace(lexer->input[lexer->pos]))
        lexer->pos++;

    if (isalpha(lexer->input[lexer->pos]))
        return read_word(lexer->input, lexer->pos);
    if (isdigit(lexer->input[lexer->pos]))
        return read_number(lexer->input, lexer->pos);

    if (lexer->input[lexer->pos] == '=') {
        lexer->pos++;
        return (Token) {
            .type = TOKEN_OPERATOR,
            .value = NULL,
            .last_pos = lexer->pos
        };
    }

    if (lexer->input[lexer->pos] == ';' || lexer->input[lexer->pos] == '\0')
        return (Token) {
            .type = TOKEN_EOF,
            .value = ";",
            .last_pos = lexer->pos
        };
    lexer->pos++;
    return (Token) {
        .type = TOKEN_UNKNOWN,
        .last_pos = lexer->pos
    };
}

Token* lex_input(const char* input)
{
    Token* tokens = malloc(sizeof(Token) * 2);
    int actual_idx = 0;
    int tokens_size = 2;
    if (!tokens)
        return NULL;
    Lexer lexer = (Lexer) { input, 0 };
    Token actual_token;
    do {
        actual_token = get_next_token(&lexer);
        if (actual_idx == tokens_size) {
            tokens_size += 2;
            Token* tmp = realloc(tokens, sizeof(Token) * tokens_size);
            if (!tmp) {
                free(tokens);
                return NULL;
            }
            tokens = tmp;
        }
        tokens[actual_idx++] = actual_token;
        lexer.pos = actual_token.last_pos;
        logging("INFO", "Token : %s\n", actual_token.value);
    } while (actual_token.type != TOKEN_EOF);
    return tokens;
};
