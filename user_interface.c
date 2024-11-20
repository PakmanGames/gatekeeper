//User interface for password manager app, GateKeeper

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Function to display usage instructions
void usage() {
    printf("GateKeeper App Usage:\n");
    printf("1 - Select an existing password database\n");
    printf("2 - Create a new password database\n");
    printf("Enter '--help' for more information.\n");
}

//Function to display help information
void help() {
    printf("Help - GateKeeper Password Manager:\n");
    printf("This application allows you to manage your password database securely.\n");
    printf("Options:\n");
    printf("1 - Select an existing database: Requires database name and password.\n");
    printf("2 - Create a new database: Prompts you for a new database name and password.\n");
    printf("Password Requirements: Minimum length of %d characters.\n", MIN_PASSWORD_LENGTH);
}


//Check password length
int is_password_valid(const char *password) {
    
    //If the length of the password is less than the minimum required character length
    if (strlen(password) < MIN_PASSWORD_LENGTH) {
        printf("Password must be at least %d characters long.\n", MIN_PASSWORD_LENGTH);  //Print password is not long enough
        return 0; //If invalud, return 0
    }

    return 1; //If valid, return 1
}


//Verify DB password (placeholder for now)
int verify_password() {
    printf("Getting password right now...\n");
    return 1; // To assume password valid, return 1. Return 0 to make it invalid
}


//Display app view upon successful entry (through successful login or database creation)
//User can access existing stored passwords, create a new password to store, change existing password, or change top-level password for the database
void app_view() {
    printf("Welcome to your password manager!\n");
    
    // Additional app functionalities to be added 
    
}


//Main function
int main() {

    //Declare variables for user choice, database name, password, and input buffer
    int choice;
    char db_name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char input[10];

    //Display usage instructions at the start
    usage();

    //Prompt user to enter option 1, 2, or --help
    printf("Enter your choice (1, 2, or --help): ");

    //Get user input as a string to check for --help or number choice
    if (fgets(input, sizeof(input), stdin) != NULL) {

        //If the input matches "--help" then display help info
        if (strncmp(input, "--help", 4) == 0) {
           
            //Call help function
            help();
            //Exit after displaying help
            exit(0);
        }
        
        //Convert the input string to an integer for further processing
        choice = atoi(input);

    //If user input is invalid
    } else {
        printf("Invalid input. Please enter 1, 2, or 'help'.\n"); //Print error
        exit(1); //Exit
    }

    //If choice is 1 then user selected to access an existing database
    if (choice == 1) {

        //Prompt the user for the database name they want to access
        printf("Enter the name of the existing password database you want to access: ");

        //Read the database name and check if it is valid
        if (fgets(db_name, MAX_NAME_LENGTH, stdin) == NULL || db_name[0] == '\n') {
            printf("Invalid database name. Please try again.\n"); //Print invalid message
            exit(1); //Exit if name is invalid
        }
        db_name[strcspn(db_name, "\n")] = '\0'; //Remove newline character from input

        //Prompt the user for the database password
        printf("Enter the password for the database: ");

        //Read the password and check if it is valid
        if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL || password[0] == '\n') {
            printf("Invalid password. Please try again.\n"); //Print invalid message
            exit(1); //Exit if password is invalid
        }
        password[strcspn(password, "\n")] = '\0'; //Remove newline character from input

        //Check if the password meets the minimum length requirement
        if (!is_password_valid(password)) {
            exit(1); //Exit if password is too short
        }

        //Run password verification (with a placeholder function for now)
        if (verify_password()) {
            app_view(); //Enter app view if verifying the password is successful
        } else {
            printf("Incorrect password. Access denied.\n");
            exit(1); //Exit if verifying the password fails
        }


    //If user choice is 2 then user selected to create a new database
    } else if (choice == 2) {

        //Prompt the user for a name for the new database
        printf("Enter a name for the new password database: ");

        //Read the user's database name and check if it is valid
        if (fgets(db_name, MAX_NAME_LENGTH, stdin) == NULL || db_name[0] == '\n') {
            printf("Invalid database name. Please try again.\n"); //Print invalid message
            exit(1); //Exit if name is invalid
        }
        db_name[strcspn(db_name, "\n")] = '\0'; //Remove newline character from input

        //Prompt the user for a password for the new database
        printf("Enter a password for the new database: ");

        //Read the user's password and check if it is valid
        if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL || password[0] == '\n') {
            printf("Invalid password. Please try again.\n"); //Print invalid message
            exit(1); //Exit if password is invalid
        }
        password[strcspn(password, "\n")] = '\0'; //Remove newline character from input

        //Check if the password meets the minimum length requirement
        if (!is_password_valid(password)) {
            exit(1); //Exit if the password is too short
        }

        //Simulate database creation and secure the new database
        printf("Database '%s' created and secured.\n", db_name);
        app_view(); //Enter app view after successful creation

    } else {
        //If the user entered an invalid argument of neither 1,2, or --help
        printf("Invalid choice. Please enter 1, 2, or 'help'.\n"); //Print invalid choice message
        exit(1);
    }

    return 0; //Return 0 indicating successful execution
}