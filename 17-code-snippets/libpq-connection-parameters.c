#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

void exit_on_error(PGconn *conn) {
    fprintf(stderr, "%s\n", PQerrorMessage(conn));
    PQfinish(conn);
    exit(EXIT_FAILURE);
}

int main() {
    const char *conninfo = getenv("DATABASE_URL");
    if (!conninfo)
        conninfo = "dbname=postgres";  // will use PGHOST, PGPORT etc. if set

    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK)
        exit_on_error(conn);

    printf("Connected successfully.\n");
    printf("Server version: %d\n", PQserverVersion(conn));
    printf("Client encoding: %s\n", pg_encoding_to_char(PQclientEncoding(conn)));

    // Demonstrate parsing a connection string for known keys
    PQconninfoOption *opts = PQconninfoParse(conninfo, NULL);
    if (opts) {
        printf("Parsed connection options:\n");
        for (PQconninfoOption *o = opts; o->keyword; o++) {
            if (o->val)
                printf("  %-20s = %s\n", o->keyword, o->val);
        }
        PQconninfoFree(opts);
    }

    PQfinish(conn);
    return EXIT_SUCCESS;
}