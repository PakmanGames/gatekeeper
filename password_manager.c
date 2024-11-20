// password_manager.c

#include "password_manager.h" 

//Array to store password entries
PasswordEntry database[MAX_PASS_ENTRIES]; 
int num_pass_entries = 0; //Initialize the password entry count to 0 (track number of password entries)

//Function to access existing stored passwords
void access_password() {
    char service_name[MAX_NAME_LENGTH]; //Variable to store service name entered by the user
    int found = 0; //Flag to indicate if the service is found

    //Prompt the user for the service name
    printf("Enter the name of the service to retrieve password: ");
    fgets(service_name, MAX_NAME_LENGTH, stdin); //Read service name
    service_name[strcspn(service_name, "\n")] = '\0'; //Remove newline character

    //Search for the password entry in the database
    for (int i = 0; i < num_pass_entries; i++) {
        //Check if the service name matches
        if (strcmp(database[i].service, service_name) == 0) {
            
            //Display the password
            printf("Password for %s: %s\n", service_name, database[i].password);
            
            found = 1; //Set flag to found
            break;
        }
    }

    //If service not found, display error message
    if (!found) { 
        printf("Error: No password found for %s.\n", service_name);
    }
}

//Function to create and store a new password
void create_password() {

    //If the database is full (exceeds max number of allowed password entries)
    if (num_pass_entries >= MAX_PASS_ENTRIES) {
        printf("Error: Password database is full.\n"); //Print error message
        return; //Exit
    }

    char service[MAX_NAME_LENGTH]; //Variable to store the service name
    char password1[MAX_PASSWORD_LENGTH]; //Variable for the first password entry
    char password2[MAX_PASSWORD_LENGTH]; //Variable for the second password entry to confirm password

    //Prompt the user for the service name they want to enter
    printf("Enter the name of the service: ");
    fgets(service, MAX_NAME_LENGTH, stdin); //Read service name
    service[strcspn(service, "\n")] = '\0'; //Remove newline character

    //Prompt the user for the password and confirmation
    printf("Enter the password for %s: ", service);
    fgets(password1, MAX_PASSWORD_LENGTH, stdin); //Read first password
    password1[strcspn(password1, "\n")] = '\0'; //Remove newline character

    printf("Re-enter the password to confirm: ");
    fgets(password2, MAX_PASSWORD_LENGTH, stdin); //Read second password to confirm password
    password2[strcspn(password2, "\n")] = '\0'; //Remove newline character

    //If the password and its confirmation both match and are valid
    if (strcmp(password1, password2) == 0 && is_password_valid(password1)) {
        
        strcpy(database[num_pass_entries].service, service); //Store the service name
        strcpy(database[num_pass_entries].password, password1); //Store the password
        
        //Increment the entry count
        num_pass_entries++;
        
        //Print successful confirmation of password storage
        printf("Password for %s has been stored successfully.\n", service);
    
    //If the password and its confirmation don't match or are invalid, print error message
    } else {
        printf("Error: Passwords do not match or do not meet the requirements.\n");
    }
}

//Function to change an existing password
void change_password() {
    char service[MAX_NAME_LENGTH]; //Variable for service name
    char current_password[MAX_PASSWORD_LENGTH]; //Variable for current password
    char new_password1[MAX_PASSWORD_LENGTH]; //Variable for new password
    char new_password2[MAX_PASSWORD_LENGTH]; //Variable to confirm new password
    int found = 0; //Flag to indicate if the service is found

    //Prompt user for the service name
    printf("Enter the name of the service you want to change the password for: ");
    fgets(service, MAX_NAME_LENGTH, stdin); //Read service name
    service[strcspn(service, "\n")] = '\0'; //Remove newline character

    //Search for the password entry
    for (int i = 0; i < num_pass_entries; i++) {
        //Check if the service name matches
        if (strcmp(database[i].service, service) == 0) {
            found = 1; //Set flag to found

            //Prompt user to enter current password
            printf("Enter the current password for %s: ", service);
            fgets(current_password, MAX_PASSWORD_LENGTH, stdin); //Read current password
            current_password[strcspn(current_password, "\n")] = '\0'; //Remove newline character

            //Verify the current password of the service
            if (strcmp(database[i].password, current_password) == 0) {
                //Prompt user to enter their new password and to confirm it by re-entering it
                printf("Enter the new password: ");
                fgets(new_password1, MAX_PASSWORD_LENGTH, stdin); //Read new password
                new_password1[strcspn(new_password1, "\n")] = '\0'; //Remove newline character

                printf("Re-enter the new password to confirm: ");
                fgets(new_password2, MAX_PASSWORD_LENGTH, stdin); //Read the new password confirmation
                new_password2[strcspn(new_password2, "\n")] = '\0'; //Remove newline character

                //If the password and its confirmation both match and are valid
                if (strcmp(new_password1, new_password2) == 0 && is_password_valid(new_password1)) {
                    
                    //Update password in the database
                    strcpy(database[i].password, new_password1);
                    printf("Password for %s has been updated successfully.\n", service);
                
                //If they dont match or aren't valid, print error message
                } else {
                    printf("Error: Passwords do not match or do not meet the requirements.\n");
                }

            //If user enters wrong current password, print error message
            } else {
                printf("Error: Current password is incorrect.\n");
            }
            break;
        }
    }

    //If password not found, display error message
    if (!found) {
        printf("Error: No password found for %s.\n", service);
    }
}

//Function to change the top-level password of the database
void change_top_level_password() {
    char current_password[MAX_PASSWORD_LENGTH]; //Variable for current top-level password
    char new_password1[MAX_PASSWORD_LENGTH]; //Variable for new top-level password
    char new_password2[MAX_PASSWORD_LENGTH]; //Variable to confirm new top-level password

    //Prompt user to enter the current top-level password of the database
    printf("Enter the current top-level password: ");
    fgets(current_password, MAX_PASSWORD_LENGTH, stdin); //Read current top-level password
    current_password[strcspn(current_password, "\n")] = '\0'; //Remove newline character

    //Placeholder for actual top-level password verification (replace with actual check)
    if (verify_password()) {
        
        //Prompt user for the new top-level password and confirmation (re-enter password)
        printf("Enter the new top-level password: ");
        fgets(new_password1, MAX_PASSWORD_LENGTH, stdin); //Read new top-level password
        new_password1[strcspn(new_password1, "\n")] = '\0'; //Remove newline character

        printf("Re-enter the new top-level password to confirm: ");
        fgets(new_password2, MAX_PASSWORD_LENGTH, stdin); //Read new top-level password confirmation
        new_password2[strcspn(new_password2, "\n")] = '\0'; //Remove newline character

        //If the new top-level passwords match and are valid
        if (strcmp(new_password1, new_password2) == 0 && is_password_valid(new_password1)) {
            //Save the new top-level password (implement later to store it securely)
            printf("Top-level password has been updated successfully.\n");
        
        //If they don't match or invalid, print error message
        } else {
            printf("Error: Passwords do not match or do not meet the requirements.\n");
        }
    
    //If user enters wrong current top-level password, print error message
    } else {
        printf("Error: Current top-level password is incorrect.\n");
    }
}
