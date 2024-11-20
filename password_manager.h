// password_manager.h

#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <stdio.h>
#include <string.h>

//Define max number of entries allowed in database and size constraints
#define MAX_PASS_ENTRIES 100
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

//Structure to store each password entry with a service name and password
typedef struct {
    char service[MAX_NAME_LENGTH]; //Store the service name
    char password[MAX_PASSWORD_LENGTH]; //Store the associated password
} PasswordEntry;

//External declaration of array to store password enteries for password database
extern PasswordEntry database[MAX_PASS_ENTRIES];

//External declaration of password entry count
extern int num_pass_entries;

//Function to access existing stored passwords
void access_password();

//Function to create and store a new password
void create_password();

//Function to change an existing password
void change_password();

//Function to change the top-level password of the database
void change_top_level_password();

// //Function to verify password validity (placeholder and yet to be implemented)
// int is_password_valid(const char *password);

// // Function to verify the current top-level password (placeholder and yet to be implemented)
// int verify_password();

#endif