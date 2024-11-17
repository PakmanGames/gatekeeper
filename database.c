#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int open_connection(sqlite3 **db, char *database_path) {
    int status = sqlite3_open(database_path, db);
    if (status != SQLITE_OK) {
        printf("error: %s\n", sqlite3_errmsg(*db));
        return 0;
    }
    return 1;
}

void close_connection(sqlite3 *db) {
    if (db != NULL) {
        sqlite3_close(db);
    }
}

int initialize_database(sqlite3 *db) {
    char *err_msg = 0;

    char *sql = "CREATE TABLE IF NOT EXISTS passwords ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT NOT NULL, "
                      "encrypted_password TEXT NOT NULL, "
                      "iv TEXT NOT NULL);";

    int status = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (status != SQLITE_OK) {
        printf("error: %s\n", err_msg);
        sqlite3_free(err_msg);
        close_connection(db);
        return 0;
    }

    printf("it work success\n");
    return 1;
}
