#include "data_encryption.h"
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <string.h>

int encrypt(char *plaintext, int plaintext_length, char *key, char *iv, char *ciphertext, int *ciphertext_len)
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
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (const unsigned char *)key, (const unsigned char *)iv))
    {
        // Return 0 to indicate failure
        return 0;
    }

    if (1 != EVP_EncryptUpdate(ctx, (unsigned char *)ciphertext, &len, (const unsigned char *)plaintext, plaintext_length))
    {
        // Return 0 to indicate failure
        return 0;
    }

    *ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, (unsigned char *)ciphertext + len, &len))
    {
        // Return 0 to indicate failure
        return 0;
    }

    // Add the length of the final block to the ciphertext length
    *ciphertext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Return 1 to indicate success
    return 1;
}

int decrypt(char *ciphertext, int ciphertext_length, char *key, char *iv, char *plaintext, int *plaintext_length)
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
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (const unsigned char *)key, (const unsigned char *)iv))
    {
        // Return 0 to indicate failure
        return 0;
    }

    if (1 != EVP_DecryptUpdate(ctx, (unsigned char*)plaintext, &len, (const unsigned char*) ciphertext, ciphertext_length))
    {
        // Return 0 to indicate failure
        return 0;
    }

    *plaintext_length = len;

    if (1 != EVP_DecryptFinal_ex(ctx, (unsigned char*) plaintext + len, &len))
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

int generate_secure_random(int length, char random[length])
{
    // Generate the key
    if (1 != RAND_bytes((unsigned char*) random, length))
    {
        // Return 0 to indicate failure
        return 0;
    }

    // Return 1 to indicate success
    return 1;
}
