/* 
Team 20 - Memory Leakers
Ahmed Yassin, 400536694
Andy Pak, 400530925
Patrick Molka, 400537630
Aditya Rao, 400517325

app_data.c handles the creation, management, and encryption of a SQLite-based database. 
It includes operations such as adding, removing, and changing credentials, as well as managing database connections and secure data storage.

This file contains:
- Database connection handling
- Functions for password encryption and decryption
- Functions to add, remove, and change credentials
*/

#include "app_data.h"
#include "database.h"
#include "data_encryption.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>

#define DATABASE_CONN ":memory:"    // Define the database connection string (in-memory)
#define MAX_NAME_LENGTH 50          // Max length for credential names
#define MAX_PASSWORD_LENGTH 50      // Max length for passwords

sqlite3 *db;             // SQLite database handle
sqlite3_int64 db_size;   // Size of the database in bytes
char *password;          // Store the database password
unsigned char *salt;     // Store the salt used for encryption
char *db_name;           // Store the database file name
unsigned char *data;     // Store serialized database data

/* 
create_database
This function initializes a new database connection and sets up the necessary structures 
to handle secure data storage. It accepts a password for the database and a name for the database file.
It also generates a salt and secures the database with encryption.

Parameters:
    - o_password: The password to use for the database.
    - o_db_name: The name of the database file.
*/
void create_database(char *o_password, char *o_db_name)
{
    int status = open_connection(&db, DATABASE_CONN);           // Open a connection to the database
    password = malloc(MAX_PASSWORD_LENGTH * sizeof(char));      // Allocate memory for password
    salt = malloc(SALT_LEN * sizeof(char));                     // Allocate memory for salt
    db_name = malloc(MAX_NAME_LENGTH * sizeof(char));           // Allocate memory for database name
    strcpy(password, o_password);                               // Copy user-provided password to the global variable
    strcpy(db_name, o_db_name);                                 // Copy user-provided database name to the global variable
    generate_secure_random(SALT_LEN, (char *)salt);             // Generate a random salt

    // Check if the database connection was successful
    if (!status)
    {
        printf("Error: Could not open database connection.\n");
        return;
    }

    // Initialize database schema
    status = initialize_database(db);
    // Check if database initialization was successful
    if (!status)
    {
        printf("Error: Could not initialize database.\n");
        close_connection(db); // Close database connection if initializion failed
        return;
    }
    save_database(); // Placeholder function to save the database
}

/* 
close_database
This function frees memory allocated for password, database name, and other resources 
when the database is closed. It helps in cleaning up the app's resources.

Parameters:
    None
*/
void close_database()
{
#ifdef DEBUG
    printf("Freeing password...\n");
#endif
    free(password);
#ifdef DEBUG
    printf("Freeing db name...\n");
#endif
    free(db_name);
#ifdef DEBUG
    printf("Freeing salt...\n");
#endif
    free(salt);
#ifdef DEBUG
    printf("Freeing data...\n");
#endif
}

/* 
list_all_credentials
This function retrieves and prints all stored credentials in the database. It lists the name 
and password for each credential.

Parameters:
    None
*/
void list_all_credentials()
{
    // Retrieve stored credentials from database
    struct credentials_list *list = list_passwords(db);
    // Check if credentials retrieval was successful
    if (!list)
    {
        printf("Error: Could not retrieve credentials.\n");
        return;
    }

    printf("Listing all credentials:\n");
    // Loop through each credential in the list
    for (int i = 0; i < list->length; i++)
    {
        printf("%d) Name: %s, Password: %s\n", i, list->entries[i].name, list->entries[i].password);
    }
}

/* 
add_new_credential
This function allows the user to input a new credential (name and password) 
and adds it to the database.

Parameters:
    None
*/
void add_new_credential()
{
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter the name for the new credential: ");
    // Validate user input for name
    if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL || name[0] == '\n')
    {
        printf("Invalid credential name. Please try again.\n");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    printf("Enter the password for the new credential: ");
    // Validate user input for password
    if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL || password[0] == '\n')
    {
        printf("Invalid password. Please try again.\n");
        return;
    }
    password[strcspn(password, "\n")] = '\0';

    // Check if new credential can be added to the database
    if (!add_password(db, name, password))
    {
        printf("Error: Could not add credential.\n");
        return;
    }

    printf("Credential added successfully.\n");
}

