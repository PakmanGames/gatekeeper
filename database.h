#include <sqlite3.h>
// This function checks the status of an SQLite operation and prints an error message if it fails.
int check_status(int status, sqlite3 *db);
// This function checks if an SQLite operation is done and prints an error message if it is not.
int check_is_done(int status, sqlite3 *db);

// Open and close the connection
int open_connection(sqlite3 **db, char *database_path);
void close_connection(sqlite3 *db);

// Initialize the database
int initialize_database(sqlite3 *db);

// Struct for when we get a password
struct credentials
{
    char *name;
    char *password;
};

// Struct for when we get a list of passwords
struct credentials_list
{
    struct credentials *entries;
    int length;
};

// Interactions with database

// Add a password to the database
int add_password(sqlite3 *db, char *name, char *password);
// Get a password from the database
struct credentials *get_password(sqlite3 *db, char *name);
// Update a password in the database
int update_password(sqlite3 *db, char *name, char *old_password, char *new_password);
// Delete a password from the database
int delete_password(sqlite3 *db, char *name);
// List all passwords in the database
struct credentials_list *list_passwords(sqlite3 *db);