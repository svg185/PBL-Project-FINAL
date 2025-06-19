#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_ACCOUNTS 100

typedef struct {
    int acc_no;
    char acc_type[10];
    float balance;
    char name[50];
} Account;

Account accounts[MAX_ACCOUNTS];
int account_count = 0;
FILE *outfp = NULL; // Output file pointer

void printDateTime() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(outfp, "[Date & Time: %02d-%02d-%04d %02d:%02d:%02d]\n",
           t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
           t->tm_hour, t->tm_min, t->tm_sec);
    fflush(outfp);
}

// Print date and time to accounts.txt
void printDateTimeToAccountsFile() {
    FILE *fp = fopen("accounts.txt", "a");
    if (!fp) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(fp, "[Date & Time: %02d-%02d-%04d %02d:%02d:%02d]\n",
           t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
           t->tm_hour, t->tm_min, t->tm_sec);
    fclose(fp);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Save all accounts to accounts.txt (append mode for log/history)
void saveAccountsToFile() {
    FILE *fp = fopen("accounts.txt", "a");
    if (!fp) {
        fprintf(outfp, "Error saving accounts to file!\n");
        return;
    }
    fprintf(fp, "---- Account State ----\n");
    for (int i = 0; i < account_count; i++) {
        fprintf(fp, "%d %s %.2f %s\n", accounts[i].acc_no, accounts[i].acc_type, accounts[i].balance, accounts[i].name);
    }
    fprintf(fp, "-----------------------\n");
    fclose(fp);
}

void printAccountsToConsole() {
    printf("\n--- Current Accounts ---\n");
    for (int i = 0; i < account_count; i++) {
        printf("Account No: %d\n", accounts[i].acc_no);
        printf("Account Type: %s\n", accounts[i].acc_type);
        printf("Account Holder: %s\n", accounts[i].name);
        printf("Balance: %.2f\n", accounts[i].balance);
        printf("----------------------\n");
    }
}

void loadAccountsFromFile() {
    FILE *fp = fopen("accounts.txt", "r");
    account_count = 0;
    if (!fp) return;
    while (fscanf(fp, "%d %s %f %[^\n]", &accounts[account_count].acc_no, accounts[account_count].acc_type, &accounts[account_count].balance, accounts[account_count].name) == 4) {
        account_count++;
        if (account_count >= MAX_ACCOUNTS) break;
    }
    fclose(fp);
}

int generateAccountNo() {
    if (account_count == 0) {
        return 100;
    } else {
        return accounts[account_count - 1].acc_no + 1;
    }
}

void createAccount() {
    if (account_count >= MAX_ACCOUNTS) {
        fprintf(outfp, "Account limit reached!\n");
        printf("Account limit reached!\n");
        fflush(outfp);
        return;
    }
    int choice;
    fprintf(outfp, "\n--- Create Account Menu ---\n");
    fprintf(outfp, "1. Current Account\n");
    fprintf(outfp, "2. Saving Account\n");
    fprintf(outfp, "Enter your choice: ");
    fflush(outfp);
    printf("\n--- Create Account Menu ---\n");
    printf("1. Current Account\n");
    printf("2. Saving Account\n");
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        fprintf(outfp, "Invalid input. Please enter a number.\n");
        printf("Invalid input. Please enter a number.\n");
        fflush(outfp);
        return;
    }
    clearInputBuffer();

    if (choice != 1 && choice != 2) {
        fprintf(outfp, "Invalid choice. Please try again.\n");
        printf("Invalid choice. Please try again.\n");
        fflush(outfp);
        return;
    }

    fprintf(outfp, "Enter account holder name: ");
    fflush(outfp);
    printf("Enter account holder name: ");
    if (!fgets(accounts[account_count].name, sizeof(accounts[account_count].name), stdin)) {
        fprintf(outfp, "Input error.\n");
        printf("Input error.\n");
        fflush(outfp);
        return;
    }
    size_t len = strlen(accounts[account_count].name);
    if (len > 0 && accounts[account_count].name[len - 1] == '\n') {
        accounts[account_count].name[len - 1] = '\0';
    }

    accounts[account_count].acc_no = generateAccountNo();
    strcpy(accounts[account_count].acc_type, (choice == 1) ? "Current" : "Saving");
    accounts[account_count].balance = 0.0f;

    fprintf(outfp, "Creating account for %s...\n", accounts[account_count].name);
    printf("Creating account for %s...\n", accounts[account_count].name);

    fprintf(outfp, "Account created successfully!\n");
    fprintf(outfp, "Account No: %d\n", accounts[account_count].acc_no);
    fprintf(outfp, "Account Type: %s\n", accounts[account_count].acc_type);
    fprintf(outfp, "Account Holder: %s\n", accounts[account_count].name);
    fflush(outfp);

    printf("Account created successfully!\n");
    printf("Account No: %d\n", accounts[account_count].acc_no);
    printf("Account Type: %s\n", accounts[account_count].acc_type);
    printf("Account Holder: %s\n", accounts[account_count].name);

    account_count++;
    printDateTimeToAccountsFile(); // Print date & time after account creation in accounts.txt
    saveAccountsToFile();
    printAccountsToConsole();
}

