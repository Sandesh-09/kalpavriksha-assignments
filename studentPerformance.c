#include <stdio.h>
#define MAX_STUDENTS 100

struct Student
{
    int roll_No;
    char name[50];
    int marks1;
    int marks2;
    int marks3;
};

int calculateTotal(struct Student s)
{
    return s.marks1 + s.marks2 + s.marks3;
}

float calculateAverage(int total)
{
    return total / 3.0;
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

void printStars(char grade)
{
    int stars = 0;

    if (grade == 'A')
        stars = 5;
    else if (grade == 'B')
        stars = 4;
    else if (grade == 'C')
        stars = 3;
    else if (grade == 'D')
        stars = 2;

    for (int i = 0; i < stars; i++)
    {
        printf("*");
    }
    printf("\n");
}

void printRollNumbers(struct Student students[], int idx, int n)
{
    if (idx >= n)
        return;

    printf("%d", students[idx].roll_No);

    if (idx < n - 1)
        printf(" ");

    printRollNumbers(students, idx + 1, n);
}

int main()
{
    int n;
    printf("Enter number of students: ");
    scanf("%d", &n);

    if (n < 1 || n > MAX_STUDENTS)
    {
        printf("Invalid number of students.\n");
        return 1;
    }

    struct Student students[MAX_STUDENTS];

    for (int i = 0; i < n; i++)
    {
        scanf("%d %s %d %d %d", &students[i].roll_No, students[i].name, &students[i].marks1, &students[i].marks2, &students[i].marks3);
    }

    printf("\n");

    for (int i = 0; i < n; i++)
    {
        int total = calculateTotal(students[i]);
        float average = calculateAverage(total);
        char grade = calculateGrade(average);

        printf("Roll: %d\n", students[i].roll_No);
        printf("Name: %s\n", students[i].name);
        printf("Total: %d\n", total);
        printf("Average: %.2f\n", average);
        printf("Grade: %c\n", grade);

        if (average < 35)
        {
            printf("\n");
            continue;
        }

        printf("Performance: ");
        printStars(grade);
        printf("\n");
    }

    printf("List of roll numbers: ");
    printRollNumbers(students, 0, n);
    printf("\n");

    return 0;
}
