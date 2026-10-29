#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#define MAXLEN 100
#define MIN_SIZE 2
#define MAX_SIZE 10

int readInteger(const char *message)
{
    char input[MAXLEN];
    char *endptr;
    long value;

    while (1)
    {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            return -1;
        }

        if (strchr(input, '\n') == NULL)
        {
            int ch;
            int too_long = 0;

            while ((ch = getchar()) != '\n' && ch != EOF)
            {
                too_long = 1;
            }

            if (too_long)
            {
                printf("Input is too long.\n");
                continue;
            }
        }

        errno = 0;
        value = strtol(input, &endptr, 10);

        if (endptr == input || errno == ERANGE || value < INT_MIN || value > INT_MAX)
        {
            printf("Enter a valid integer.\n");
            continue;
        }

        while (isspace((unsigned char)*endptr))
            endptr++;

        if (*endptr != '\0')
        {
            printf("Enter only an integer.\n");
            continue;
        }

        return (int)value;
    }
}

int **createMatrix(int n)
{
    int **matrix = malloc(n * sizeof(*matrix));

    if (matrix == NULL)
        return NULL;

    for (int i = 0; i < n; i++)
    {
        *(matrix + i) = malloc(n * sizeof(**matrix));

        if (*(matrix + i) == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(*(matrix + j));
            }

            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

void freeMatrix(int **matrix, int n)
{
    if (matrix == NULL)
        return;

    for (int i = 0; i < n; i++)
    {
        free(*(matrix + i));
    }

    free(matrix);
}

void generateMatrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(*(matrix + i) + j) = rand() % 256;
        }
    }
}

void displayMatrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", *(*(matrix + i) + j));
        }

        printf("\n");
    }
}

void swapValues(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void transposeMatrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            swapValues(*(matrix + i) + j, *(matrix + j) + i);
        }
    }
}

void reverseRows(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        int *left = *(matrix + i);
        int *right = left + n - 1;

        while (left < right)
        {
            swapValues(left, right);
            left++;
            right--;
        }
    }
}

void rotateMatrix(int **matrix, int n)
{
    transposeMatrix(matrix, n);
    reverseRows(matrix, n);
}

int applyFilter(int **matrix, int n)
{
    int *prevRow = malloc(n * sizeof(*prevRow));
    int *currRow = malloc(n * sizeof(*currRow));

    if (prevRow == NULL || currRow == NULL)
    {
        free(prevRow);
        free(currRow);
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(currRow + j) = *(*(matrix + i) + j);
        }

        for (int j = 0; j < n; j++)
        {
            int sum = 0;
            int count = 0;

            for (int r = i - 1; r <= i + 1; r++)
            {
                for (int c = j - 1; c <= j + 1; c++)
                {
                    if (r < 0 || r >= n || c < 0 || c >= n)
                        continue;

                    if (r == i - 1)
                        sum += *(prevRow + c);
                    else if (r == i)
                        sum += *(currRow + c);
                    else
                        sum += *(*(matrix + r) + c);

                    count++;
                }
            }

            *(*(matrix + i) + j) = sum / count;
        }

        for (int j = 0; j < n; j++)
            *(prevRow + j) = *(currRow + j);
    }

    free(prevRow);
    free(currRow);
    return 1;
}

int main()
{
    int n = readInteger("Enter matrix size (2-10): ");

    if (n == -1)
    {
        printf("Unable to read input.\n");
        return 1;
    }

    while (n < MIN_SIZE || n > MAX_SIZE)
    {
        printf("Size must be between 2 and 10.\n");
        n = readInteger("Enter matrix size: ");

        if (n == -1)
            return 1;
    }

    srand((unsigned int)time(NULL));

    int **matrix = createMatrix(n);

    if (matrix == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    generateMatrix(matrix, n);

    printf("\nOriginal Randomly Generated Matrix:\n");
    displayMatrix(matrix, n);

    rotateMatrix(matrix, n);

    printf("\nMatrix after 90-degree Clockwise Rotation:\n");
    displayMatrix(matrix, n);

    if (!applyFilter(matrix, n))
    {
        printf("Unable to apply filter.\n");
        freeMatrix(matrix, n);
        return 1;
    }

    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    displayMatrix(matrix, n);

    freeMatrix(matrix, n);
    return 0;
}
