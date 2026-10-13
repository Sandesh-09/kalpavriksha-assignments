#include <stdio.h>

#define MAX_STUDENTS 100
#define SUBJECTS 3

struct Student
{
    int rollNumber;
    char name[50];
    int marks[SUBJECTS];
};

int calculateTotal(const struct Student *s)
{
    int total = 0;

    for (int i = 0; i < SUBJECTS; i++)
    {
        total += s->marks[i];
    }

    return total;
}

float calculateAverage(int total)
{
    return total / (float)SUBJECTS;
}

char calculateGrade(float average)
{
    if (average >= 85)
        return 'A';
    else if (average >= 70)
        return 'B';
    else if (average >= 50)
        return 'C';
    else if (average >= 35)
        return 'D';
    else
        return 'F';
}

int getStarCount(char grade)
{
    switch (grade)
    {
    case 'A':
        return 5;
    case 'B':
        return 4;
    case 'C':
        return 3;
    case 'D':
        return 2;
    default:
        return 0;
    }
}

void printStars(char grade)
{
    int stars = getStarCount(grade);

    for (int i = 0; i < stars; i++)
    {
        printf("*");
    }

    printf("\n");
}

int inputStudent(struct Student *s)
{
    if (scanf("%d %49s %d %d %d",
              &s->rollNumber,
              s->name,
              &s->marks[0],
              &s->marks[1],
              &s->marks[2]) != 5)
    {
        printf("Invalid student input.\n");
        return 0;
    }

    if (s->rollNumber <= 0)
    {
        printf("Roll number must be positive.\n");
        return 0;
    }

    for (int i = 0; i < SUBJECTS; i++)
    {
        if (s->marks[i] < 0 || s->marks[i] > 100)
        {
            printf("Invalid marks. Each subject must be between 0 and 100.\n");
            return 0;
        }
    }

    return 1;
}

void displayStudentReport(struct Student students[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int total = calculateTotal(&students[i]);
        float average = calculateAverage(total);
        char grade = calculateGrade(average);

        printf("Roll: %d\n", students[i].rollNumber);
        printf("Name: %s\n", students[i].name);
        printf("Total: %d\n", total);
        printf("Average: %.2f\n", average);
        printf("Grade: %c\n", grade);

        if (grade == 'F')
        {
            printf("\n");
            continue;
        }

        printf("Performance: ");
        printStars(grade);
        printf("\n");
    }
}

void printRollNumbers(struct Student students[], int idx, int n)
{
    if (idx >= n)
        return;

    printf("%d", students[idx].rollNumber);

    if (idx < n - 1)
        printf(" ");

    printRollNumbers(students, idx + 1, n);
}

int main(void)
{
    int n;

    printf("Enter number of students: ");

    if (scanf("%d", &n) != 1)
    {
        printf("Invalid input for number of students.\n");
        return 1;
    }

    if (n < 1 || n > MAX_STUDENTS)
    {
        printf("Invalid number of students. Enter a value between 1 and %d.\n",
               MAX_STUDENTS);
        return 1;
    }

    struct Student students[MAX_STUDENTS];

    for (int i = 0; i < n; i++)
    {
        if (!inputStudent(&students[i]))
        {
            return 1;
        }
    }

    printf("\n");
    displayStudentReport(students, n);

    printf("List of roll numbers: ");
    printRollNumbers(students, 0, n);
    printf("\n");

    return 0;
}
