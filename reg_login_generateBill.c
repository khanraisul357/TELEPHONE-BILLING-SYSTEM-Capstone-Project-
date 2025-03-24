#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include<conio.h>

// Struct definition
struct userInfo {
    char username[50];
    char email[50];
    char password[50];
    char phone[15];
    char address[100];
    float balance;
} user, store;

// File paths for storing user information
const char *textFilePath = "UserInformation.txt"; 
const char *binaryFilePath = "UserInformation.dat";

// UDF for Password validation Functionality
int isPasswordValid(char *password) {
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    for (int i = 0; password[i]; i++) {
        if (isupper(password[i])) 
            hasUpper = 1;
        else if (islower(password[i])) 
            hasLower = 1;
        else if (isdigit(password[i])) 
            hasDigit = 1;
        else 
            hasSpecial = 1;
    }
    return hasUpper && hasLower && hasDigit && hasSpecial && strlen(password) >= 8;
}

// UDF for Set Password Functionality
void maskPassword(char *password) {
    char ch;
    int i = 0;
    
    printf("Enter password: ");
    while ((ch = getch())!= '\r') { // Ensure '\r' (carriage return) is handled correctly.
        if (ch == '\b' && i > 0) { // Backspace handling
            printf("\b \b");
            i--;
        } else if (isprint(ch) && i < 49) { // Limit password length to 49
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
}

// UDF for Registration Functionality
void registerUser() {
    FILE *file = fopen(textFilePath, "a");

    if (file == NULL) { // Error handling for file opening
        printf("Error: Unable to open or create the file '%s'.\n", textFilePath);
        printf("Ensure your application has proper permissions.\n");
        return;
    }

    printf("\n| --- Register --- |\n");
    printf("Enter username: ");
    scanf("%s", user.username);
    printf("Enter phone number: ");
    scanf("%s", user.phone);
    printf("Enter email: ");
    scanf("%s", user.email);
    
    do {
        maskPassword(user.password); // Input password securely
        if (!isPasswordValid(user.password)) {
            printf("\nPassword must be at least 8 characters with upper/lowercase, number, and special character.\n");
        }
    } while (!isPasswordValid(user.password));

    fprintf(file, "User Name: %s\nPhone: %s\nEmail: %s\nPassword: %s\n\n", user.username, user.phone, user.email, user.password);
    fclose(file);

    printf("\nRegistration successful!\n");

    // Save user information in the binary file for future updates
    FILE *binaryFile = fopen(binaryFilePath, "ab");
    user.balance = 0.0;
    fwrite(&user, sizeof(struct userInfo), 1, binaryFile);
    fclose(binaryFile);
}

// UDF for Log in Functionality
void loginUser() {
    char email[50], password[50];
    int found = 0;

    printf("\n| --- Login --- |\n");
    printf("Enter email: ");
    scanf("%s", email);
    maskPassword(password);

    FILE *file = fopen(textFilePath, "r");
    if (file == NULL) { // Error handling for file opening
        printf("\nNo users registered yet.\n");
        return;
    }

    while (fscanf(file, "User Name: %[^\n]\nPhone: %[^\n]\nEmail: %s\nPassword: %s\n", user.username, user.phone, user.email, user.password) != EOF) { 
        // fscanf format to match file content
        if (strcmp(email, user.email) == 0 && strcmp(password, user.password) == 0) {
            found = 1;
            break;
        }
    }
    fclose(file);

    if (found) {
        printf("\nLogin successful. Welcome, %s!\n", user.username);
    } else {
        printf("\nInvalid email or password.\n");
    }
}

// UDF for Generate Bill and calculation Functionality
void generateBill(char phone[]) {
    FILE *file = fopen(binaryFilePath, "r+b"); // use .dat for binary access

    if (file == NULL) { // Error handling for file access
        printf("Error: Unable to open or create the file '%s'.\n", binaryFilePath);
        printf("Ensure your application has proper permissions or the file exists.\n");
        return;
    }

    float callDuration, rate = 1.5, billAmount;
    int found = 0;

    while (fread(&store, sizeof(struct userInfo), 1, file)) {
        if (strcmp(store.phone, phone) == 0) {
            found = 1;

            printf("Enter call duration (in minutes): ");
            scanf("%f", &callDuration);
            
            billAmount = callDuration * rate;
            store.balance += billAmount;

            fseek(file, -sizeof(struct userInfo), SEEK_CUR); // Move the file pointer back to overwrite
            fwrite(&store, sizeof(struct userInfo), 1, file);

            printf("Bill Generated: BDT %.2f\n", billAmount);
            printf("Updated Balance: BDT %.2f\n", store.balance);
            break;
        }
    }
    if(!found) {
        printf("Error: Phone number not found.\n");
    }
    fclose(file);
}

// Main Function
int main() {
    int choice;
    char phone[15];

    while (1) {
        // Navigation Menu
        printf("\n| --- Menu --- |\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Generate Bill\n");
        printf("4. Exit\n");
        printf("Select from Navigation Menu: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                printf("Enter phone number: ");
                scanf("%s", phone);
                generateBill(phone);
                break;
            case 4:
                printf("Exiting the program. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}