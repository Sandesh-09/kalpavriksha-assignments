#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int STANDARD_PRICE = 150;
const int PREMIUM_PRICE = 250;
const int RECLINER_PRICE = 400;

struct Customer
{
    char name[50];
    int number_of_tickets;
    int ticket_category;
};

int getTicketPrice(struct Customer *c)
{
    switch (c->ticket_category)
    {
    case 1:
        return STANDARD_PRICE;
    case 2:
        return PREMIUM_PRICE;
    case 3:
        return RECLINER_PRICE;
    default:
        return 0;
    }
}

int getCustomerDetails(struct Customer *c)
{
    printf("Enter customer name: ");
    if (scanf("%49s", c->name) != 1)
        return 0;

    printf("Enter number of tickets: ");
    if (scanf("%d", &c->number_of_tickets) != 1 ||
        c->number_of_tickets < 1 ||
        c->number_of_tickets > 20)
    {
        printf("Error: Tickets must be between 1 and 20.\n");
        return 0;
    }

    printf("Enter ticket category (1-Standard, 2-Premium, 3-Recliner): ");
    if (scanf("%d", &c->ticket_category) != 1 || c->ticket_category < 1 || c->ticket_category > 3)
    {
        printf("Invalid category: Category must be 1, 2, or 3.\n");
        return 0;
    }

    return 1;
}

float calculateSubtotal(int price, int tickets)
{
    return (float)price * tickets;
}

float calculateDiscount(float subtotal, int tickets)
{
    if (tickets >= 5)
        return subtotal * 0.10f;

    return 0.0f;
}

void displayBill(struct Customer *c,
                 float subtotal, float discount)
{
    const char *category;

    switch (c->ticket_category)
    {
    case 1:
        category = "Standard";
        break;
    case 2:
        category = "Premium";
        break;
    default:
        category = "Recliner";
    }

    printf("\nCustomer: %s\n", c->name);
    printf("Ticket Category: %s\n", category);
    printf("Tickets: %d\n", c->number_of_tickets);
    printf("Subtotal: %.2f\n", subtotal);
    printf("Discount: %.2f\n", discount);
    printf("Final Bill: %.2f\n", subtotal - discount);
}

void processBooking(struct Customer *c)
{
    int price = getTicketPrice(c);

    float subtotal = calculateSubtotal(price, c->number_of_tickets);
    float discount = calculateDiscount(subtotal, c->number_of_tickets);

    displayBill(c, subtotal, discount);
}

int main(void)
{
    struct Customer *c = malloc(sizeof(struct Customer));

    if (c == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    if (getCustomerDetails(c))
    {
        processBooking(c);
    }

    free(c);
    c = NULL;

    return 0;
}