int findAccount(int acc_no) {
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].acc_no == acc_no)
            return i;
    }
    return -1;
}

void deposit() {
    int acc_no, idx;
    float amt;
    fprintf(outfp, "Enter account number: ");
    fflush(outfp);
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) {
        clearInputBuffer();
        fprintf(outfp, "Invalid input.\n");
        printf("Invalid input.\n");
        fflush(outfp);
        return;
    }
    clearInputBuffer();
    idx = findAccount(acc_no);
    if (idx == -1) {
        fprintf(outfp, "Account not found!\n");
        printf("Account not found!\n");
        fflush(outfp);
        return;
    }
    fprintf(outfp, "Enter amount to deposit: ");
    fflush(outfp);
    printf("Enter amount to deposit: ");
    if (scanf("%f", &amt) != 1) {
        clearInputBuffer();
        fprintf(outfp, "Invalid input.\n");
        printf("Invalid input.\n");
        fflush(outfp);
        return;
    }
    clearInputBuffer();
    if (amt < 0) {
        fprintf(outfp, "Cannot deposit negative amount.\n");
        printf("Cannot deposit negative amount.\n");
        fflush(outfp);
        return;
    }
    accounts[idx].balance += amt;
    fprintf(outfp, "Deposited: %.2f. New Balance: %.2f\n", amt, accounts[idx].balance);
    printf("Deposited: %.2f. New Balance: %.2f\n", amt, accounts[idx].balance);
    fflush(outfp);
    saveAccountsToFile();
    printAccountsToConsole();
}

void withdraw() {
    int acc_no, idx;
    float amt;
    fprintf(outfp, "Enter account number: ");
    fflush(outfp);
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) {
        clearInputBuffer();
        fprintf(outfp, "Invalid input.\n");
        printf("Invalid input.\n");
        fflush(outfp);
        return;
    }
    clearInputBuffer();
    idx = findAccount(acc_no);
    if (idx == -1) {
        fprintf(outfp, "Account not found!\n");
        printf("Account not found!\n");
        fflush(outfp);
        return;
    }
    fprintf(outfp, "Enter amount to withdraw: ");
    fflush(outfp);
    printf("Enter amount to withdraw: ");
    if (scanf("%f", &amt) != 1) {
        clearInputBuffer();
        fprintf(outfp, "Invalid input.\n");
        printf("Invalid input.\n");
        fflush(outfp);
        return;
    }
    clearInputBuffer();
    if (amt < 0) {
        fprintf(outfp, "Cannot withdraw negative amount.\n");
        printf("Cannot withdraw negative amount.\n");
        fflush(outfp);
        return;
    }
    if (amt > accounts[idx].balance) {
        fprintf(outfp, "Insufficient balance!\n");
        printf("Insufficient balance!\n");
    } else {
        accounts[idx].balance -= amt;
        fprintf(outfp, "Withdrawn: %.2f. New Balance: %.2f\n", amt, accounts[idx].balance);
        printf("Withdrawn: %.2f. New Balance: %.2f\n", amt, accounts[idx].balance);
    }
    fflush(outfp);
    saveAccountsToFile();
    printAccountsToConsole();
}

void checkBalance() {
    int acc_no, idx;
    fprintf(outfp, "Enter account number: ");
    fflush(outfp);
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) {
        clearInputBuffer();
        fprintf(outfp, "Invalid input.\n");
        printf("Invalid input.\n");
        fflush(outfp);
        return;
    }
    clearInputBuffer();
    idx = findAccount(acc_no);
    if (idx == -1) {
        fprintf(outfp, "Account not found!\n");
        printf("Account not found!\n");
        fflush(outfp);
        return;
    }
    fprintf(outfp, "Current Balance: %.2f\n", accounts[idx].balance);
    printf("Current Balance: %.2f\n", accounts[idx].balance);
    fflush(outfp);
    saveAccountsToFile();
    printAccountsToConsole();
}

