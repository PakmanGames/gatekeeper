# GateKeeper
CLI-based password manager written in C, which incorporates encryption and an SQLite database.

This project was created by myself and my wonderful teammates: [Patrick](https://github.com/PatrMo), [Ahmed](https://github.com/ahyassin15), and [Aditya](https://github.com/adityarao2005).

This was the culmination of what we learned in the 2XC3 Development Basics course. We wrote a modular password manager, followed propper Linux-style conventions for the usage and help flags. Tested our code using `gcov` and had a GitHub Actions pipeline to continuously test as we integrated new features.

In addition to what we had learned in the course, we incorporated the OpenSSL library to handle the encryption as well as the SQLite3 library to create a database which is safely encrypted.

### How to use the application
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

**More detail on how to use it with `gatekeeper --help`**

### Technologies
<p align="center">
    <img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white">
    <img src="https://img.shields.io/badge/Sqlite-003B57?style=for-the-badge&logo=sqlite&logoColor=white">
    <img src="https://img.shields.io/badge/GIT-E44C30?style=for-the-badge&logo=git&logoColor=white">
    <img src="https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white">
    <img src="https://img.shields.io/badge/GitHub_Actions-2088FF?style=for-the-badge&logo=github-actions&logoColor=white">
</p>

### Dependencies
```bash
sudo apt-get update
sudo apt-get install libssl-dev
sudo apt-get install libsqlite3-dev
```

### Resources used
**References**: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
