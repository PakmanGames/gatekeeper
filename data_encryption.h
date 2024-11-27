#define SALT_LEN 16 // 128 bits
#define KEY_LEN 32  // 256 bits
#define IV_LEN 16   // 128 bits
#define ITERATIONS 100000
#define MAGIC_NUMBER "GOON"
#define MAGIC_NUMBER_LENGTH 4

// Encrypts the plaintext using the key and iv provided
// Returns 1 on success, 0 on failure
int encrypt(char *plaintext, int plaintext_length, const unsigned char *key, const unsigned char *iv, char *ciphertext, int *ciphertext_len);
// Decrypts the ciphertext using the key and iv provided
// Returns 1 on success, 0 on failure
int decrypt(char *ciphertext, int ciphertext_length, const unsigned char *key, const unsigned char *iv, char *plaintext, int *plaintext_length);
// Generates a random key of the specified length
// Returns 1 on success, 0 on failure
int generate_secure_random(int length, char random[length]);
// Generates an iv and key from password and salt
// Returns 1 on success, 0 on failure
void generate_key_iv(const char *password, const unsigned char *salt,
                     unsigned char *key, unsigned char *iv);

// Encrypts the plaintext using the password and salt provided
// Returns 1 on success, 0 on failure
int encrypt_data_salt_pepper(char *plaintext, int plaintext_length, const char *password, const unsigned char *salt, char *ciphertext, int *ciphertext_len);
// Decrypts the ciphertext using the password and salt provided
// Returns 1 on success, 0 on failure
int decrypt_data_salt_pepper(char *ciphertext, int ciphertext_length, const char *password, const unsigned char *salt, char *plaintext, int *plaintext_length);