#include <stdio.h>
#include <string.h>

// Ingredients needed per serving, low threshhold of each ingredient
#define COFFEE_BEANS 8
#define WATER 30
#define MWATER 39
#define CMILK 70
#define MMILK 160
#define MCHOCOSYRUP 30
#define CORRECT_PASSWORD 1234 // Set a numeric password

// Prices
double ePrice = 3.5;
double cPrice = 4.5;
double mPrice = 5.5;

// Inventory variables, the ones repelnished 
int coffeeBeans = 8, water = 100, milk = 200, chocolateSyrup = 30;
double total_amount=0.0; //track total sales

//functions prototypes
void user();
void admin();
void replenishIngredients();
void changePrices();
void displayIngredients(); 

int main() {
    int choice;
    while(1) {
        printf("Welcome to the Coffee Shop!\n");
        printf("1) Order coffee\n");
        printf("2) Admin mode\n");
        printf("3) Exit\n");
        scanf("%d", &choice);

        if (choice == 1) {
            user();
        } else if (choice == 2) {
            admin();
        } else if (choice == 3) {
            printf("Goodbye!\n");
            break; // Exit the loop
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0; // Added return statement for main
}

void user() {
    int drink; // Declare the drink variable
    double price = 0; // Move this inside the function

    printf("Select your drink:\n");
    printf("%-12s%.2f AED\n", "1) Espresso", ePrice);
    printf("%-12s%.2f AED\n", "2) Cappuccino", cPrice);
    printf("%-12s%.2f AED\n", "3) Mocha", mPrice);
    printf("%-12s\n", "0) Exit");
    scanf("%d", &drink);

    if (drink==0){
        return; //exit to main funciton
    }
    if (drink == 1) {
        price = ePrice;
        if (coffeeBeans < COFFEE_BEANS || water < WATER) {
            printf(" Unavailable due to temporarily insufficient ingredients.\n");
            return; // Proper return here
        }
    } else if (drink == 2) {
        price = cPrice;
        if (coffeeBeans < COFFEE_BEANS || water < WATER || milk < CMILK) {
            printf(" Unavailable due to temporarily insufficient ingredients.\n");
            return; // Proper return here
        }
    } else if (drink == 3) {
        price = mPrice;
        if (coffeeBeans < COFFEE_BEANS || water < MWATER || milk < MMILK || chocolateSyrup < MCHOCOSYRUP) {
            printf(" Unavailable at the moment.\n");
            return; // Proper return here
        }
    } else {
        printf("Invalid choice.\n");
        return; // Proper return here
    }

    char confirm;
    printf("Confirm order for %.2f AED (y/n)? ", price);
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        printf("Insert %.2f AED: ", price);
        double payment = 0;
        double coin;

        while (payment < price) {
            do {
                printf("Enter a 1 or 0.5 coin: ");
                scanf("%lf", &coin);

                // Check if the coin is valid (1 or 0.5)
                if (coin == 1 || coin == 0.5) {
                    payment += coin; // Update payment only if valid
                    printf("Paid amount: %.2f\n", payment);
                } else {
                    printf("Error: Invalid coin. Please enter either 1 or 0.5.\n");
                }

            } while (payment < price); // Continue until the payment reaches or exceeds the price

            if (payment >= price) {
                printf("Enjoy your drink!\n");
                coffeeBeans -= COFFEE_BEANS;
                water -= (drink == 3) ? MWATER : WATER;
                milk -= (drink == 2) ? CMILK : (drink == 3) ? MMILK : 0;
                chocolateSyrup -= (drink == 3) ? MCHOCOSYRUP : 0;
                total_amount += price; // Add to total sales

            }
        }
    } else {
        printf("Order cancelled.\n");
    }
}

void admin() {
    int password; // Declare password variable
    printf("Enter admin password: ");
    scanf("%d", &password);

    if (CORRECT_PASSWORD != password) {
        printf("Incorrect password.\n");
        return; // Proper return here
    }

    int choice;
    do {
         printf("\nAdmin Menu:\n");
        printf("1) Display ingredients and total sales\n");
        printf("2) Replenish ingredients\n");
        printf("3) Change drink prices\n");
        printf("0) Exit admin mode\n");
        scanf("%d", &choice);

        if (choice == 1) {
            displayIngredients();
        } else if (choice == 2) {
            replenishIngredients();
        } else if (choice == 3) {
            changePrices();
        } else if (choice == 0) {
            printf("Exiting admin mode.\n");
        } else {
            printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

void displayIngredients() {
    printf("\nCurrent Ingredients:\n");
    printf("%-12s%d\n", "Coffee Beans:", coffeeBeans);
    if (coffeeBeans < COFFEE_BEANS) printf("Warning: Not enough coffee beans for a single serving!\n");

    printf("%-12s%d\n", "Water:", water);
    if (water < WATER) printf("Warning: Not enough water for a single serving!\n");

    printf("%-12s%d\n", "Milk:", milk);
    if (milk < CMILK) printf("Warning: Not enough milk for a Cappuccino!\n");
    if (milk < MMILK) printf("Warning: Not enough milk for a Mocha!\n");

    printf("%-12s%d\n", "Chocolate Syrup:", chocolateSyrup);
    if (chocolateSyrup < MCHOCOSYRUP) printf("Warning: Not enough chocolate syrup for a Mocha!\n");

    printf("%-12s%.2f AED\n", "Total Sales:", total_amount);

    char reset;
    printf("Would you like to reset the total sales amount? (y/n): ");
    scanf(" %c", &reset);

    if (reset == 'y' || reset == 'Y') {
        total_amount = 0.0;
        printf("Total sales amount reset. Please collect the money.\n");
    }
}


#include <stdlib.h>  // For rand()

void replenishIngredients() {
    // Generate random amounts to replenish each ingredient (between 10 and 50 units)
    int beans = rand() % 41 + 10;   // Random between 10 and 50
    int w = rand() % 41 + 10;       // Random between 10 and 50
    int m = rand() % 41 + 10;       // Random between 10 and 50
    int syrup = rand() % 41 + 10;   // Random between 10 and 50

    // Add the random amounts to the current ingredient levels
    coffeeBeans += beans;
    water += w;
    milk += m;
    chocolateSyrup += syrup;

    // Print how much of each ingredient was replenished
    printf("\nIngredients replenished randomly:\n");
    printf("%-12s%d (added %d)\n", "Coffee Beans:", coffeeBeans, beans);
    printf("%-12s%d (added %d)\n", "Water:", water, w);
    printf("%-12s%d (added %d)\n", "Milk:", milk, m);
    printf("%-12s%d (added %d)\n", "Chocolate Syrup:", chocolateSyrup, syrup);
}


void changePrices() {
    // Display the current price of Espresso
    printf("Current Espresso price: %.2f AED\n", ePrice);
    printf("New price for Espresso: ");
    scanf("%lf", &ePrice); // Update the price
    
    // Repeat for other drinks (Cappuccino and Mocha)
    printf("Current Cappuccino price: %.2f AED\n", cPrice);
    printf("New price for Cappuccino: ");
    scanf("%lf", &cPrice);

    printf("Current Mocha price: %.2f AED\n", mPrice);
    printf("New price for Mocha: ");
    scanf("%lf", &mPrice);
    
    printf("Prices updated.\n");
}


// When the quantity of any ingredient in the machine becomes less than or equal to its corresponding threshold value, an alert should be sent to the coffee maker operator. This alert is modelled by printing a relevant message.
//A real-value variable total_amount which gives the total sales amount.  
// we assume all user inputs are valid; any printed value should occupy a field of 12 spaces, be left aligned, with 2 fractional precisions when applicable.
//add a 0 in drinks menu to exit back to main menu
//add in admin menu-> 1:  Display the quantity of each ingredient in the machine and the total sale amount, 2;replenish 3;change price 0;exit
// Display the total sale amount (total_amount); prompt the operator if (s)he would like to reset it to zero. If so, reset the total_amount variable and remind the operator to collect the money from the machine.
//add an exit to the infinite loop

//make error message under amoint of ingredeients 
//replenish ingredients using rand function 