/* 
remove_credential
This function allows the user to remove an existing credential by specifying its name.

Parameters:
    None
*/
void remove_credential()
{
    char name[MAX_NAME_LENGTH];

    printf("Enter the name of the credential to remove: ");
    // Validate user input for name
    if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL || name[0] == '\n')
    {
        printf("Invalid credential name. Please try again.\n");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    // Check if the credential can be removed from the database
    if (!delete_password(db, name))
    {
        printf("Error: Could not remove credential.\n");
        return;
    }

    printf("Credential removed successfully.\n");
}

/* 
change_credential
This function allows the user to change the password for an existing credential.

Parameters:
    None
*/
void change_credential()
{
    char name[MAX_NAME_LENGTH];
    char old_password[MAX_PASSWORD_LENGTH];
    char new_password[MAX_PASSWORD_LENGTH];

    printf("Enter the name of the credential to change: ");
    // Validate user input for name
    if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL || name[0] == '\n')
    {
        printf("Invalid credential name. Please try again.\n");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    printf("Enter the current password for the credential: ");
    // Validate old password input
    if (fgets(old_password, MAX_PASSWORD_LENGTH, stdin) == NULL || old_password[0] == '\n')
    {
        printf("Invalid password. Please try again.\n");
        return;
    }
    old_password[strcspn(old_password, "\n")] = '\0';

    printf("Enter the new password for the credential: ");
    // Validate new password input
    if (fgets(new_password, MAX_PASSWORD_LENGTH, stdin) == NULL || new_password[0] == '\n')
    {
        printf("Invalid password. Please try again.\n");
        return;
    }
    new_password[strcspn(new_password, "\n")] = '\0';

    // Check if the password can be updated in the database
    if (!update_password(db, name, old_password, new_password))
    {
        printf("Error: Could not change credential.\n");
        return;
    }

    printf("Credential changed successfully.\n");
}

/* 
change_database_password
This function allows the user to change the password used to encrypt the database.

Parameters:
    None
*/
void change_database_password()
{
    if (password != NULL)
    {
        free(password); // Free the old password memory
    }
    password = malloc(MAX_PASSWORD_LENGTH * sizeof(char)); // Allocate new memory for the password

    printf("Enter the new password for the database: ");
    // Validate user input for new password
    if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL || password[0] == '\n')
    {
        printf("Invalid password. Please try again.\n");
        return;
    }
    password[strcspn(password, "\n")] = '\0';

    printf("Database password changed successfully.\n");
}

