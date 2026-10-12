#include "sql/executor.h"
#include "common/errors.h"
#include "common/functions.h"
#include "common/types.h"
#include "sql/parser.h"
#include "storage/tablespace.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

QueryResult selectQuery(Query query);
QueryResult insertQuery(Query query);
QueryResult updateQuery(Query query);

QueryResult createQuery(Query query)
{
    if (query.type != CREATE_Q)
        return (QueryResult) { ERR_INVALID, NULL };
    CreateTableQuery* ct = &query.data.create;
    int column_count = ct->col_count;
    Column* tableColumns = calloc(column_count, sizeof(Column));
    for (int idx = 0; idx < column_count; idx++) {
        snprintf(tableColumns[idx].name, STR_LENGHT, "%s", ct->columns[idx].name);
        tableColumns[idx].type = ct->columns[idx].type;
        tableColumns[idx].configs = 0;
    }
    for (int idx = 0; idx < column_count; idx++) {
        logging("INFO", "ColumnName: %s | ColumnType: %d", tableColumns[idx].name, tableColumns[idx].type);
    }

    Table table;
    memset(&table, 0, sizeof(Table));
    snprintf(table.header.name, STR_LENGHT, "%s", query.table);
    table.columns = tableColumns;
    table.header.col_count = column_count;
    create_space(&table);
    return (QueryResult) { OK, NULL };
}
