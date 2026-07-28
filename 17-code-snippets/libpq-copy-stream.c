#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

int main(int argc, char *argv[]) {
    const char *conninfo = (argc > 1) ? argv[1] : "dbname=postgres";
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return EXIT_FAILURE;
    }

    // Create target table
    PGresult *res = PQexec(conn, "CREATE TEMP TABLE IF NOT EXISTS csv_import (col1 text, col2 int, col3 date)");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Table creation failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return EXIT_FAILURE;
    }
    PQclear(res);

    // Start COPY
    res = PQexec(conn, "COPY csv_import FROM STDIN WITH (FORMAT CSV)");
    if (PQresultStatus(res) != PGRES_COPY_IN) {
        fprintf(stderr, "COPY failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return EXIT_FAILURE;
    }
    PQclear(res);

    // Read stdin and send data
    char buf[4096];
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        // fgets leaves newline, COPY expects newline terminator
        if (PQputCopyData(conn, buf, strlen(buf)) != 1) {
            fprintf(stderr, "PUT data error: %s\n", PQerrorMessage(conn));
            PQputCopyEnd(conn, "put error");
            PQfinish(conn);
            return EXIT_FAILURE;
        }
    }

    // Signal end of data
    if (PQputCopyEnd(conn, NULL) != 1) {
        fprintf(stderr, "PUT end error: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return EXIT_FAILURE;
    }

    // Get the COPY result
    res = PQgetResult(conn);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "COPY end failed: %s\n", PQresultErrorMessage(res));
        PQclear(res);
        PQfinish(conn);
        return EXIT_FAILURE;
    }
    printf("COPY completed: %s\n", PQcmdTuples(res));
    PQclear(res);

    PQfinish(conn);
    return EXIT_SUCCESS;
}