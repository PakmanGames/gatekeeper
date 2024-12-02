/* 
Team 20 - Memory Leakers
Ahmed Yassin, 400536694
Andy Pak, 400530925
Patrick Molka, 400537630
Aditya Rao, 400517325

data_encryption.c contains functions for securely encrypting and decrypting data using the AES-256-CBC encryption algorithm. 
It includes additional utilities for generating secure random values and deriving cryptographic keys and 
initialization vectors (IVs) from passwords using PBKDF2 (HMAC-SHA256). 

The encryption process ensures data confidentiality by converting plaintext 
into ciphertext and vice versa. The file also supports enhanced security 
through a salt and pepper approach, where additional salt and a magic 
number are used to further protect the data during encryption.

Contents:
- AES-256-CBC encryption and decryption functions
- Key and IV generation using PBKDF2
- Secure random number generation
- Salt and pepper encryption for additional security
- Magic number verification during decryption
*/

#include "data_encryption.h"
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <string.h>

/* 
encrypt
This function encrypts the given plaintext using AES-256-CBC encryption. 
It initializes the encryption operation, encrypts the plaintext, and returns the ciphertext.

Parameters:
    - plaintext: The text to be encrypted.
    - plaintext_length: The length of the plaintext.
    - key: The encryption key used.
    - iv: The initialization vector for the encryption.
    - ciphertext: The resulting encrypted text.
    - ciphertext_len: The length of the resulting ciphertext.
*/
int encrypt(char *plaintext, int plaintext_length, const unsigned char *key, const unsigned char *iv, char *ciphertext, int *ciphertext_len)
{

    // Declare the cipher context
    EVP_CIPHER_CTX *ctx;
    int len;

    // Create the cipher context
    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        // Return 0 to indicate failure
        return 0;
    }

    // Initialize the encryption operation
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    {
        // Return 0 to indicate failure
        return 0;
    }

// Encrypt the plaintext
#ifdef DEBUG
    printf("Encrypting data...\n");
#endif

    if (1 != EVP_EncryptUpdate(ctx, (unsigned char *)ciphertext, &len, (const unsigned char *)plaintext, plaintext_length))
    {
        // Return 0 to indicate failure
        return 0;
    }

    *ciphertext_len = len;

#ifdef DEBUG
    printf("Encrypted data length: %d\n", len);
#endif

    if (1 != EVP_EncryptFinal_ex(ctx, (unsigned char *)ciphertext + len, &len))
    {
        // Return 0 to indicate failure
        return 0;
    }

    // Add the length of the final block to the ciphertext length
    *ciphertext_len += len;

#ifdef DEBUG
    printf("Encrypted data length: %d\n", len);
#endif

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Return 1 to indicate success
    return 1;
}

/* 
decrypt
This function decrypts the given ciphertext using AES-256-CBC decryption. 
It initializes the decryption operation, decrypts the ciphertext, and returns the plaintext.

Parameters:
    - ciphertext: The text to be decrypted.
    - ciphertext_length: The length of the ciphertext.
    - key: The decryption key used.
    - iv: The initialization vector for decryption.
    - plaintext: The resulting decrypted text.
    - plaintext_length: The length of the resulting plaintext.
*/
int decrypt(char *ciphertext, int ciphertext_length, const unsigned char *key, const unsigned char *iv, char *plaintext, int *plaintext_length)
{

    // Declare the cipher context
    EVP_CIPHER_CTX *ctx;
    int len;

    // Create the cipher context
    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        // Return 0 to indicate failure
        return 0;
    }

    // Initialize the decryption operation
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    {
        // Return 0 to indicate failure
        return 0;
    }

    if (1 != EVP_DecryptUpdate(ctx, (unsigned char *)plaintext, &len, (const unsigned char *)ciphertext, ciphertext_length))
    {
        // Return 0 to indicate failure
        return 0;
    }

    *plaintext_length = len;

    if (1 != EVP_DecryptFinal_ex(ctx, (unsigned char *)plaintext + len, &len))
    {
        // Return 0 to indicate failure
        return 0;
    }

    // Add the length of the final block to the plaintext length
    *plaintext_length += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Return 1 to indicate success
    return 1;
}

/* 
generate_secure_random
This function generates a cryptographically secure random value of a specified length. 
It uses the OpenSSL random number generator to ensure high entropy.

Parameters:
    - length: The length of the random value to generate.
    - random: The buffer to store the generated random value.
*/
int generate_secure_random(int length, char random[length])
{
    // Generate the key
    if (1 != RAND_bytes((unsigned char *)random, length))
    {
        // Return 0 to indicate failure
        return 0;
    }

    // Return 1 to indicate success
    return 1;
}

