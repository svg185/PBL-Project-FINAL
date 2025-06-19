#include <stdio.h>
#include <string.h>

#define MAX_ACCOUNTS 100

typedef struct {
    int accountNumber;
    char holderName[100];
    char accountType[20];
    float balance;
} Account;

int findAccount(Account accounts[], int count, int accountNumber) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

void createAccount(Account accounts[], int *count) {
    if (*count >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached.\n");
        return;
    }
    int accNum;
    printf("Enter account number: ");
    if (scanf("%d", &accNum) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); // clear input buffer
        return;
    }
    while (getchar() != '\n'); // clear input buffer

    if (findAccount(accounts, *count, accNum) != -1) {
        printf("Account number already exists.\n");
        return;
    }
    accounts[*count].accountNumber = accNum;

    printf("Enter account holder name: ");
    fgets(accounts[*count].holderName, sizeof(accounts[*count].holderName), stdin);
    accounts[*count].holderName[strcspn(accounts[*count].holderName, "\n")] = 0; // Remove newline

    printf("Enter account type (Savings/Current): ");
    fgets(accounts[*count].accountType, sizeof(accounts[*count].accountType), stdin);
    accounts[*count].accountType[strcspn(accounts[*count].accountType, "\n")] = 0; // Remove newline

    accounts[*count].balance = 0.0;
    printf("Account created successfully!\n");
    (*count)++;
}

void depositMoney(Account accounts[], int count) {
    int accNum, idx;
    float amount;
    printf("Enter account number: ");
    if (scanf("%d", &accNum) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    idx = findAccount(accounts, count, accNum);
    if (idx == -1) {
        printf("Account not found.\n");
        while (getchar() != '\n');
        return;
    }
    printf("Enter amount to deposit: ");
    if (scanf("%f", &amount) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    if (amount > 0) {
        accounts[idx].balance += amount;
        printf("Amount deposited successfully!\n");
    } else {
        printf("Invalid amount.\n");
    }
    while (getchar() != '\n');
}

void withdrawMoney(Account accounts[], int count) {
    int accNum, idx;
    float amount;
    printf("Enter account number: ");
    if (scanf("%d", &accNum) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    idx = findAccount(accounts, count, accNum);
    if (idx == -1) {
        printf("Account not found.\n");
        while (getchar() != '\n');
        return;
    }
    printf("Enter amount to withdraw: ");
    if (scanf("%f", &amount) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    if (amount > 0 && accounts[idx].balance >= amount) {
        accounts[idx].balance -= amount;
        printf("Amount withdrawn successfully!\n");
    } else {
        printf("Insufficient balance or invalid amount.\n");
    }
    while (getchar() != '\n');
}

void checkBalance(Account accounts[], int count) {
    int accNum, idx;
    printf("Enter account number: ");
    if (scanf("%d", &accNum) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    idx = findAccount(accounts, count, accNum);
    if (idx == -1) {
        printf("Account not found.\n");
        while (getchar() != '\n');
        return;
    }
    printf("Current Balance: %.2f\n", accounts[idx].balance);
    while (getchar() != '\n');
}

void viewDetails(Account accounts[], int count) {
    int accNum, idx;
    printf("Enter account number: ");
    if (scanf("%d", &accNum) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    idx = findAccount(accounts, count, accNum);
    if (idx == -1) {
        printf("Account not found.\n");
        while (getchar() != '\n');
        return;
    }
    printf("\n--- Account Details ---\n");
    printf("Account Number: %d\n", accounts[idx].accountNumber);
    printf("Holder Name: %s\n", accounts[idx].holderName);
    printf("Account Type: %s\n", accounts[idx].accountType);
    printf("Balance: %.2f\n", accounts[idx].balance);
    while (getchar() != '\n');
}

int main() {
    Account accounts[MAX_ACCOUNTS];
    int accountCount = 0, choice;

    printf("\n--- Bank Management System ---\n");
    printf("1. Create Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Check Balance\n");
    printf("5. View Account Details\n");
    printf("6. Exit\n");

    do {
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 1:
                createAccount(accounts, &accountCount);
                break;
            case 2:
                depositMoney(accounts, accountCount);
                break;
            case 3:
                withdrawMoney(accounts, accountCount);
                break;
            case 4:
                checkBalance(accounts, accountCount);
                break;
            case 5:
                viewDetails(accounts, accountCount);
                break;
            case 6:
                printf("Thank you for using the Bank Management System!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    return 0;
}