void viewAccount() {
    int acc_no, idx;
    fprintf(outfp, "Enter account number: ");
    fflush(outfp);
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) {
        clearInputBuffer();
        fprintf(outfp, "Invalid input.\n");
        printf("Invalid input.\n");
        fflush(outfp);
        return;
    }
    clearInputBuffer();
    idx = findAccount(acc_no);
    if (idx == -1) {
        fprintf(outfp, "Account not found!\n");
        printf("Account not found!\n");
        fflush(outfp);
        return;
    }
    fprintf(outfp, "Account No: %d\n", accounts[idx].acc_no);
    fprintf(outfp, "Account Type: %s\n", accounts[idx].acc_type);
    fprintf(outfp, "Account Holder: %s\n", accounts[idx].name);
    fprintf(outfp, "Balance: %.2f\n", accounts[idx].balance);
    printf("Account No: %d\n", accounts[idx].acc_no);
    printf("Account Type: %s\n", accounts[idx].acc_type);
    printf("Account Holder: %s\n", accounts[idx].name);
    printf("Balance: %.2f\n", accounts[idx].balance);
    fflush(outfp);
    saveAccountsToFile();
    printAccountsToConsole();
}

void displayAllAccounts() {
    if (account_count == 0) {
        fprintf(outfp, "No accounts to display.\n");
        printf("No accounts to display.\n");
        fflush(outfp);
        return;
    }
    fprintf(outfp, "\n--- All Accounts ---\n");
    printf("\n--- All Accounts ---\n");
    for (int i = 0; i < account_count; i++) {
        fprintf(outfp, "Account No: %d\n", accounts[i].acc_no);
        fprintf(outfp, "Account Type: %s\n", accounts[i].acc_type);
        fprintf(outfp, "Account Holder: %s\n", accounts[i].name);
        fprintf(outfp, "Balance: %.2f\n", accounts[i].balance);
        fprintf(outfp, "----------------------\n");
        printf("Account No: %d\n", accounts[i].acc_no);
        printf("Account Type: %s\n", accounts[i].acc_type);
        printf("Account Holder: %s\n", accounts[i].name);
        printf("Balance: %.2f\n", accounts[i].balance);
        printf("----------------------\n");
    }
    fflush(outfp);
    saveAccountsToFile();
    printAccountsToConsole();
}

void searchAccount() {
    int acc_no, found = 0;
    fprintf(outfp, "Enter account number to search: ");
    fflush(outfp);
    printf("Enter account number to search: ");
    if (scanf("%d", &acc_no) != 1) {
        clearInputBuffer();
        fprintf(outfp, "Invalid input.\n");
        printf("Invalid input.\n");
        fflush(outfp);
        return;
    }
    clearInputBuffer();
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].acc_no == acc_no) {
            fprintf(outfp, "Account found!\n");
            fprintf(outfp, "Account No: %d\n", accounts[i].acc_no);
            fprintf(outfp, "Account Type: %s\n", accounts[i].acc_type);
            fprintf(outfp, "Account Holder: %s\n", accounts[i].name);
            fprintf(outfp, "Balance: %.2f\n", accounts[i].balance);
            printf("Account found!\n");
            printf("Account No: %d\n", accounts[i].acc_no);
            printf("Account Type: %s\n", accounts[i].acc_type);
            printf("Account Holder: %s\n", accounts[i].name);
            printf("Balance: %.2f\n", accounts[i].balance);
            found = 1;
            break;
        }
    }
    if (!found) {
        fprintf(outfp, "Account not found.\n");
        printf("Account not found.\n");
    }
    fflush(outfp);
    saveAccountsToFile();
    printAccountsToConsole();
}

