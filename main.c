#include <stdio.h>
#include <string.h>
#include "data_encryption.h"

#include <sqlite3.h>
#include "database.h"

void print_bloated(char *text, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%c", text[i]);
    }
    printf("\n");
}

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

// testing function to test database functions
int test_database_stuff() {
    char *database_path = "passwords.db";
    sqlite3 *db = NULL;

    // open connection
    if (!open_connection(&db, database_path)) {
        printf("uh oh no good");
        return 1;
    }
    
    // initialize db
    if (!initialize_database(db)) {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    }

    // test adding a password
    char *name = "Instagram";
    char *password = "Password123";
    if (!add_password(db, name, password)) {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    } else {
        printf("Added password for %s where the password is %s\n", name, password);
    }
    
    // test adding another password
    char *name2 = "Facebook";
    if (!add_password(db, name2, password)) {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    } else {
        printf("Added password for %s where the password is %s\n", name2, password);
    }

    // test deleting a password
    if (!delete_password(db, name2)) {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    } else {
        printf("Deleted password for %s\n", name2);
    }

    // close connection
    close_connection(db);
    // open connection again just to test
    if (!open_connection(&db, database_path)) {
        printf("uh oh no good");
        return 1;
    }

    // test getting a password

    struct credentials *result = get_password(db, name);

    if (!result) {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    } else {
        printf("Got password for %s\n", result->name);
        printf("The password is %s\n", result->password);
    }

    // Test updating a password
    if (!update_password(db, name, password, "NewPassword123")) {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    } else {
        printf("Updated password for %s\n", name);
    }

    // Test getting all passwords
    struct credentials_list *list = list_passwords(db);
    if (!list) {
        printf("uh oh no good");
        close_connection(db);
        return 1;
    } else {
        printf("Got all passwords\n");
        for (int i = 0; i < list->length; i++) {
            printf("Name: %s\n", list->entries[i].name);
            printf("Password: %s\n", list->entries[i].password);
        }
    }

    return 0;
}

int main()
{
    return test_database_stuff();
    // return test_encrypt_decrypt();
}
