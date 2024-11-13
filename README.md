# The Team Assignment for the Memory Leakers: GateKeeper

It uses OpenSSL, C, and potentially bash to create a password manager.
Workflow:

1. Open application in command line and choose to either create a database or open an existing one
2. If choosing to create a database
   1. Inputs password and storage location
   2. Creates a salt, a key, and an initialization vector from randomly using OpenSSL
   3. Write the file layout structure and add all of the values into the file
3. If choosing to open an existing one
   1. Input password and storage location
   2. Attempt to decrypt the information with the salt, key, and initialization vector
   3. If decrypted successfully go to 5, otherwise exit
4. Otherwise exit
5. Allows the user to view, add, remove, and modify existing credentials in the database and stored in an encrypted fashion 

**References**: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption


The first few bytes in the decrypted encrypted data should also be the magic number
