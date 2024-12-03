/**
 * Team 20 - Memory Leakers
 * Ahmed Yassin, 400536694
 * Andy Pak, 400530925
 * Patrick Molka, 400537630
 * Aditya Rao, 400517325
 * 
 * database.c contains functions for interacting with an SQLite database.
 * It includes functions for opening and closing the connection, initializing the database,
 * adding, deleting, updating, and retrieving passwords, and listing all passwords.
 * 
 * Contents:
 * - SQLite database connection functions
 * - SQLite database initialization
 * - Password management functions
 * - Password retrieval functions
 * - Password listing functions
 * - Error checking functions
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "database.h"

/**
 * check_status
 * This function checks the status of an SQLite operation and prints an error message if it fails.
 * 
 * Parameters:
 *  - status: The status of the SQLite operation.
 *  - db: The SQLite database connection.
 * 
 * Returns:
 *  - 1 if the operation was successful.
 *  - 0 if the operation failed.
 */
int check_status(int status, sqlite3 *db)
{
    if (status != SQLITE_OK)
    {
        printf("error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return 1;
}

/**
 * check_is_done
 * This function checks if an SQLite operation is done and prints an error message if it is not.
 * 
 * Parameters:
 * - status: The status of the SQLite operation.
 * - db: The SQLite database connection.
 * 
 * Returns:
 * - 1 if the operation is done.
 * - 0 if the operation is not done.
 */
int check_is_done(int status, sqlite3 *db)
{
    if (status != SQLITE_DONE)
    {
        printf("error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return 1;
}

/**
 * open_connection
 * This function opens a connection to an SQLite database.
 * 
 * Parameters:
 * - db: The SQLite database connection.
 * - database_path: The path to the SQLite database file.
 * 
 * Returns:
 * - 1 if the connection was successful.
 * - 0 if the connection failed.
 */
int open_connection(sqlite3 **db, char *database_path)
{
    int status = sqlite3_open(database_path, db);
    if (!check_status(status, *db))
    {
        return 0;
    }
    return 1;
}

/**
 * close_connection
 * This function closes a connection to an SQLite database.
 * 
 * Parameters:
 * - db: The SQLite database connection.
 */
void close_connection(sqlite3 *db)
{
    if (db != NULL)
    {
        sqlite3_close(db);
    }
}

/**
 * initialize_database
 * This function initializes an SQLite database by creating a table for storing passwords.
 * 
 * Parameters:
 * - db: The SQLite database connection.
 * 
 * Returns:
 * - 1 if the database was successfully initialized.
 * - 0 if the database initialization failed.
 */
int initialize_database(sqlite3 *db)
{
    char *err_msg = 0;

    // Create the table schema using SQL
    char *sql = "CREATE TABLE IF NOT EXISTS passwords ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "name TEXT NOT NULL, "
                "password TEXT NOT NULL);";

    int status = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (status != SQLITE_OK)
    {
        printf("error: %s\n", err_msg);
        sqlite3_free(err_msg);
        close_connection(db);
        return 0;
    }

    printf("it work success\n");
    return 1;
}

/**
 * add_password
 * This function adds a password to the SQLite database.
 * 
 * Parameters:
 * - db: The SQLite database connection.
 * - name: The name of the password.
 * - password: The password to add.
 * 
 * Returns:
 * - 1 if the password was successfully added.
 * - 0 if the password addition failed.
 */
int add_password(sqlite3 *db, char *name, char *password)
{
    sqlite3_stmt *stmt;
    // SQL query to insert a password into the database
    char *sql = "INSERT INTO passwords (name, password) VALUES (?, ?);";

    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (!check_status(status, db))
    {
        return 0;
    }

    // Bind the values to the ?
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    status = sqlite3_step(stmt);
    if (!check_is_done(status, db))
    {
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}

/**
 * delete_password
 * This function deletes a password from the SQLite database.
 * 
 * Parameters:
 * - db: The SQLite database connection.
 * - name: The name of the password to delete.
 * 
 * Returns:
 * - 1 if the password was successfully deleted.
 * - 0 if the password deletion failed.
 */
int delete_password(sqlite3 *db, char *name)
{
    sqlite3_stmt *stmt;
    // SQL query to delete a password from the database
    char *sql = "DELETE FROM passwords WHERE name = ?;";

    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (!check_status(status, db))
    {
        return 0;
    }

    // Bind the values to the ?
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    status = sqlite3_step(stmt);
    if (!check_is_done(status, db))
    {
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}

/**
 * update_password
 * This function updates a password in the SQLite database.
 * 
 * Parameters:
 * - db: The SQLite database connection.
 * - name: The name of the password to update.
 * - old_password: The old password to update.
 * - new_password: The new password to set.
 * 
 * Returns:
 * - 1 if the password was successfully updated.
 * - 0 if the password update failed.
 */
int update_password(sqlite3 *db, char *name, char *old_password, char *new_password)
{
    sqlite3_stmt *stmt;
    // SQL query to update a password in the database
    char *sql = "UPDATE passwords SET password = ? WHERE name = ? AND password = ?;";

    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (!check_status(status, db))
    {
        return 0;
    }

    // Bind the values to the ?
    sqlite3_bind_text(stmt, 1, new_password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, old_password, -1, SQLITE_STATIC);

    status = sqlite3_step(stmt);
    if (!check_is_done(status, db))
    {
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

/**
 * get_password
 * This function retrieves a password from the SQLite database.
 * 
 * Parameters:
 * - db: The SQLite database connection.
 * - name: The name of the password to retrieve.
 * 
 * Returns:
 * - A pointer to a struct containing the name and password if the password was found.
 * - NULL if the password was not found or an error occurred.
 */
struct credentials *get_password(sqlite3 *db, char *name)
{
    sqlite3_stmt *stmt;
    // SQL query to select a password from the database
    char *sql = "SELECT password FROM passwords WHERE name = ?;";

    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (!check_status(status, db))
    {
        return NULL;
    }

    // Bind the values to the ?
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    // Check if the password was found
    status = sqlite3_step(stmt);
    if (status == SQLITE_ROW)
    {
        // Allocate memory for the result
        struct credentials *result = malloc(sizeof(struct credentials));
        if (!result)
        {
            printf("error: malloc failed\n");
            return NULL;
        }

        // Copy the name and password
        result->name = strdup(name);
        result->password = strdup((char *)sqlite3_column_text(stmt, 0));

        return result;
    }
    else if (!check_is_done(status, db))
    {
        return NULL;
    }
    sqlite3_finalize(stmt);

    return NULL;
}

/**
 * list_passwords
 * This function retrieves all passwords from the SQLite database.
 * 
 * Parameters:
 * - db: The SQLite database connection.
 * 
 * Returns:
 * - A pointer to a struct containing an array of credentials if the passwords were found.
 * - NULL if the passwords were not found or an error occurred.
 */
struct credentials_list *list_passwords(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    // SQL query to select all passwords from the database
    char *sql = "SELECT name, password FROM passwords;";

    int status = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (!check_status(status, db))
    {
        return NULL;
    }

    // Allocate memory for the results
    struct credentials_list *results = malloc(sizeof(struct credentials_list));
    if (!results)
    {
        printf("error: malloc failed\n");
        return NULL;
    }

    // Initialize the results
    results->length = 0;
    results->entries = NULL;

    // Loop through the results
    while ((status = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        // Reallocate memory for the results
        struct credentials *new_entries = realloc(results->entries, (results->length + 1) * sizeof(struct credentials));
        if (!new_entries)
        {
            printf("Error: realloc failed\n");
            return NULL;
        }

        // Update the results
        results->entries = new_entries;

        // Copy the name and password
        struct credentials *result = &results->entries[results->length];
        result->name = strdup((char *)sqlite3_column_text(stmt, 0));
        result->password = strdup((char *)sqlite3_column_text(stmt, 1));

        if (!result->name || !result->password)
        {
            printf("Error: strdup failed\n");
            return NULL;
        }

        results->length++;
    }

    if (!check_is_done(status, db))
    {
        return NULL;
    }
    sqlite3_finalize(stmt);

    return results;
}