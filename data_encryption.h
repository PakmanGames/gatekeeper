// Encrypts the plaintext using the key and iv provided
// Returns 1 on success, 0 on failure
int encrypt(char *plaintext, int plaintext_length, char *key, char *iv, char *ciphertext, int *ciphertext_len);
// Decrypts the ciphertext using the key and iv provided
// Returns 1 on success, 0 on failure
int decrypt(char *ciphertext, int ciphertext_length, char *key, char *iv, char *plaintext, int *plaintext_length);
// Generates a random key of the specified length
// Returns 1 on success, 0 on failure
int generate_secure_random(int length, char random[length]);
