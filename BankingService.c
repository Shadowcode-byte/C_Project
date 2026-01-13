/* 
WAP to display the banking service with the following options:
1) Enquiry
2) Open Account
3) Close Account
4) Deposit Money
5) Withdraw Money
6) Display Balance
7) Display Transaction
8) Identify Defaulter
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Account {
    int acc_no;
    char name[100];
    float balance;
};

void enquiry();
void openAccount();
void closeAccount();
void depositMoney();
void withdrawMoney();
void displayBalance();
void displayTransaction();
void identifyDefaulter();

int main() {
    int choice;

    while (1) {
        printf("\n========= BANKING SERVICE =========\n");
        printf("1) Enquiry\n");
        printf("2) Open Account\n");
        printf("3) Close Account\n");
        printf("4) Deposit Money\n");
        printf("5) Withdraw Money\n");
        printf("6) Display Balance\n");
        printf("7) Display Transaction\n");
        printf("8) Identify Defaulter\n");
        printf("9) Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: enquiry(); break;
            case 2: openAccount(); break;
            case 3: closeAccount(); break;
            case 4: depositMoney(); break;
            case 5: withdrawMoney(); break;
            case 6: displayBalance(); break;
            case 7: displayTransaction(); break;
            case 8: identifyDefaulter(); break;
            case 9:
                printf("Thank you for using our Banking Service!\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}


void enquiry() {
    printf("\n--- BANK ENQUIRY ---\n");
    printf("We offer the following services:\n");
    printf("1. Savings Account\n");
    printf("2. Current Account\n");
    printf("3. Fixed Deposit\n");
    printf("4. Online & Mobile Banking\n");
    printf("For more details, visit our nearest branch.\n");
}

void openAccount() {
    struct Account acc;
    FILE *fp;

    fp = fopen("bank_records.txt", "ab"); 
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n--- OPEN NEW ACCOUNT ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &acc.acc_no);
    getchar(); 

    printf("Enter Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0'; 

    printf("Enter Initial Deposit: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);

    printf("Account created successfully for %s!\n", acc.name);
}


void closeAccount() {
    struct Account acc;
    FILE *fp, *temp;
    int acc_no, found = 0;

    fp = fopen("bank_records.txt", "rb");
    temp = fopen("temp.txt", "wb");

    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Account Number to close: ");
    scanf("%d", &acc_no);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no != acc_no) 
            fwrite(&acc, sizeof(acc), 1, temp);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove("bank_records.txt");
    rename("temp.txt", "bank_records.txt");

    if (found)
        printf("Account %d closed successfully!\n", acc_no);
    else
        printf("Account not found!\n");
}

void depositMoney() {
    struct Account acc;
    FILE *fp;
    int acc_no, found = 0;
    float amount;

    fp = fopen("bank_records.txt", "rb+");
    if (fp == NULL) {
        printf("No account records found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &acc_no);
    printf("Enter Amount to Deposit: ");
    scanf("%f", &amount);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            acc.balance += amount;
            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            printf("₹%.2f deposited successfully!\n", amount);
            printf("Updated Balance: ₹%.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

void withdrawMoney() {
    struct Account acc;
    FILE *fp;
    int acc_no, found = 0;
    float amount;

    fp = fopen("bank_records.txt", "rb+");
    if (fp == NULL) {
        printf("No account records found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &acc_no);
    printf("Enter Amount to Withdraw: ");
    scanf("%f", &amount);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            if (acc.balance >= amount) {
                acc.balance -= amount;
                fseek(fp, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, fp);
                printf("Withdrawal Successful! Remaining Balance: ₹%.2f\n", acc.balance);
            } else {
                printf("Insufficient Balance!\n");
            }
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

void displayBalance() {
    struct Account acc;
    FILE *fp;
    int acc_no, found = 0;

    fp = fopen("bank_records.txt", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            printf("\nAccount Holder: %s\n", acc.name);
            printf("Account Balance: ₹%.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

void displayTransaction() {
    struct Account acc;
    FILE *fp;
    int count = 0;

    fp = fopen("bank_records.txt", "rb");
    if (fp == NULL) {
        printf("No transaction records found!\n");
        return;
    }

    printf("\n--- ALL ACCOUNT DETAILS ---\n");
    printf("Acc.No\tName\t\tBalance (₹)\n");
    printf("-------------------------------------------\n");

    while (fread(&acc, sizeof(acc), 1, fp)) {
        printf("%d\t%-15s\t%.2f\n", acc.acc_no, acc.name, acc.balance);
        count++;
    }

    if (count == 0)
        printf("No accounts found!\n");
    else
        printf("\nTotal Accounts: %d\n", count);

    fclose(fp);
}


void identifyDefaulter() {
    struct Account acc;
    FILE *fp;
    int found = 0;

    fp = fopen("bank_records.txt", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("\n--- DEFAULTERS (Balance below ₹500) ---\n");
    printf("Acc.No\tName\t\tBalance (₹)\n");
    printf("-------------------------------------------\n");

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.balance < 500) {
            printf("%d\t%-15s\t%.2f\n", acc.acc_no, acc.name, acc.balance);
            found = 1;
        }
    }

    if (!found)
        printf("No defaulters found!\n");

    fclose(fp);
}