/* 
generate_key_iv
This function generates a cryptographic key and initialization vector (IV) 
from a password and salt using PBKDF2 with HMAC-SHA256. 

Parameters:
    - password: The user-supplied password to derive the key and IV.
    - salt: A cryptographically secure salt used in the derivation process.
    - key: The derived cryptographic key.
    - iv: The derived initialization vector.
*/
void generate_key_iv(const char *password, const unsigned char *salt,
                     unsigned char *key, unsigned char *iv)
{
    // Derive key and IV using PBKDF2
    unsigned char derived[KEY_LEN + IV_LEN];
    if (!PKCS5_PBKDF2_HMAC(password, strlen(password), salt, SALT_LEN, ITERATIONS,
                           EVP_sha256(), KEY_LEN + IV_LEN, derived))
    {
        fprintf(stderr, "Error: Key derivation failed.\n");
        return;
    }

    // Split derived output into key and IV
    memcpy(key, derived, KEY_LEN);
    memcpy(iv, derived + KEY_LEN, IV_LEN);
}

/* 
encrypt_data_salt_pepper
This function encrypts the given plaintext with additional security 
by adding a salt and a magic number to the data. It derives the key and IV 
from a password and salt before performing the encryption.

Parameters:
    - plaintext: The text to be encrypted.
    - plaintext_length: The length of the plaintext.
    - password: The password used to derive the key and IV.
    - salt: The salt used in the derivation process.
    - ciphertext: The resulting encrypted text.
    - ciphertext_len: The length of the resulting ciphertext.
*/
int encrypt_data_salt_pepper(char *plaintext, int plaintext_length, const char *password, const unsigned char *salt, char *ciphertext, int *ciphertext_len)
{
    // Generate key and IV
    unsigned char key[KEY_LEN], iv[IV_LEN];
    printf("Generating key and IV...\n");
#ifdef DEBUG
    printf("Generating key and IV...\n");
#endif
    generate_key_iv(password, salt, key, iv);
#ifdef DEBUG
    printf("Key and IV generated\n");
    printf("New plain text\n");
#endif

    int new_plaintext_length = MAGIC_NUMBER_LENGTH + plaintext_length;
    char new_plaintext[new_plaintext_length];

#ifdef DEBUG
    printf("Copying...\n");
#endif
    memcpy(new_plaintext, MAGIC_NUMBER, MAGIC_NUMBER_LENGTH);
#ifdef DEBUG
    printf("Copied some\n");
#endif
    memcpy(new_plaintext + MAGIC_NUMBER_LENGTH, plaintext, plaintext_length);
#ifdef DEBUG
    printf("Copied\n");
    printf("Encrypting data with salt and pepper...\n");
#endif
    // Encrypt the plaintext
    return encrypt(new_plaintext, new_plaintext_length, key, iv, ciphertext, ciphertext_len);
}


/* 
decrypt_data_salt_pepper
This function decrypts the given ciphertext and verifies its integrity 
by checking a magic number. It derives the key and IV from the password and salt 
before performing the decryption.

Parameters:
    - ciphertext: The text to be decrypted.
    - ciphertext_length: The length of the ciphertext.
    - password: The password used to derive the key and IV.
    - salt: The salt used in the derivation process.
    - plaintext: The resulting decrypted text.
    - plaintext_length: The length of the resulting plaintext.
*/
int decrypt_data_salt_pepper(char *ciphertext, int ciphertext_length, const char *password, const unsigned char *salt, char *plaintext, int *plaintext_length)
{
    // Generate key and IV
    unsigned char key[KEY_LEN], iv[IV_LEN];
    generate_key_iv(password, salt, key, iv);

    // Decrypt the ciphertext
    int new_plaintext_length;
    char new_plaintext[ciphertext_length];
    if (!decrypt(ciphertext, ciphertext_length, key, iv, new_plaintext, &new_plaintext_length))
    {
        // Return 0 to indicate failure
        return 0;
    }

    if (new_plaintext_length < MAGIC_NUMBER_LENGTH || memcmp(new_plaintext, MAGIC_NUMBER, MAGIC_NUMBER_LENGTH) != 0)
    {
        // Return 2 to indicate invalid password
        return 0;
    }

    *plaintext_length = new_plaintext_length - MAGIC_NUMBER_LENGTH;
    memcpy(plaintext, new_plaintext + MAGIC_NUMBER_LENGTH, *plaintext_length);

    return 1;
}