#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>// Include string.h for string operations

#define MAX_ACCOUNTS 100

typedef struct {
    int acc_no;
    char acc_type[10];
    float balance;
    char name[50]; // Added name field for account holder
} Account;

Account accounts[MAX_ACCOUNTS];
int account_count = 0;
void printDateTime() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    printf("[Date & Time: %02d-%02d-%04d %02d:%02d:%02d]\n",
           t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
           t->tm_hour, t->tm_min, t->tm_sec);
}

int generateAccountNo() {
    int acc_no, unique, i;
    do {
        acc_no = 100 + rand() % 900;
        unique = 1;
        for (i = 0; i < account_count; i++) {
            if (accounts[i].acc_no == acc_no) {
                unique = 0;
                break;
            }
        }
    } while (!unique);
    return acc_no;
}

void createAccount() {
    if (account_count >= MAX_ACCOUNTS) {
        printf("Account limit reached!\n");
        return;
    }
    int choice;
    printf("\n--- Create Account Menu ---\n");
    printf("1. Current Account\n");
    printf("2. Saving Account\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice != 1 && choice != 2) {
        printf("Invalid choice. Please try again.\n");
        return;
    }

    printf("Enter account holder name: ");
    getchar(); // Clear newline left by previous scanf
    fgets(accounts[account_count].name, sizeof(accounts[account_count].name), stdin);
    size_t len = strlen(accounts[account_count].name);
    if (len > 0 && accounts[account_count].name[len - 1] == '\n') {
        accounts[account_count].name[len - 1] = '\0';
    }

    accounts[account_count].acc_no = generateAccountNo();
    strcpy(accounts[account_count].acc_type, (choice == 1) ? "Current" : "Saving");
    accounts[account_count].balance = 0.0f;
    printf("Account created successfully!\n");
    printf("Account No: %d\n", accounts[account_count].acc_no);
    printf("Account Type: %s\n", accounts[account_count].acc_type);
    printf("Account Holder: %s\n", accounts[account_count].name);
    account_count++;
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
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    idx = findAccount(acc_no);
    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }
    printf("Enter amount to deposit: ");
    scanf("%f", &amt);
    accounts[idx].balance += amt;
    printf("Deposited: %.2f. New Balance: %.2f\n", amt, accounts[idx].balance);
}

void withdraw() {
    int acc_no, idx;
    float amt;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    idx = findAccount(acc_no);
    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }
    printf("Enter amount to withdraw: ");
    scanf("%f", &amt);
    if (amt > accounts[idx].balance) {
        printf("Insufficient balance!\n");
    } else {
        accounts[idx].balance -= amt;
        printf("Withdrawn: %.2f. New Balance: %.2f\n", amt, accounts[idx].balance);
    }
}

void checkBalance() {
    int acc_no, idx;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    idx = findAccount(acc_no);
    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }
    printf("Current Balance: %.2f\n", accounts[idx].balance);
}

void viewAccount() {
    int acc_no, idx;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    idx = findAccount(acc_no);
    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }
    printf("Account No: %d\n", accounts[idx].acc_no);
    printf("Account Type: %s\n", accounts[idx].acc_type);
    printf("Balance: %.2f\n", accounts[idx].balance);
}

void displayAllAccounts() {
    if (account_count == 0) {
        printf("No accounts to display.\n");
        return;
    }
    printf("\n--- All Accounts ---\n");
    for (int i = 0; i < account_count; i++) {
        printf("Account No: %d\n", accounts[i].acc_no);
        printf("Account Type: %s\n", accounts[i].acc_type);
        printf("Account Holder: %s\n", accounts[i].name);
        printf("Balance: %.2f\n", accounts[i].balance);
        printf("----------------------\n");
    }
}

void searchAccount() {
    int acc_no, found = 0;
    printf("Enter account number to search: ");
    scanf("%d", &acc_no);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].acc_no == acc_no) {
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
        printf("Account not found.\n");
    }
}

void deleteAccount() {
    int acc_no, found = 0;
    printf("Enter account number to delete: ");
    scanf("%d", &acc_no);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].acc_no == acc_no) {
            for (int j = i; j < account_count - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            account_count--;
            printf("Account deleted successfully.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Account not found.\n");
    }
}

void updateAccount() {
    int acc_no, found = 0;
    printf("Enter account number to update: ");
    scanf("%d", &acc_no);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].acc_no == acc_no) {
            printf("Enter new account holder name: ");
            scanf(" %[^\n]", accounts[i].name);
            printf("Enter new balance: ");
            scanf("%f", &accounts[i].balance);
            printf("Account updated successfully.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Account not found.\n");
    }
}



int main() {
    int op;
    srand((unsigned int)time(NULL));
    while (1) {
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
        printf("Enter your choice: ");
        scanf("%d", &op);

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
            case 9:
                updateAccount();
                printDateTime();
                break;        
            default:
                printf("Invalid option!\n");
                printf("Thank you for using the bank!\n");
                printDateTime();
                return 0;
        }
    }
   
}