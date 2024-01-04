// Practice using structs
// Practice writing a linear search function

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

// Number of menu items
// Adjust this value (10) to number of items input below
#define NUM_ITEMS 10

// Menu itmes have item name and price
typedef struct
{
    string item;
    float price;
}
menu_item;

// Array of menu items
menu_item menu[NUM_ITEMS];

// Add items to menu
void add_items(void);

// Calculate total cost
float get_cost(string item);

int main(void)
{
    add_items();

    printf("\nWelcome to Beach Burger Shack!\n");
    printf("Choose from the following menu to order. Press enter when done.\n\n");

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        printf("%s: $%.2f\n", menu[i].item, menu[i]. price);
    }
    printf("\n");

    float total = 0;
    while (true)
    {
        string item = get_string("Enter a food item: ");
        if (strlen(item) == 0)
        {
            printf("\n");
            break;
        }

        total += get_cost(item);
    }

    printf("Your total cost is: $%.2f\n", total);
}

/**
 * Beach Burger Shack has the following 10 items on their menu
 * Burger: $9.5
 * Vegan Burger: $11
 * Hot Dog: $5
 * Cheese Dog: $7
 * Fries: $5
 * Cheese Fries: $6
 * Cold Pressed Juice: $7
 * Cold Brew: $3
 * Water: $2
 * Soda: $2
*/


// Add at least the first four items to the menu array
void add_items(void)
{
    menu_item a = {.item = "Burger", .price = 9.5};
    menu_item b = {.item = "Vegan Burger", .price = 11};
    menu_item c = {.item = "Hot Dog", .price = 5};
    menu_item d = {.item = "Cheese Dog", .price = 7};
    menu_item e = {.item = "Fries", .price = 5};
    menu_item f = {.item = "Cheese Fries", .price = 6};
    menu_item g = {.item = "Cold Pressed Juice", .price = 7};
    menu_item h = {.item = "Cold Brew", .price = 3};
    menu_item i = {.item = "Water", .price = 2};
    menu_item j = {.item = "Soda", .price = 2};
    menu[0] = a;
    menu[1] = b;
    menu[2] = c;
    menu[3] = d;
    menu[4] = e;
    menu[5] = f;
    menu[6] = g;
    menu[7] = h;
    menu[8] = i;
    menu[9] = j;
    return;
}

// Search through the menu array to find an item's cost
float get_cost(string item)
{
    item[0] = toupper(item[0]); //makes the first character of the inputted string uppercase, no matter what
    for (int j = 1; j < strlen(item); j++) //loops through inputted string to fix it
    {
        if (isspace(item[j - 1])) //if there is a space before our place, then make the letter uppercase
        {
            item[j] = toupper(item[j]);
        }
        else //otherwise, make it lowercase
        {
            item[j] = tolower(item[j]);
        }
    }
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        if (strcmp(menu[i].item, item) == 0)
        {
            return menu[i].price; //if our fixed string is identical to a menu string, return its price
        }
    }
    return 0.0;
}
