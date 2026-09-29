#include <stdio.h>
int main()
{
    char exp[1000];

    int i = 0;
    char oper[1000];
    int num[1000];

    int n = 0, o = 0;
    int number = 0;
    int hasNumber = 0;

    printf("Enter expression: ");
    scanf("%[^\n]", exp);

    while (exp[i] != '\0')
    {

        // ignore spaces
        if (exp[i] == ' ')
        {
            i++;
            continue;
        }

        // reading number
        if (exp[i] >= '0' && exp[i] <= '9')
        {
            // handeling edge case ( like 1 2)
            if (hasNumber == 1)
            {
                printf("Error: Invalid expression.\n");
                return 0;
            }

            number = 0;

            while (exp[i] >= '0' && exp[i] <= '9')
            {
                number = number * 10 + (exp[i] - '0');
                i++;
            }

            num[n] = number;
            n++;

            hasNumber = 1;
        }

        // reading operator
        else if (exp[i] == '+' || exp[i] == '-' ||
                 exp[i] == '*' || exp[i] == '/')
        {

            if (hasNumber == 0)
            {
                printf("Error: Invalid expression.\n");
                return 0;
            }

            oper[o] = exp[i];
            o++;

            hasNumber = 0;
            i++;
        }

        // invalid character
        else
        {
            printf("Error: Invalid expression.\n");
            return 0;
        }
    }

    // expression ending with operator
    if (hasNumber == 0 || n == 0)
    {
        printf("Error: Invalid expression.\n");
        return 0;
    }

    // handle * and /
    i = 0;

    while (i < o)
    {

        if (oper[i] == '*' || oper[i] == '/')
        {

            if (oper[i] == '/' && num[i + 1] == 0)
            {
                printf("Error: Division by zero.\n");
                return 0;
            }

            if (oper[i] == '*')
            {
                num[i] = num[i] * num[i + 1];
            }
            else
            {
                num[i] = num[i] / num[i + 1];
            }

            // shift numbers left
            int j;

            for (j = i + 1; j < n - 1; j++)
            {
                num[j] = num[j + 1];
            }

            // shift operators left
            for (j = i; j < o - 1; j++)
            {
                oper[j] = oper[j + 1];
            }

            n--;
            o--;
        }
        else
        {
            i++;
        }
    }

    // handle + and -
    int result = num[0];

    for (i = 0; i < o; i++)
    {

        if (oper[i] == '+')
        {
            result = result + num[i + 1];
        }
        else
        {
            result = result - num[i + 1];
        }
    }

    printf("%d\n", result);

    return 0;
}