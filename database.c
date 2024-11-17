#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int initialize_database(char *database_path, sqlite3 **db) {
    char *err_msg = 0;

    int status = sqlite3_open(database_path, db);
    if (status != SQLITE_OK) {
        printf("error: %s\n", sqlite3_errmsg(*db));
        return 0;
    }

    char *sql = "CREATE TABLE IF NOT EXISTS passwords ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT NOT NULL, "
                      "encrypted_password TEXT NOT NULL, "
                      "iv TEXT NOT NULL);";

    status = sqlite3_exec(*db, sql, 0, 0, &err_msg);
    if (status != SQLITE_OK) {
        printf("error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        return 0;
    }

    printf("it work success\n");
    return 1;
}
