#include <stdio.h>
#include <math.h>

#define coffeebeans 8 //needed coffee beans for all
#define water 30 //needed water for espresso and cappuccino
#define mwater 39 //needed water for mocha
#define cmilk 70 //needed milk for cappuccino
#define mmilk 160 //needed milk for mocha
#define mchocosyrup 30 //needed chocolate syrup for mocha
#define eprice 3.5 //price of espresso
#define cprice 4.3 //price of cappuccino
#define mprice 5.5  //price of mocha

int main()
{
    int x;
    printf(" Hello! Please choose of the following to continue: 1) Order coffee type.  2) admin mode. 3) end application execution.");
    scanf("%d", &x);
    if (x==1) //shows user funciton
    {
        /* code */
    }
    elif (x==2) //shows admin funciton
    {

    }
    elif (x==3) //ends 
    {

    }
    else 
    printf("ERROR. please enter a valid number");
    return 0;
}





/*  
function for each mode, menu
*/
/*
int # (), void #, voide #(), int main ()*/
/*
main menu: order coffee, admin mode, end application execution  
dont consider returning change
send message to operator if insufficient ingredients 
order summary/ type of accepted coins
back to main menu    

for admin, 
admin pass, give error message and return to main if it had a mismatch
function to upgrade price
total sale amount
ingredients quentity
ingredients refill
exit admin
 */
