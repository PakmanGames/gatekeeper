int open_connection(sqlite3 **db, char *database_path);
void close_connection(sqlite3 *db);

// init database return db else 0 cuz not work
int initialize_database(sqlite3 *db);

int add_password(sqlite3 *db, char *name, char *password);
int get_password(sqlite3 *db, char *name);
int update_password(sqlite3 *db, char *name, char *old_password, char *new_password);
int delete_password(sqlite3 *db, char *name);
int list_passwords(sqlite3 *db);