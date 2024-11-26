#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

struct credentials {
    char *name;
    char *password;
};

int check_status(int status, sqlite3 *db) {
    if (status != SQLITE_OK) {
        printf("error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return 1;
}

int check_is_done(int status, sqlite3 *db) {
    if (status != SQLITE_DONE) {
        printf("error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return 1;
}

int open_connection(sqlite3 **db, char *database_path) {
    int status = sqlite3_open(database_path, db);
    if (!check_status(status, *db)) {
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
                      "password TEXT NOT NULL);";

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

int add_password(sqlite3 *db, char *name, char *password) {
    sqlite3_stmt *stmt;
    char *sql = "INSERT INTO passwords (name, password) VALUES (?, ?);";

    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (!check_status(status, db)) {
        return 0;
    }

    // Bind the values to the ?
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    status = sqlite3_step(stmt);
    if (!check_is_done(status, db)) {
        return 0;
    }
    return 1;
}

int delete_password(sqlite3 *db, char *name) {
    sqlite3_stmt *stmt;
    char *sql = "DELETE FROM passwords WHERE name = ?;";

    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (!check_status(status, db)) {
        return 0;
    }

    // Bind the values to the ?
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    status = sqlite3_step(stmt);
    if (!check_is_done(status, db)) {
        return 0;
    }
    return 1;
}

int update_password(sqlite3 *db, char *name, char *old_password, char *new_password) {
    sqlite3_stmt *stmt;
    char *sql = "UPDATE passwords SET password = ? WHERE name = ? AND password = ?;";

    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (!check_status(status, db)) {
        return 0;
    }

    // Bind the values to the ?
    sqlite3_bind_text(stmt, 1, new_password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, old_password, -1, SQLITE_STATIC);

    status = sqlite3_step(stmt);
    if (!check_is_done(status, db)) {
        return 0;
    }
    return 1;
}

struct credentials *get_password(sqlite3 *db, char *name) {
    sqlite3_stmt *stmt;
    char *sql = "SELECT password FROM passwords WHERE name = ?;";

    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (!check_status(status, db)) {
        return NULL;
    }

    // Bind the values to the ?
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    status = sqlite3_step(stmt);
    if (status == SQLITE_ROW) {
        struct credentials *result = malloc(sizeof(struct credentials));
        if (!result) {
            printf("error: malloc failed\n");
            return NULL;
        }

        result->name = strdup(name);
        result->password = strdup((char *) sqlite3_column_text(stmt, 0));

        return result;
    } else if (!check_is_done(status, db)) {
        return NULL;
    }

    return NULL;
}