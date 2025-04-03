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
    int Choice,Sub_Choice_1,Sub_Choice_2,user_ID;

    while (1) {
        //Navigation Menu
        printf("\n| --- Menu --- |\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Select from Navigation Menu: ");
        scanf("%d", &Choice);
        getchar();

        switch (Choice) {
            case 1:
                registerUser();
                break;
            case 2:
                user_ID=loginUser();
                printf("USER id= %d\n",user_ID);

                int logged_in=1;
                if(user_ID!=-1){
                    while(logged_in){
                        //Dashboard Menu
                        printf("\n| --- Dashboard --- |\n1. Call Log Details\n2. Generate Bill\n3. Report\n4. Ledger\n5. Log Out\nSelect : ");
                        scanf("%d",&Sub_Choice_1);
                        getchar();
                        switch(Sub_Choice_1){
                            case 1:
                                //Call Log Menu
                                printf("\n| --- Call Logs --- |\n");
                                printf("1. Dialed\n");
                                printf("2. Received\n");
                                printf("3. Missed\n");
                                printf("4. Dashboard\n");
                                printf("Select: ");
                                scanf("%d",&Sub_Choice_2);
                                getchar();
                        
                                switch(Sub_Choice_2){
                                    case 1:
                                      //   CallLogDetails(user_ID,Sub_Choice_2);
                                      break;
                                      case 2:
                                         //
                                         break;
                                      case 3:
                                         //
                                         break;
                                      case 4:
                                         break;//return to Dashboard from Call Log Menu
                                      default: 
                                          printf("\n\nInvalid choice! Please try again.\n");
                                      
                                }
                                
                                break;
                            case 2:
                            //    GenerateBill();
                               break;
                            case 3:
                            //    Report();
                               break;   
                            case 4:
                            //    Ledger();
                               break;
                            case 5:
                               printf("\nLogging out...\n");
                               logged_in=0;//return to Main Menu from Dashboard
                               break;
                               
                            default:
                               printf("\n\nInvalid choice! Please try again.\n");
                        }
                    }    
                }
                break;
            case 3:
                printf("\nExiting the program. Goodbye!\n\n");
                return 0;
            default:
                printf("\nInvalid choice! Please try again.\n\n");
        }
    }

    return 0;
}
