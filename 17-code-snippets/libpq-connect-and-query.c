#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

int main(int argc, char *argv[]) {
    const char *conninfo = (argc > 1) ? argv[1] : "dbname=postgres";
    PGconn     *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return EXIT_FAILURE;
    }

    PGresult *res = PQexec(conn, "SELECT version(), current_timestamp");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return EXIT_FAILURE;
    }

    int nrows = PQntuples(res);
    int ncols = PQnfields(res);
    printf("Query returned %d rows, %d columns.\n", nrows, ncols);

    // Print column names
    for (int i = 0; i < ncols; i++)
        printf("%-40s ", PQfname(res, i));
    putchar('\n');

    // Print rows
    for (int r = 0; r < nrows; r++) {
        for (int c = 0; c < ncols; c++)
            printf("%-40s ", PQgetvalue(res, r, c));
        putchar('\n');
    }

    PQclear(res);
    PQfinish(conn);
    return EXIT_SUCCESS;
}