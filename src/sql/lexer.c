#pragma once
#include "lexer.h"
#include "common/functions.h"
#include "sql/keywords.h"
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
    char* upper = malloc(lenght + 1);
    for (int i = 0; i < lenght; i++)
        upper[i] = toupper((unsigned char)word[i]);
    upper[lenght] = '\0';
    TokenType type = getType(upper);
    free(upper);

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
        .type = NUMBER,
        .value = number,
        .last_pos = pos
    };
}

Token get_next_token(Lexer* lexer)
{
    while (isspace(lexer->input[lexer->pos]))
        lexer->pos++;

    if (isalpha(lexer->input[lexer->pos])) {
        Token next = read_word(lexer->input, lexer->pos);
        if (lexer->last_type == FROM && next.type == IDENTIFIER) {
            next.type = TABLE;
            lexer->last_type = TABLE;
        } else {
            lexer->last_type = next.type;
        }
        return next;
    }
    if (isdigit(lexer->input[lexer->pos])) {
        lexer->last_type = NUMBER;
        return read_number(lexer->input, lexer->pos);
    }

    if (lexer->input[lexer->pos] == '=') {
        lexer->pos++;
        lexer->last_type = OPERATOR;
        return (Token) {
            .type = OPERATOR,
            .value = "=",
            .last_pos = lexer->pos
        };
    }

    if (lexer->input[lexer->pos] == ',') {
        lexer->pos++;
        lexer->last_type = DELIMITER;
        return (Token) {
            .type = DELIMITER,
            .value = ",",
            .last_pos = lexer->pos
        };
    }

    if (lexer->input[lexer->pos] == '(') {
        lexer->pos++;
        lexer->last_type = INIT_BRACKET;
        return (Token) {
            .type = INIT_BRACKET,
            .value = "(",
            .last_pos = lexer->pos
        };
    }

    if (lexer->input[lexer->pos] == ')') {
        lexer->pos++;
        lexer->last_type = END_BRACKET;
        return (Token) {
            .type = END_BRACKET,
            .value = ")",
            .last_pos = lexer->pos
        };
    }

    if (lexer->input[lexer->pos] == ';' || lexer->input[lexer->pos] == '\0') {
        lexer->pos++;
        lexer->last_type = TEOF;
        return (Token) {
            .type = TEOF,
            .value = ";",
            .last_pos = lexer->pos
        };
    }

    if (lexer->input[lexer->pos] == '*') {
        lexer->pos++;
        lexer->last_type = IDENTIFIER;
        return (Token) {
            .type = IDENTIFIER,
            .value = "*",
            .last_pos = lexer->pos
        };
    }

    lexer->pos++;
    lexer->last_type = UNKNOWN;
    return (Token) {
        .type = UNKNOWN,
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
    Lexer lexer = (Lexer) { .input = input, .pos = 0, .last_type = TEOF };
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
        logging("INFO", "Token: %s | Type: %d \n", actual_token.value, actual_token.type);
    } while (actual_token.type != TEOF);
    return tokens;
};
