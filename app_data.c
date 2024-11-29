#include "database.h"
#include "data_encryption.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>

sqlite3 *db;
#define DATABASE_CONN ":memory:"
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define DEBUG 1
char *password;
unsigned char *salt;
char *db_name;

#define DEBUG 1

void create_database(char *o_password, char *o_db_name)
{
    int status = open_connection(&db, DATABASE_CONN);
    password = malloc(MAX_PASSWORD_LENGTH * sizeof(char));
    salt = malloc(SALT_LEN * sizeof(char));
    db_name = malloc(MAX_NAME_LENGTH * sizeof(char));
    strcpy(password, o_password);
    strcpy(db_name, o_db_name);
    generate_secure_random(SALT_LEN, (char *)salt);

    if (!status)
    {
        printf("Error: Could not open database connection.\n");
        return;
    }

    status = initialize_database(db);
    if (!status)
    {
        printf("Error: Could not initialize database.\n");
        close_connection(db);
        return;
    }
}

void close_database()
{
    close_connection(db);
    free(password);
    free(db_name);
    free(salt);
}

void list_all_credentials()
{
    struct credentials_list *list = list_passwords(db);
    if (!list)
    {
        printf("Error: Could not retrieve credentials.\n");
        return;
    }

    printf("Listing all credentials:\n");
    for (int i = 0; i < list->length; i++)
    {
        printf("%d) Name: %s, Password: %s\n", i, list->entries[i].name, list->entries[i].password);
    }
}

void add_new_credential()
{
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter the name for the new credential: ");
    if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL || name[0] == '\n')
    {
        printf("Invalid credential name. Please try again.\n");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    printf("Enter the password for the new credential: ");
    if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL || password[0] == '\n')
    {
        printf("Invalid password. Please try again.\n");
        return;
    }
    password[strcspn(password, "\n")] = '\0';

    if (!add_password(db, name, password))
    {
        printf("Error: Could not add credential.\n");
        return;
    }

    printf("Credential added successfully.\n");
}

void remove_credential()
{
    char name[MAX_NAME_LENGTH];

    printf("Enter the name of the credential to remove: ");
    if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL || name[0] == '\n')
    {
        printf("Invalid credential name. Please try again.\n");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    if (!delete_password(db, name))
    {
        printf("Error: Could not remove credential.\n");
        return;
    }

    printf("Credential removed successfully.\n");
}

void change_credential()
{
    char name[MAX_NAME_LENGTH];
    char old_password[MAX_PASSWORD_LENGTH];
    char new_password[MAX_PASSWORD_LENGTH];

    printf("Enter the name of the credential to change: ");
    if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL || name[0] == '\n')
    {
        printf("Invalid credential name. Please try again.\n");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    printf("Enter the current password for the credential: ");
    if (fgets(old_password, MAX_PASSWORD_LENGTH, stdin) == NULL || old_password[0] == '\n')
    {
        printf("Invalid password. Please try again.\n");
        return;
    }
    old_password[strcspn(old_password, "\n")] = '\0';

    printf("Enter the new password for the credential: ");
    if (fgets(new_password, MAX_PASSWORD_LENGTH, stdin) == NULL || new_password[0] == '\n')
    {
        printf("Invalid password. Please try again.\n");
        return;
    }
    new_password[strcspn(new_password, "\n")] = '\0';

    if (!update_password(db, name, old_password, new_password))
    {
        printf("Error: Could not change credential.\n");
        return;
    }

    printf("Credential changed successfully.\n");
}

void change_database_password()
{
    if (password != NULL)
    {
        free(password);
    }
    password = malloc(MAX_PASSWORD_LENGTH * sizeof(char));

    printf("Enter the new password for the database: ");
    if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL || password[0] == '\n')
    {
        printf("Invalid password. Please try again.\n");
        return;
    }
    password[strcspn(password, "\n")] = '\0';

    printf("Database password changed successfully.\n");
}
void save_database()
{
#ifdef DEBUG
    printf("Saving database to file...\n");
#endif
    sqlite3_int64 db_size;
    unsigned char *data = sqlite3_serialize(db, "main", &db_size, 0);

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
#endif

    char *db_name_unencrypted = malloc(strlen(db_name) + 7 * sizeof(char));
    sprintf(db_name_unencrypted, "%s_unenc", db_name);
    printf("Unencrypted database saved to file: %s\n", db_name_unencrypted);

    FILE *file = fopen(db_name, "wb");
    FILE *un_file = fopen(db_name_unencrypted, "wb");
    if (!file)
    {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    fwrite(salt, 1, SALT_LEN, file);
    fwrite(ciphertext, 1, length, file);
    fwrite(data, 1, db_size, un_file);
    fclose(file);
    fclose(un_file);
    free(ciphertext);

#ifdef DEBUG
    printf("Wrote bytes to file\n");
#endif

    // Save the database to a file
    printf("Database saved to file.\n");
}

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

    char *plaintext = malloc(length * sizeof(char));
#ifdef DEBUG
    printf("Plaintext mallocated\n");
#endif
    int plaintext_length;
    if (!decrypt_data_salt_pepper(ciphertext, length, password, salt, plaintext, &plaintext_length))
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
    fwrite(plaintext, 1, plaintext_length, un_file);
    fclose(un_file);
#endif
    int status = sqlite3_deserialize(db, "main", (unsigned char *)plaintext, plaintext_length, plaintext_length, SQLITE_DESERIALIZE_RESIZEABLE);
    if (status != SQLITE_OK)
    {
        printf("Error: Could not deserialize database.\n");
        return 0;
    }

    printf("Password verified. Access granted.\n");
    free(plaintext);

    return 1;
}