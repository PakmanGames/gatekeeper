#include <stdio.h>
#include <string.h>
#include "data_encryption.h"

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

int main()
{
    return test_encrypt_decrypt();
}
