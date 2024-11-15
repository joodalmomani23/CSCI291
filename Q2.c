#include <stdio.h>
#include <stdlib.h>

void processTransactions(int *transactions, int size, int initial_balance);

int main() {
    int initial_balance = 1000;
    int transaction;
    int size = 0;
    int capacity = 5;

    // Dynamically allocated array for transactions with an initial capacity
    int *transactions = (int *)malloc(capacity * sizeof(int)); 
    //int *transactions part declares transactions as a pointer to an integer (int).
//The malloc function then allocates a block of memory that can hold capacity number of int values.

    printf("Enter transactions (positive for deposit, negative for withdrawal). Enter 0 to finish:\n");

    int balance = initial_balance; // Use a separate balance variable for tracking during input

    while (1) {
        if (balance <= 0) {
            printf("Error: Balance is 0. No further transactions can be entered.\n");
            break;
        }

        printf("Transaction %d: ", size + 1);
        scanf("%d", &transaction);

        if (transaction == 0) {
            break;
        }

        // Check if this transaction will result in a balance of zero
        if (balance + transaction <= 0) {
            printf("Error: Balance has reached 0. No further transactions will be processed.\n");
            transactions[size++] = transaction; // Add this transaction before stopping
            break;
        }

        // Ensure capacity to store new transactions
        if (size >= capacity) {
            capacity *= 2;
            transactions = (int *)realloc(transactions, capacity * sizeof(int)); 
            //requests a new memory block that can hold the updated capacity of integers.
        }

        // Add the transaction and update balance
        transactions[size++] = transaction;
        balance += transaction;
    }

    processTransactions(transactions, size, initial_balance);

    free(transactions); // Free allocated memory
    return 0;
}

void processTransactions(int *transactions, int size, int initial_balance) {
    int balance = initial_balance;
    int *toBeProcessed = (int *)malloc(size * sizeof(int)); // Dynamically allocated array for unprocessed transactions; 
    int toBeProcessedCount = 0;

//int size keeps track of how many transactions are currently in the array.
    for (int i = 0; i < size; i++) {
        int transaction = transactions[i];

        if (transaction < 0 && balance + transaction < 0) {
            // Transaction is a withdrawal and exceeds the balance
            printf("Transaction %d is invalid due to insufficient funds.\n", transaction);
            toBeProcessed[toBeProcessedCount++] = transaction;
            continue;
        }

        balance += transaction;

        if (balance <= 0) {
            printf("Error: Balance has reached 0. No further transactions will be processed.\n");
            break;
        }
    }

    printf("Final Balance: %d AED\n", balance);
    printf("Unprocessed Transactions: ");
    for (int i = 0; i < toBeProcessedCount; i++) {
        printf("%d ", toBeProcessed[i]);
    }
    printf("\n");

    free(toBeProcessed); // Free allocated memory
}
