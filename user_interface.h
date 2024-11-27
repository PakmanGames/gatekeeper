//Header file for user_interface.c

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
