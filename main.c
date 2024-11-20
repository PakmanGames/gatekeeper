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
    char key[32];
    char iv[16];
    // Generate a random key
    if (!generate_secure_random(32, key))
    {
        printf("Failed to generate a random key\n");
        return 1;
    }

    // Generate a random iv
    if (!generate_secure_random(16, iv))
    {
        printf("Failed to generate a random iv\n");
        return 1;
    }

    // Print the generated key and iv
    printf("Generated key: %s\n", key);
    printf("Generated iv: %s\n", iv);

    // Plain text, cipher text, and decrypted text
    char *plaintext = "Hello, World!";
    printf("Plain text: %s\n", plaintext);
    char ciphertext[1024], decryptedtext[1024];

    int decryptedtext_length = 0;
    int ciphertext_length = 0;

    // Encrypt the plaintext
    if (!encrypt(plaintext, strlen(plaintext), key, iv, ciphertext, &ciphertext_length))
    {
        printf("Failed to encrypt the plaintext\n");
        return 1;
    }

    // Print the encrypted text
    printf("Encrypted text: ");
    print_bloated(ciphertext, ciphertext_length);

    // Decrypt the ciphertext
    if (!decrypt(ciphertext, ciphertext_length, key, iv, decryptedtext, &decryptedtext_length))
    {
        printf("Failed to decrypt the ciphertext\n");
        return 1;
    }

    // Print the decrypted text
    printf("Decrypted text: ");
    print_bloated(decryptedtext, decryptedtext_length);

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
    return 0;
}

int main()
{
    return test_database_stuff();
    // return test_encrypt_decrypt();
}
