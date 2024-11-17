int open_connection(sqlite3 **db, char *database_path);
void close_connection(sqlite3 *db);

// init database return db else 0 cuz not work
int initialize_database(sqlite3 *db);