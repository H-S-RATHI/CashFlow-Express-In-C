#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <conio.h>
#define CLEAR_SCREEN "cls"
#else
#include <stdio_ext.h> // For __fpurge on some non-Windows systems
#define CLEAR_SCREEN "clear"
#endif

typedef struct {
  int accountNumber;
  int pin;
  int64_t balance;
  char name[50];
  char email[100];
  char phone[15];
} BankAccount;

// Function prototypes
int authenticateUser(BankAccount *accounts, int numAccounts,
                     int enteredAccountNumber, int enteredPin);
void openAccount(BankAccount *accounts, int *numAccounts);
void saveAccountsToFile(BankAccount *accounts, int numAccounts);
void loadAccountsFromFile(BankAccount *accounts, int *numAccounts);
void performTransactions(BankAccount *accounts, int loggedInAccountIndex,
                         int numAccounts);

int main() {
  int numAccounts = 0;
  BankAccount accounts[10]; // Assuming a maximum of 10 accounts

  loadAccountsFromFile(accounts, &numAccounts);
  system(CLEAR_SCREEN);
  printf("\n\t\t\t\t===***Welcome To CashFlow Express***===");

  while (1) {
    printf("\n\t\t\t\t========*Available Operations*=========");
    printf("\n\n\t\t\t\t\t    1. Login");
    printf("\n\t\t\t\t\t    2. Open Bank Account");
    printf("\n\t\t\t\t\t    0. Exit");
    printf("\n\n\t\t\t\tPlease select an option: ");
    int option;
    scanf("%d", &option);
    system(CLEAR_SCREEN);
    
    switch (option) {
    case 1: {
      printf("\n\n\t\t\t\tEnter your account number: ");
      int enteredAccountNumber;
      scanf("%d", &enteredAccountNumber);

      printf("\n\t\t\t\tEnter your PIN: ");
      int enteredPin;
      scanf("%d", &enteredPin);

      int loggedInAccountIndex = authenticateUser(
          accounts, numAccounts, enteredAccountNumber, enteredPin);

      if (loggedInAccountIndex == -1) {
        printf("\n\n\t\t\t\tAuthentication failed. Invalid account number or "
               "PIN.\n");
      } else {
        printf("\n\n\t\t\t\t\tAuthentication successful!\n");
        performTransactions(accounts, loggedInAccountIndex, numAccounts);
      }
      break;
    }
    case 2:
      openAccount(accounts, &numAccounts);
      saveAccountsToFile(accounts, numAccounts);
      break;
    case 0:
      saveAccountsToFile(accounts, numAccounts);
      printf("\n\n\t\t\t\tThank you for using the ATM. Goodbye!\n");
      return 0;
    default:
      printf("\n\n\t\t\t\tInvalid option\n");
      break;
    }
  }

  return 0;
}

int authenticateUser(BankAccount *accounts, int numAccounts,
                     int enteredAccountNumber, int enteredPin) {
  // Iterate through the accounts to find a match
  for (int i = 0; i < numAccounts; i++) {
    if (accounts[i].accountNumber == enteredAccountNumber &&
        accounts[i].pin == enteredPin) {
      return i; // Return the index of the authenticated account
    }
  }
  return -1; // Return -1 to indicate authentication failure
}

void openAccount(BankAccount *accounts, int *numAccounts) {
  if (*numAccounts < 10) {
    BankAccount newAccount;

    printf("\n\n\t\t\t\tEnter your name: ");
    scanf("%s", newAccount.name);

    printf("\t\t\t\tEnter your email: ");
    scanf("%s", newAccount.email);

    printf("\t\t\t\tEnter your phone number: ");
    scanf("%s", newAccount.phone);

    newAccount.accountNumber =
        1000 + *numAccounts; // Example way of generating account numbers
    printf("\n\t\t\t\tYour account number: %d\n", newAccount.accountNumber);

    printf("\t\t\t\tSet a PIN for your account: ");
    scanf("%d", &newAccount.pin);

    newAccount.balance = 0;

    accounts[*numAccounts] = newAccount;
    (*numAccounts)++;

    printf("\n\t\t\t\tAccount successfully opened!\n");
  } else {
    printf("\n\t\t\t\tMaximum number of accounts reached. Cannot open a new "
           "account.\n");
  }
}

