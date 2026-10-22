#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define NAME_SIZE 50

struct Employee
{
    int id;
    char name[NAME_SIZE];
    char designation[50];
    int managerId;
    struct Employee *manager;
};

struct Employee *employees[MAX];
int employeeCount = 0;

int readInt(const char *message, int *val)
{
    char input[100];
    char extra;

    printf("%s", message);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 0;

    if (sscanf(input, " %d %c", val, &extra) != 1)
    {
        printf("Invalid integer input.\n");
        return 0;
    }

    return 1;
}

int readString(const char *message, char *str, int size)
{
    printf("%s", message);

    if (fgets(str, size, stdin) == NULL)
        return 0;

    if (strchr(str, '\n') == NULL)
    {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;
    }

    str[strcspn(str, "\n")] = '\0';

    if (str[0] == '\0')
    {
        printf("Input cannot be empty.\n");
        return 0;
    }

    return 1;
}

struct Employee *findEmployee(int id)
{
    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i]->id == id)
            return employees[i];
    }

    return NULL;
}

struct Employee *findCEO(void)
{
    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i]->managerId == -1)
            return employees[i];
    }

    return NULL;
}

void updateManagers(void)
{
    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i]->managerId == -1)
            employees[i]->manager = NULL;
        else
            employees[i]->manager =
                findEmployee(employees[i]->managerId);
    }
}

int countReports(struct Employee *employee)
{
    int count = 0;

    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i]->manager == employee)
            count++;
    }

    return count;
}

int countEmployeesUnder(struct Employee *employee)
{
    if (employee == NULL)
        return 0;

    int total = 0;

    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i]->manager == employee)
        {
            total += 1 + countEmployeesUnder(employees[i]);
        }
    }

    return total;
}

void showHierarchy(struct Employee *employee, int level)
{
    if (employee == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("    ");

    printf("%d - %s - %s\n",
           employee->id,
           employee->name,
           employee->designation);

    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i]->manager == employee)
            showHierarchy(employees[i], level + 1);
    }
}

void addEmployee()
{
    int id, managerId;

    if (employeeCount >= MAX)
    {
        printf("Employee limit reached.\n");
        return;
    }

    if (!readInt("Enter Employee ID: ", &id))
        return;

    if (id <= 0 || findEmployee(id) != NULL)
    {
        printf("Invalid or duplicate Employee ID.\n");
        return;
    }

    if (!readInt("Enter Reporting Manager ID (-1 for CEO): ",
                 &managerId))
        return;

    if (managerId == -1)
    {
        if (employeeCount != 0)
        {
            printf("CEO already exists.\n");
            return;
        }
    }
    else
    {
        if (employeeCount == 0)
        {
            printf("Add the CEO first.\n");
            return;
        }

        if (findEmployee(managerId) == NULL)
        {
            printf("Reporting manager does not exist.\n");
            return;
        }
    }

    struct Employee *e = malloc(sizeof(struct Employee));

    if (e == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    e->id = id;
    e->managerId = managerId;
    e->manager = NULL;

    if (!readString("Enter Name: ", e->name, NAME_SIZE))
    {
        free(e);
        return;
    }

    if (!readString("Enter Designation: ",
                    e->designation, 50))
    {
        free(e);
        return;
    }

    employees[employeeCount] = e;
    employeeCount++;

    updateManagers();

    printf("Employee added successfully.\n");
}

void removeEmployee()
{
    int id;

    if (!readInt("Enter Employee ID to remove: ", &id))
        return;

    int index = -1;

    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i]->id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Employee not found.\n");
        return;
    }

    struct Employee *employee = employees[index];

    if (employee->managerId == -1)
    {
        printf("CEO cannot be removed.\n");
        return;
    }

    struct Employee *superior = employee->manager;
    struct Employee *bestSibling = NULL;
    int minimumReports = MAX + 1;

    for (int i = 0; i < employeeCount; i++)
    {
        struct Employee *sibling = employees[i];

        if (sibling != employee &&
            sibling->manager == superior)
        {
            int reports = countReports(sibling);

            if (reports < minimumReports)
            {
                minimumReports = reports;
                bestSibling = sibling;
            }
        }
    }

    struct Employee *destination;

    if (bestSibling != NULL)
        destination = bestSibling;
    else
        destination = superior;

    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i]->managerId == id)
        {
            employees[i]->managerId = destination->id;
        }
    }

    for (int i = index; i < employeeCount - 1; i++)
    {
        employees[i] = employees[i + 1];
    }

    employeeCount--;

    free(employee);

    updateManagers();

    printf("Employee removed successfully.\n");
    printf("Direct reports transferred to employee ID %d.\n",
           destination->id);
}

void displayEmployeeHierarchy(void)
{
    int id;

    if (!readInt("Enter Employee ID: ", &id))
        return;

    struct Employee *employee = findEmployee(id);

    if (employee == NULL)
    {
        printf("Employee not found.\n");
        return;
    }

    printf("\n===== EMPLOYEE HIERARCHY =====\n");

    showHierarchy(employee, 0);

    printf("Total Employees Under %d: %d\n",
           id, countEmployeesUnder(employee));
}

void displayCompleteHierarchy(void)
{
    struct Employee *ceo = findCEO();

    if (ceo == NULL)
    {
        printf("Organization is empty.\n");
        return;
    }

    printf("\n===== ORGANIZATION HIERARCHY =====\n");

    showHierarchy(ceo, 0);

    printf("Total Employees: %d\n", employeeCount);
}

void freeOrganization(void)
{
    for (int i = 0; i < employeeCount; i++)
    {
        free(employees[i]);
        employees[i] = NULL;
    }

    employeeCount = 0;
}

int main(void)
{
    int choice = 0;

    do
    {
        printf("\n===== EMPLOYEE HIERARCHY MANAGEMENT =====\n");
        printf("1. Add Employee\n");
        printf("2. Remove Employee\n");
        printf("3. Display Employee Hierarchy\n");
        printf("4. Display Complete Hierarchy\n");
        printf("5. Exit\n");

        if (!readInt("Enter your choice: ", &choice))
        {
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            addEmployee();
            break;

        case 2:
            removeEmployee();
            break;

        case 3:
            displayEmployeeHierarchy();
            break;

        case 4:
            displayCompleteHierarchy();
            break;

        case 5:
            freeOrganization();
            printf("Program terminated.\n");
            break;

        default:
            printf("Choose a number from 1 to 5.\n");
        }

    } while (choice != 5);

    return 0;
}
