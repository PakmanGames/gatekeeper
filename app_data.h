void list_all_credentials();
void add_new_credential();
void change_credential();
void remove_credential();
void change_database_password();
int verify_password(char *password, char *db_name);
void create_database(char *password, char *db_name);
void close_database();
void save_database();