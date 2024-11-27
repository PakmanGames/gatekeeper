#include "data_encryption.h"
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <string.h>

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

int encrypt_data_salt_pepper(char *plaintext, int plaintext_length, const char *password, const unsigned char *salt, char *ciphertext, int *ciphertext_len)
{
    // Generate key and IV
    unsigned char key[KEY_LEN], iv[IV_LEN];
    generate_key_iv(password, salt, key, iv);

    int new_plaintext_length = MAGIC_NUMBER_LENGTH + plaintext_length;
    char new_plaintext[new_plaintext_length];

    memcpy(new_plaintext, MAGIC_NUMBER, MAGIC_NUMBER_LENGTH);
    memcpy(new_plaintext + MAGIC_NUMBER_LENGTH, plaintext, plaintext_length);
    // Encrypt the plaintext
    return encrypt(new_plaintext, new_plaintext_length, key, iv, ciphertext, ciphertext_len);
}

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