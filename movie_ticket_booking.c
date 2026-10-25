#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#define STANDARD_PRICE 150
#define PREMIUM_PRICE 250
#define RECLINER_PRICE 400

struct Customer
{
    char name[50];
    int number_of_tickets;
    int ticket_category;
};

int readLine(const char *message, char *input, size_t size)
{
    printf("%s", message);

    if (fgets(input, size, stdin) == NULL)
    {
        printf("Error: Unable to read input.\n");
        return 0;
    }

    size_t len = strlen(input);

    if (len > 0 && input[len - 1] == '\n')
    {
        input[len - 1] = '\0';
        return 1;
    }

    int ch = getchar();

    if (ch == '\n' || ch == EOF)
    {
        return 1;
    }

    while (ch != '\n' && ch != EOF)
    {
        ch = getchar();
    }

    printf("Error: Input is too long.\n");
    return 0;
}

int readInt(const char *message, int min, int max, int *value)
{
    char input[100];
    char *endptr;
    long number;

    if (!readLine(message, input, sizeof(input)))
        return 0;

    errno = 0;
    number = strtol(input, &endptr, 10);

    if (input == endptr || errno == ERANGE)
    {
        printf("Error: Please enter a valid integer.\n");
        return 0;
    }

    while (isspace((unsigned char)*endptr))
    {
        endptr++;
    }

    if (*endptr != '\0' || number < INT_MIN || number > INT_MAX)
    {
        printf("Error: Please enter a valid integer.\n");
        return 0;
    }

    if (number < min || number > max)
    {
        printf("Error: Value must be between %d and %d.\n", min, max);
        return 0;
    }

    *value = (int)number;
    return 1;
}

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
    if (!readLine("Enter customer name: ", c->name, sizeof(c->name)))
    {
        return 0;
    }

    if (c->name[0] == '\0')
    {
        printf("Error: Customer name cannot be empty.\n");
        return 0;
    }

    if (!readInt("Enter number of tickets: ", 1, 20, &c->number_of_tickets))
    {
        return 0;
    }

    if (!readInt("Enter ticket category (1-Standard, 2-Premium, 3-Recliner): ", 1, 3, &c->ticket_category))
    {
        return 0;
    }

    return 1;
}

double calculateSubtotal(int price, int tickets)
{
    return (double)price * tickets;
}

double calculateDiscount(double subtotal, int tickets)
{
    if (tickets >= 5)
        return subtotal * 0.10;

    return 0.0;
}

int displayBill(struct Customer *c, double subtotal, double discount)
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

    case 3:
        category = "Recliner";
        break;

    default:
        printf("Error: Invalid ticket category. Cannot display bill.\n");
        return 0;
    }

    printf("\nMovie Ticket Booking Bill\n\n");
    printf("Customer: %s\n", c->name);
    printf("Ticket Category: %s\n", category);
    printf("Tickets: %d\n", c->number_of_tickets);
    printf("Subtotal: %.2f\n", subtotal);
    printf("Discount: %.2f\n", discount);
    printf("Final Bill: %.2f\n", subtotal - discount);

    return 1;
}

int processBooking(struct Customer *c)
{
    int price = getTicketPrice(c);

    if (price == 0)
    {
        printf("Error: Unable to calculate price for invalid category.\n");
        return 0;
    }

    double subtotal = calculateSubtotal(price, c->number_of_tickets);

    double discount = calculateDiscount(subtotal, c->number_of_tickets);

    return displayBill(c, subtotal, discount);
}

int main()
{
    struct Customer *c = malloc(sizeof(struct Customer));

    if (c == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    if (!getCustomerDetails(c))
    {
        free(c);
        return 1;
    }

    if (!processBooking(c))
    {
        free(c);
        return 1;
    }

    free(c);
    c = NULL;

    return 0;
}
