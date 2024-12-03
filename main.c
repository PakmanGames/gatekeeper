/**
 * Team 20 - Memory Leakers
 * Ahmed Yassin, 400536694
 * Andy Pak, 400530925
 * Patrick Molka, 400537630
 * Aditya Rao, 400517325
 * 
 * main.c contains the main function for the Password Manager application.
 * It includes functions for testing the data encryption, database, and user interface modules.
 * 
 * Contents:
 * - Main function for the Password Manager application
 * - Testing functions for data encryption, database, and user interface modules
 */
#include <stdio.h>
#include <string.h>
#include "data_encryption.h"
#include "user_interface.h"
#include "database.h"
#define DEBUG 1
#ifdef DEBUG
#include <sqlite3.h>
#include <stdlib.h>
#endif

/**
 * print_bloated
 * This function prints a bloated string.
 * 
 * Parameters:
 * - text: The bloated string to print.
 * - length: The length of the bloated string.
 * 
 * Returns:
 * - void
 */
void print_bloated(char *text, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%c", text[i]);
    }
    printf("\n");
}

/**
 * test_encrypt_decrypt
 * This function tests the encryption and decryption functions.
 * 
 * Parameters:
 * - None
 * 
 * Returns:
 * - 0 if the test was successful.
 * - 1 if the test failed.
 */
int test_encrypt_decrypt()
{
    // Generate a random key
    char password[] = "password";
    unsigned char salt[SALT_LEN];

    // Generate a random salt
    if (!generate_secure_random(SALT_LEN, (char *)salt))
    {
        printf("Failed to generate a random salt\n");
        return 1;
    }

    // Print the generated key and iv
    printf("Generated password: %s\n", password);
    printf("Generated salt: %s\n", salt);

    // Plain text, cipher text, and decrypted text
    char *plaintext = "Hello, World!";
    printf("Plain text: %s\n", plaintext);
    char ciphertext[1024], decryptedtext[1024];

    int decryptedtext_length = 0;
    int ciphertext_length = 0;

    // Encrypt the plaintext
    if (!encrypt_data_salt_pepper(plaintext, strlen(plaintext), password, salt, ciphertext, &ciphertext_length))
    {
        printf("Failed to encrypt the plaintext\n");
        return 1;
    }

    // Print the encrypted text
    printf("Encrypted text: ");
    print_bloated(ciphertext, ciphertext_length);

    // Decrypt the ciphertext
    if (!decrypt_data_salt_pepper(ciphertext, ciphertext_length, password, salt, decryptedtext, &decryptedtext_length))
    {
        printf("Wrong password or data is corrupted\n");
        return 1;
    }

    // Print the decrypted text
    printf("Decrypted text: ");
    print_bloated(decryptedtext, decryptedtext_length);

    // Test modified
    ciphertext[1] = 'a';
    printf("Modified encrypted text: ");
    print_bloated(ciphertext, ciphertext_length);
    decryptedtext_length = 0;
    memset(decryptedtext, 0, 1024);
    if (!decrypt_data_salt_pepper(ciphertext, ciphertext_length, password, salt, decryptedtext, &decryptedtext_length))
    {
        printf("Wrong password or data is corrupted\n");
    }

    return 0;
}

/**
 * test_database_stuff
 * This function tests the database functions.
 * 
 * Parameters:
 * - None
 * 
 * Returns:
 * - 0 if the test was successful.
 * - 1 if the test failed.
 */
int test_database_stuff()
{
    char *database_path = "passwords.db";
    sqlite3 *db = NULL;

    // open connection
    if (!open_connection(&db, database_path))
    {
        printf("uh oh no good");
        return 1;
    }

    // initialize db
    if (!initialize_database(db))
    {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    }

    // test adding a password
    char *name = "Instagram";
    char *password = "Password123";
    if (!add_password(db, name, password))
    {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    }
    else
    {
        printf("Added password for %s where the password is %s\n", name, password);
    }

    // test adding another password
    char *name2 = "Facebook";
    if (!add_password(db, name2, password))
    {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    }
    else
    {
        printf("Added password for %s where the password is %s\n", name2, password);
    }

    // test deleting a password
    if (!delete_password(db, name2))
    {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    }
    else
    {
        printf("Deleted password for %s\n", name2);
    }

    // close connection
    close_connection(db);
    // open connection again just to test
    if (!open_connection(&db, database_path))
    {
        printf("uh oh no good");
        return 1;
    }

    // test getting a password

    struct credentials *result = get_password(db, name);

    if (!result)
    {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    }
    else
    {
        printf("Got password for %s\n", result->name);
        printf("The password is %s\n", result->password);
    }

    // Test updating a password
    if (!update_password(db, name, password, "NewPassword123"))
    {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    }
    else
    {
        printf("Updated password for %s\n", name);
    }

    // Test getting all passwords
    struct credentials_list *list = list_passwords(db);
    if (!list)
    {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    }
    else
    {
        printf("Got all passwords\n");
        for (int i = 0; i < list->length; i++)
        {
            printf("Name: %s\n", list->entries[i].name);
            printf("Password: %s\n", list->entries[i].password);
        }
    }

    return 0;
}

/**
 * test_read_sqlite_from_file
 * This function tests reading an SQLite database from a file.
 * 
 * Parameters:
 * - None
 * 
 * Returns:
 * - 0 if the test was successful.
 * - 1 if the test failed.
 */
int test_read_sqlite_from_file()
{

    FILE *un_file = fopen("unenc.db", "rb");
    fseek(un_file, 0, SEEK_END);
    int length = ftell(un_file);
    fseek(un_file, 0, SEEK_SET);

    char *plaintext = malloc(length * sizeof(char));
    // int plaintext_length = length;
    if (fread(plaintext, 1, length, un_file) != length)
    {
        printf("Error: Could not read plaintext from file.\n");
        free(plaintext);
        fclose(un_file);
        return 0;
    }
    fclose(un_file);

    sqlite3 *db;
#define DATABASE_CONN ":memory:"

    if (!open_connection(&db, DATABASE_CONN))
    {
        printf("Error: Could not open database connection.\n");
        return 0;
    }

    int status = sqlite3_deserialize(db, "main", (unsigned char *)plaintext, length, length, SQLITE_DESERIALIZE_FREEONCLOSE);
    if (status != SQLITE_OK)
    {
        printf("Error: Could not deserialize database. SQLite error code: %d, Message: %s\n", status, sqlite3_errmsg(db));
        return 0;
    }

    // Check if db is valid
    /*if (db != NULL)
    {
        int status_check = sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, NULL, NULL, 0);
        if (status_check == SQLITE_OK)
        {
            printf("Database connection is valid.\n");
        }
        else
        {
            printf("Database connection is not valid.\n");
        }
    }
    else
    {
        printf("Error: db pointer is null.\n");
    }*/

    // Run a simple query to check database validity
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparing statement after deserialization: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Statement prepared successfully.\n");
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    printf("Password verified. Access granted.\n");
    free(plaintext);
    printf("SQLITE verison: %s\n", sqlite3_libversion());
    return 0;
}

/**
 * main
 * The main function for the Password Manager application.
 * 
 * Parameters:
 * - None
 * 
 * Returns:
 * - 0 if the application runs successfully.
 * - 1 if the application encounters an error.
 */
int main()
{
    // return test_database_stuff();
    //  return test_encrypt_decrypt();
    // return test_read_sqlite_from_file();
    return not_main();
}