void performTransactions(BankAccount *accounts, int loggedInAccountIndex,
                         int numAccounts) {
  while (1) {
    printf("\n\t\t\t================* Available Transactions*===============");
    printf("\n\n\t\t\t\t\t1. Withdraw");
    printf("\n\t\t\t\t\t2. Deposit");
    printf("\n\t\t\t\t\t3. Check Balance");
    printf("\n\t\t\t\t\t0. Logout");
    printf("\n\n\t\t\tPlease select an option: ");
    int option;
    scanf("%d", &option);

    switch (option) {
    case 1:
      printf("\n\t\t\t\tEnter amount to withdraw: ");
      int amount;
      scanf("%d", &amount);
      if (amount > 0 && amount <= accounts[loggedInAccountIndex].balance) {
        accounts[loggedInAccountIndex].balance -= amount;
        printf("\n\t\t\t\tWithdrawal successful. New balance: %" PRId64 "\n",
               accounts[loggedInAccountIndex].balance);
        saveAccountsToFile(accounts,
                           numAccounts); // Update and save account data
      } else {
        printf(
            "\n\t\t\t\tInvalid withdrawal amount or insufficient balance.\n");
      }
      sleep(2); // Use sleep function instead of Sleep
      break;
    case 2:
      printf("\n\t\t\t\tEnter amount to deposit: ");
      scanf("%d", &amount);
      if (amount > 0) {
        accounts[loggedInAccountIndex].balance += amount;
        printf("\n\t\t\t\tDeposit successful. New balance: %" PRId64 "\n",
               accounts[loggedInAccountIndex].balance);
        saveAccountsToFile(accounts,
                           numAccounts); // Update and save account data
      } else {
        printf("\n\t\t\t\tInvalid deposit amount.\n");
      }
      sleep(2); // Use sleep function instead of Sleep
      break;
    case 3:
      printf("\n\t\t\t\t\tYour balance: %" PRId64 "\n",
             accounts[loggedInAccountIndex].balance);
      sleep(2); // Use sleep function instead of Sleep
      break;
    case 0:
      printf("\n\t\t\t\t\tLogged out. Returning to the main menu.\n");
      sleep(2); // Use sleep function instead of Sleep
      return;
    default:
      printf("\n\t\t\t\t\tInvalid option\n");
      sleep(2); // Use sleep function instead of Sleep
      break;
    }
  }
}

void saveAccountsToFile(BankAccount *accounts, int numAccounts) {
  FILE *file = fopen("./accounts.txt", "w"); // Use a relative path
  if (file == NULL) {
    printf("\n\t\t\t\tError opening file for writing.\n");
    return;
  }

  for (int i = 0; i < numAccounts; i++) {
    fprintf(file, "%d %d %" PRId64 " %s %s %s\n", accounts[i].accountNumber,
            accounts[i].pin, accounts[i].balance, accounts[i].name,
            accounts[i].email, accounts[i].phone);
  }

  fclose(file);
}

void loadAccountsFromFile(BankAccount *accounts, int *numAccounts) {
  FILE *file = fopen("./accounts.txt", "r"); // Use a relative path
  if (file == NULL) {
    printf("\n\t\t\t\tNo saved account data found.\n");
    return;
  }

  *numAccounts = 0;
  while (*numAccounts < 10 &&
         fscanf(file, "%d %d %" PRId64 " %s %s %s",
                &accounts[*numAccounts].accountNumber,
                &accounts[*numAccounts].pin, &accounts[*numAccounts].balance,
                accounts[*numAccounts].name, accounts[*numAccounts].email,
                accounts[*numAccounts].phone) != EOF) {
    (*numAccounts)++;
  }

  fclose(file);
}