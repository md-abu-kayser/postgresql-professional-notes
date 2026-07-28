#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

void exit_on_error(PGconn *conn, PGresult *res, const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, PQerrorMessage(conn));
    if (res) PQclear(res);
    PQfinish(conn);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    const char *conninfo = (argc > 1) ? argv[1] : "dbname=postgres";
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK)
        exit_on_error(conn, NULL, "Connection failed");

    // Create a test table (ignore errors if already exists)
    PGresult *res = PQexec(conn, "CREATE TEMP TABLE IF NOT EXISTS demo (id SERIAL PRIMARY KEY, name TEXT)");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        exit_on_error(conn, res, "CREATE failed");
    PQclear(res);

    // Prepare a statement
    res = PQprepare(conn, "insert_name", "INSERT INTO demo (name) VALUES ($1) RETURNING id", 1, NULL);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        exit_on_error(conn, res, "PREPARE failed");
    PQclear(res);

    // Bind parameter and execute
    const char *paramValues[1] = { "John Doe" };
    res = PQexecPrepared(conn, "insert_name", 1, paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
        exit_on_error(conn, res, "INSERT failed");

    // Print returned id
    printf("Inserted row with id = %s\n", PQgetvalue(res, 0, 0));
    PQclear(res);

    // Query back
    res = PQexec(conn, "SELECT * FROM demo");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
        exit_on_error(conn, res, "SELECT failed");

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++)
        printf("%s | %s\n", PQgetvalue(res, i, 0), PQgetvalue(res, i, 1));
    PQclear(res);

    PQfinish(conn);
    return EXIT_SUCCESS;
}