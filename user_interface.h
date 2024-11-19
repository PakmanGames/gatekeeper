//Header file for user_interface.c

#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50 //Define max length for database name
#define MAX_PASSWORD_LENGTH 50 //Define max length for database password
#define MIN_PASSWORD_LENGTH 8 //Define min length for database password

//Function to display usage instructions
void usage();

//Function to display help information
void help();

//Function to check if a password meets the minimum length requirement
int is_password_valid(const char *password);

//Placeholder function to verify the database password (yet to be implemented)
int verify_password();

//Function to display application view after successful login or database creation
//User can access existing stored passwords, create a new password to store, change existing password, or change top-level password for the database
void app_view();

#endif