void deleteAccount() {
    int acc_no, found = 0;
    fprintf(outfp, "Enter account number to delete: ");
    fflush(outfp);
    printf("Enter account number to delete: ");
    if (scanf("%d", &acc_no) != 1) {
        clearInputBuffer();
        fprintf(outfp, "Invalid input.\n");
        printf("Invalid input.\n");
        fflush(outfp);
        return;
    }
    clearInputBuffer();
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].acc_no == acc_no) {
            for (int j = i; j < account_count - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            account_count--;
            fprintf(outfp, "Account deleted successfully.\n");
            printf("Account deleted successfully.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        fprintf(outfp, "Account not found.\n");
        printf("Account not found.\n");
    }
    fflush(outfp);
    saveAccountsToFile();
    printAccountsToConsole();
}

void updateAccount() {
    int acc_no, found = 0;
    fprintf(outfp, "Enter account number to update: ");
    fflush(outfp);
    printf("Enter account number to update: ");
    if (scanf("%d", &acc_no) != 1) {
        clearInputBuffer();
        fprintf(outfp, "Invalid input.\n");
        printf("Invalid input.\n");
        fflush(outfp);
        return;
    }
    clearInputBuffer();
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].acc_no == acc_no) {
            fprintf(outfp, "Enter new account holder name: ");
            fflush(outfp);
            printf("Enter new account holder name: ");
            if (!fgets(accounts[i].name, sizeof(accounts[i].name), stdin)) {
                fprintf(outfp, "Input error.\n");
                printf("Input error.\n");
                fflush(outfp);
                return;
            }
            size_t len = strlen(accounts[i].name);
            if (len > 0 && accounts[i].name[len - 1] == '\n') {
                accounts[i].name[len - 1] = '\0';
            }
            fprintf(outfp, "Enter new balance: ");
            fflush(outfp);
            printf("Enter new balance: ");
            if (scanf("%f", &accounts[i].balance) != 1) {
                clearInputBuffer();
                fprintf(outfp, "Invalid input.\n");
                printf("Invalid input.\n");
                fflush(outfp);
                return;
            }
            clearInputBuffer();
            fprintf(outfp, "Account updated successfully.\n");
            printf("Account updated successfully.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        fprintf(outfp, "Account not found.\n");
        printf("Account not found.\n");
    }
    fflush(outfp);
    saveAccountsToFile();
    printAccountsToConsole();
}

void printMainMenu() {
    printf("\nMain Menu:\n");
    printf("1. Create Account\n");
    printf("2. Deposit\n");
    printf("3. Withdraw\n");
    printf("4. Check Balance\n");
    printf("5. View Account Details\n");
    printf("6. Display All Accounts\n");
    printf("7. Search Account\n");
    printf("8. Delete Account\n");
    printf("9. Update Account\n");
    printf("10. Exit\n");
}

int main() {
    int op;
    srand((unsigned int)time(NULL));
    outfp = fopen("output.txt", "w");
    if (!outfp) {
        printf("Unable to open output.txt for writing.\n");
        return 1;
    }

    loadAccountsFromFile();

    // Show main menu only once
    fprintf(outfp, "\nMain Menu:\n");
    fprintf(outfp, "1. Create Account\n");
    fprintf(outfp, "2. Deposit\n");
    fprintf(outfp, "3. Withdraw\n");
    fprintf(outfp, "4. Check Balance\n");
    fprintf(outfp, "5. View Account Details\n");
    fprintf(outfp, "6. Display All Accounts\n");
    fprintf(outfp, "7. Search Account\n");
    fprintf(outfp, "8. Delete Account\n");
    fprintf(outfp, "9. Update Account\n");
    fprintf(outfp, "10. Exit\n");
    fflush(outfp);

    printMainMenu();

    while (1) {
        fprintf(outfp, "Enter your choice: ");
        fflush(outfp);

        printf("Enter your choice: ");
        if (scanf("%d", &op) != 1) {
            clearInputBuffer();
            fprintf(outfp, "Invalid input. Please enter a number.\n");
            printf("Invalid input. Please enter a number.\n");
            fflush(outfp);
            continue;
        }
        clearInputBuffer();

        switch (op) {
            case 1:
                createAccount();
                printDateTime();
                break;
            case 2:
                deposit();
                printDateTime();
                break;
            case 3:
                withdraw();
                printDateTime();
                break;
            case 4:
                checkBalance();
                printDateTime();
                break;
            case 5:
                viewAccount();
                printDateTime();
                break;
            case 6:
                displayAllAccounts();
                printDateTime();
                break;
            case 7:
                searchAccount();
                printDateTime();
                break;
            case 8:
                deleteAccount();
                printDateTime();
                break;
                 case 10:
                fprintf(outfp, "Thank you for using the bank!\n");
                printf("Thank you for using the bank!\n");
                printDateTime();
                fclose(outfp);
                return 0;
            default:
                fprintf(outfp, "Invalid option!\n");
                printf("Invalid option!\n");
                printDateTime();
        }
        printMainMenu();
    }
    fclose(outfp);
    return 0;
}