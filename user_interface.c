/**
 * Team 20 - Memory Leakers
 * Ahmed Yassin, 400536694
 * Andy Pak, 400530925
 * Patrick Molka, 400537630
 * Aditya Rao, 400517325
 * 
 * user_interface.c contains functions for the user interface of the Password Manager application.
 * It includes functions for displaying usage instructions, help information, and the application view.
 * 
 * Contents:
 * - Usage instructions and help information functions
 * - Password validation function
 * - Application view function
 * - Main function for the Password Manager application
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface.h"
#include "app_data.h"

// Define max length for database name
#define MAX_NAME_LENGTH 50
// Define min and max length for database password
#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 16

/**
 * usage
 * This function displays the usage instructions for the GateKeeper Password Manager application.
 * 
 * Parameters:
 * - None
 * 
 * Returns:
 * - void
 */
void usage() {
    printf("Usage: gatekeeper [OPTION]\n");
    printf("Manage your password databases with GateKeeper.\n\n");
    printf("Options:\n");
    printf("  [NONE]      Start gatekeeper application\n");
    printf("  --help      Display this help and exit\n");
    printf("GateKeeper App Usage:\n");    
    printf("1 - Select an existing password database\n");
    printf("2 - Create a new password database\n");
}

/**
 * help
 * This function displays help information for the GateKeeper Password Manager application.
 * 
 * Parameters:
 * - None
 * 
 * Returns:
 * - void
 */
void help() {
    printf("GateKeeper Password Manager - Help\n\n");
    printf("Usage:\n");
    printf("  gatekeeper [OPTION]\n\n");
    printf("Options:\n");
    printf("  [NONE]      Start gatekeeper application\n");
    printf("  --help      Display this help and exit\n\n");
    printf("Application Usage (gatekeeper [NONE]):\n");
    printf("  1           Select an existing password database\n");
    printf("              Requires database name and password.\n");
    printf("              Access stored passwords, add, change, or remove credentials.\n\n");
    printf("  2           Create a new password database\n");
    printf("              Prompts for a new database name and password.\n\n");
    printf("Password Requirements:\n");
    printf("  Minimum length of %d characters.\n", MIN_PASSWORD_LENGTH);
}

/**
 * is_password_valid
 * This function checks if a password meets the minimum length requirement.
 * 
 * Parameters:
 * - password: The password to check.
 * 
 * Returns:
 * - 1 if the password is valid.
 * - 0 if the password is invalid.
 */
int is_password_valid(const char *password)
{

    // If the length of the password is less than the minimum required character length
    if (strlen(password) < MIN_PASSWORD_LENGTH)
    {
        printf("Password must be at least %d characters long.\n", MIN_PASSWORD_LENGTH); // Print password is not long enough
        return 0;                                                                       // If invalud, return 0
    }

    return 1; // If valid, return 1
}

/**
 * app_view
 * This function displays the application view for the Password Manager.
 * It allows the user to add, change, or remove credentials, and change the database password.
 * 
 * Parameters:
 * - None
 * 
 * Returns:
 * - void
 */
void app_view() {
    int choice;
    do {
        printf("Welcome to your password manager!\n");

        list_all_credentials(); // Placeholder function to list all credentials

        printf("Select an option:\n");
        printf("1 - Add a new credential\n");
        printf("2 - Change a credential\n");
        printf("3 - Remove credential\n");
        printf("4 - Change the database password\n");
        printf("5 - Exit\n");

        // Get user choice
        char vv[10];
        if (fgets(vv, sizeof(vv), stdin) != NULL) {
            choice = atoi(vv);

            if (choice < 1 || choice > 5) {
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
                continue;
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        // Switch statement to handle user choice
        switch (choice) {
        case 1:
            printf("Adding a new credential...\n");
            add_new_credential();
            break;
        case 2:
            printf("Changing a credential...\n");
            change_credential();
            break;
        case 3:
            printf("Removing a credential...\n");
            remove_credential();
            break;
        case 4:
            printf("Changing the database password...\n");
            change_database_password();
            break;
        case 5:
            printf("Exiting the password manager...\n");
            close_database();
            exit(0);
            return;
        default:
            printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }

        save_database(); // Placeholder function to save the database

    } while (choice != 5); // Loop to keep the app running until user exits
}

/**
 * not_main
 * This function contains the main logic for the Password Manager application.
 * 
 * Parameters:
 * - None
 * 
 * Returns:
 * - 0 if the application runs successfully.
 * - 1 if the application encounters an error.
 */
int not_main() {
    // Declare variables for user choice, database name, password, and input buffer
    int choice;
    char db_name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char input[10];

    // Display usage instructions at the start
    usage();

    // Prompt user to enter option 1, 2, or --help
    printf("Enter your choice (1 or 2): ");

    // Get user input as a string to check for --help or number choice
    if (fgets(input, sizeof(input), stdin) != NULL) {
        // Convert the input string to an integer for choices 1 and 2
        choice = atoi(input);

        
    } else { // If user input is invalid
        printf("Invalid input. Please enter 1 or 2.\n");
        exit(1);
    }

    // If choice is 1 then user selected to access an existing database
    if (choice == 1) {

        // Prompt the user for the database name they want to access
        printf("Enter the name of the existing password database you want to access: ");

        // Read the database name and check if it is valid
        if (fgets(db_name, MAX_NAME_LENGTH, stdin) == NULL || db_name[0] == '\n') {
            printf("Invalid database name. Please try again.\n");
            exit(1);
        }
        db_name[strcspn(db_name, "\n")] = '\0';

        // Prompt the user for the database password
        printf("Enter the password for the database: ");

        // Read the password and check if it is valid
        if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL || password[0] == '\n') {
            printf("Invalid password. Please try again.\n");
            exit(1);
        }
        password[strcspn(password, "\n")] = '\0';

        // Check if the password meets the minimum length requirement
        if (!is_password_valid(password)) {
            exit(1);
        }

        // Run password verification (with a placeholder function for now)
        if (verify_password(password, db_name))
        {
            app_view(); // Enter app view if verifying the password is successful
        }
        else
        {
            fprintf(stderr, "Incorrect password. Access denied.\n");
            exit(1); // Exit if verifying the password fails
        }
       
    } else if (choice == 2) { // If user choice is 2 then user selected to create a new database
        printf("Enter a name for the new password database: ");
        // Read the user's database name and check if it is valid
        if (fgets(db_name, MAX_NAME_LENGTH, stdin) == NULL || db_name[0] == '\n') {
            printf("Invalid database name. Please try again.\n");
            exit(1);
        }
        db_name[strcspn(db_name, "\n")] = '\0';

        printf("Enter a password for the new database: ");
        // Read the user's password and check if it is valid
        if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL || password[0] == '\n') {
            printf("Invalid password. Please try again.\n");
            exit(1);
        }
        password[strcspn(password, "\n")] = '\0';

        // Check if the password meets the minimum length requirement
        if (!is_password_valid(password)) {
            exit(1);
        }

        // Simulate database creation and secure the new database
        printf("Database '%s' created and secured.\n", db_name);
        create_database(password, db_name);
        app_view();
    } else { // If user choice is not 1 or 2
        printf("Invalid choice. Please enter 1 or 2.\n");
        exit(1);
    }

    return 0; // Success
}