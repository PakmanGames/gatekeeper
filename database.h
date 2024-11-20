// Check status codes functions
int check_status(int status, sqlite3 *db);
int check_is_done(int status, sqlite3 *db);

// Open and close the connection
int open_connection(sqlite3 **db, char *database_path);
void close_connection(sqlite3 *db);

// Initialize the database
int initialize_database(sqlite3 *db);

// Interactions with database
int add_password(sqlite3 *db, char *name, char *password);
int get_password(sqlite3 *db, char *name);
int update_password(sqlite3 *db, char *name, char *old_password, char *new_password);
int delete_password(sqlite3 *db, char *name);
int list_passwords(sqlite3 *db);