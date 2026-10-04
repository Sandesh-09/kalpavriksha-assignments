#include <stdio.h>
#include <ctype.h>
#include <limits.h>

#define MAX_EXPRESSION_LENGTH 1000
#define MAX_TOKENS 1000

int parseExpression(const char exp[], int num[], char oper[], int *n, int *o)
{
    int i = 0;
    int number = 0;
    int previousTokenWasNumber = 0;

    while (exp[i] != '\0')
    {
        // Ignore whitespace characters.
        if (isspace((unsigned char)exp[i]))
        {
            i++;
            continue;
        }

        // Parse a number.
        if (exp[i] >= '0' && exp[i] <= '9')
        {
            if (previousTokenWasNumber)
            {
                return 0;
            }

            number = 0;

            while (exp[i] >= '0' && exp[i] <= '9')
            {
                int digit = exp[i] - '0';

                if (number > (INT_MAX - digit) / 10)
                {
                    return 0;
                }

                number = number * 10 + digit;
                i++;
            }

            if (*n >= MAX_TOKENS)
            {
                return 0;
            }

            num[*n] = number;
            (*n)++;

            previousTokenWasNumber = 1;
        }

        // Parse an operator.
        else if (exp[i] == '+' || exp[i] == '-' ||
                 exp[i] == '*' || exp[i] == '/')
        {
            if (!previousTokenWasNumber)
            {
                return 0;
            }

            if (*o >= MAX_TOKENS)
            {
                return 0;
            }

            oper[*o] = exp[i];
            (*o)++;

            previousTokenWasNumber = 0;
            i++;
        }

        // Reject unsupported characters.
        else
        {
            return 0;
        }
    }

    // Expression must end with a number.
    if (!previousTokenWasNumber || *n == 0)
    {
        return 0;
    }

    return 1;
}

int applyOperator(int left, int right, char operator, int *result)
{
    if (operator == '+')
    {
        *result = left + right;
    }
    else if (operator == '-')
    {
        *result = left - right;
    }
    else if (operator == '*')
    {
        *result = left * right;
    }
    else if (operator == '/')
    {
        if (right == 0)
        {
            return 0;
        }

        *result = left / right;
    }

    return 1;
}

int evaluateExpression(int num[], char oper[], int n, int o, int *result)
{
    int i = 0;

    // Evaluate multiplication and division first.
    while (i < o)
    {
        if (oper[i] == '*' || oper[i] == '/')
        {
            int value;

            if (!applyOperator(num[i], num[i + 1], oper[i], &value))
            {
                return 0;
            }

            num[i] = value;

            // Shift remaining numbers and operators left.
            for (int j = i + 1; j < n - 1; j++)
            {
                num[j] = num[j + 1];
            }

            for (int j = i; j < o - 1; j++)
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

    // Evaluate addition and subtraction.
    *result = num[0];

    for (i = 0; i < o; i++)
    {
        int value;

        if (!applyOperator(*result, num[i + 1], oper[i], &value))
        {
            return 0;
        }

        *result = value;
    }

    return 1;
}

int main()
{
    char exp[MAX_EXPRESSION_LENGTH];
    int num[MAX_TOKENS];
    char oper[MAX_TOKENS];

    int n = 0;
    int o = 0;
    int result;

    printf("Enter expression: ");

    if (fgets(exp, sizeof(exp), stdin) == NULL)
    {
        printf("Error: Invalid expression.\n");
        return 0;
    }

    if (!parseExpression(exp, num, oper, &n, &o))
    {
        printf("Error: Invalid expression.\n");
        return 0;
    }

    if (!evaluateExpression(num, oper, n, o, &result))
    {
        printf("Error: Division by zero.\n");
        return 0;
    }

    printf("%d\n", result);

    return 0;
}