/* 
save_database
This function serializes the database and saves it to a file in an encrypted form. 
It uses the provided password and a salt value to encrypt the database before saving.

Parameters:
    None
*/
void save_database()
{
#ifdef DEBUG
    printf("Saving database to file...\n");
#endif
    data = sqlite3_serialize(db, "main", &db_size, 0);

    if (data == NULL)
    {
        printf("Error: Could not serialize database.\n");
        printf("%s\n", sqlite3_errmsg(db));
        exit(1);
        return;
    }
#ifdef DEBUG
    printf("Serialized database to bytes\n");
#endif

    // Write the serialized database to a file
    char *ciphertext = malloc(db_size + SALT_LEN * sizeof(char) + EVP_MAX_BLOCK_LENGTH);

#ifdef DEBUG
    printf("Ciphertext mallocated\n");
#endif

    int length;
    if (!encrypt_data_salt_pepper((char *)data, db_size, password, salt, ciphertext, &length))
    {
        printf("Error: Could not encrypt database.\n");
        return;
    }

#ifdef DEBUG
    printf("Encrypted bytes\n");

    char *db_name_unencrypted = malloc(strlen(db_name) + 7 * sizeof(char));
    sprintf(db_name_unencrypted, "%s_unenc", db_name);
    printf("Unencrypted database saved to file: %s\n", db_name_unencrypted);

    FILE *un_file = fopen(db_name_unencrypted, "wb");
    if (!file)
    {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    fwrite(data, 1, db_size, un_file);
    fclose(un_file);
#endif

    FILE *file = fopen(db_name, "wb");

    fwrite(salt, 1, SALT_LEN, file);
    fwrite(ciphertext, 1, length, file);
    fclose(file);
    free(ciphertext);

#ifdef DEBUG
    printf("Wrote bytes to file\n");
    // Save the database to a file
    printf("Database saved to file.\n");
#endif
}

/* 
verify_password
This function verifies the password provided by the user by decrypting the stored database 
and checking its integrity. If successful, it grants access to the database.

Parameters:
    - o_password: The password to verify.
    - o_db_name: The name of the database file.
*/
int verify_password(char *o_password, char *o_db_name)
{
#ifdef DEBUG
    printf("Verifying password...\n");
#endif
    if (db != NULL)
    {
        close_connection(db);
    }
#ifdef DEBUG
    printf("Freed db\n");
#endif
    if (password != NULL)
    {
        free(password);
    }

#ifdef DEBUG
    printf("Freed password\n");
#endif

    password = malloc(strlen(o_password) * sizeof(char));
    strcpy(password, o_password);

#ifdef DEBUG
    printf("Password copied\n");
#endif
    if (db_name != NULL)
    {
        free(db_name);
    }
    db_name = malloc(strlen(o_db_name) * sizeof(char));
    strcpy(db_name, o_db_name);

#ifdef DEBUG
    printf("DB name copied\n");
#endif

    FILE *file = fopen(db_name, "rb");
    if (!file)
    {
        printf("Error: Could not open file for reading.\n");
        return 0;
    }

    if (salt != NULL)
    {
        free(salt);
    }
    salt = malloc((SALT_LEN + 1) * sizeof(char));
#ifdef DEBUG
    printf("Salt mallocated\n");
#endif
    if (fread(salt, 1, SALT_LEN, file) != SALT_LEN)
    {
        printf("Error: Could not read salt from file.\n");
        free(salt);
        fclose(file);
        return 0;
    }
#ifdef DEBUG
    printf("Salt read\n");
#endif
    salt[SALT_LEN] = '\0';

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, SALT_LEN, SEEK_SET);
#ifdef DEBUG
    printf("File size: %ld\n", fileSize);
#endif
    int length = fileSize - SALT_LEN;

#ifdef DEBUG
    printf("Length: %d\n", length);
#endif
    if (length <= 0)
    {
        printf("Error: File is empty.\n");
        fclose(file);
        return 0;
    }

    char *ciphertext = malloc((length + 1) * sizeof(char));

#ifdef DEBUG
    printf("Ciphertext mallocated\n");
#endif
    if (fread(ciphertext, 1, length, file) != length)
    {
        printf("Error: Could not read ciphertext from file.\n");
        free(ciphertext);
        fclose(file);
        return 0;
    }
#ifdef DEBUG
    printf("Ciphertext read\n");
#endif
    fclose(file);
    ciphertext[length] = '\0';

    data = malloc(length * sizeof(char));
#ifdef DEBUG
    printf("Plaintext mallocated\n");
#endif
    if (!decrypt_data_salt_pepper(ciphertext, length, password, salt, (char *)data, (int *)&db_size))
    {
        printf("Error: Could not decrypt database.\n");
        return 0;
    }
#ifdef DEBUG
    printf("Decrypted\n");
#endif
    free(ciphertext);

#ifdef DEBUG
    printf("Freed\n");
#endif

    if (!open_connection(&db, DATABASE_CONN))
    {
        printf("Error: Could not open database connection.\n");
        return 0;
    }

#ifdef DEBUG
    FILE *un_file = fopen("unenc.db", "wb");
    fwrite(data, 1, db_size, un_file);
    fclose(un_file);
#endif
    int status = sqlite3_deserialize(db, "main", data, db_size, db_size, SQLITE_DESERIALIZE_FREEONCLOSE);
    if (status != SQLITE_OK)
    {
        printf("Error: Could not deserialize database.\n");
        return 0;
    }

    printf("Password verified. Access granted.\n");

    return 1;
}