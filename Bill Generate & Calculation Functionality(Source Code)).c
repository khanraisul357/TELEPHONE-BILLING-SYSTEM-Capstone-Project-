#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include<conio.h>

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
