#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libpq-fe.h>

int main(int argc, char *argv[]) {
    const char *conninfo = (argc > 1) ? argv[1] : "dbname=postgres";
    const char *channel  = (argc > 2) ? argv[2] : "test_channel";

    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return EXIT_FAILURE;
    }

    // LISTEN
    PGresult *res = PQexec(conn, "LISTEN test_channel");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "LISTEN failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return EXIT_FAILURE;
    }
    PQclear(res);
    printf("Listening on channel '%s'. Use 'NOTIFY test_channel, ''hello'';' to test.\n", channel);

    // Enter event loop
    while (1) {
        // Consume any input from server
        if (PQconsumeInput(conn) == 0) {
            fprintf(stderr, "Connection lost.\n");
            break;
        }

        // Check for notifications
        PGnotify *notify;
        while ((notify = PQnotifies(conn)) != NULL) {
            printf("NOTIFY received: pid=%d, channel=%s, payload='%s'\n",
                   notify->be_pid, notify->relname, notify->extra);
            PQfreemem(notify);
        }

        // Wait a bit before polling again
        usleep(100000);  // 100 ms
    }

    PQfinish(conn);
    return EXIT_SUCCESS;
}