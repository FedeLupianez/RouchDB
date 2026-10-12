
#include "sql/executor.h"
#include "sql/keywords.h"
#include "sql/lexer.h"
#include "sql/parser.h"
#include "storage/file/file.h"
#include <stdlib.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    RouchFile* file = empty_file("database/test.db", 3);
    init_keywords();

    Token* input = lex_input("CREATE TABLE test (name STRING, age INT);");
    Query query = parse_tokens(input);
    createQuery(query);

    RouchFileFree(file);
    return EXIT_SUCCESS;
}
