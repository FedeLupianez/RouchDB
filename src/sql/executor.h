#pragma once
#include "common/errors.h"
#include "sql/parser.h"
#include "storage/tablespace.h"

typedef struct {
    RouchError error;
    Row* resultRows;
} QueryResult;

QueryResult executeQuery(Query query);
QueryResult selectQuery(Query query);
QueryResult insertQuery(Query query);
QueryResult updateQuery(Query query);
QueryResult createQuery(Query query);
