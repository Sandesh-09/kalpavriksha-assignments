#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User
{
    int id;
    char name[50];
    int age;
};

void createUser()
{
    FILE *fp;
    struct User user;

    fp = fopen("users.txt", "a");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &user.id);

    printf("Enter Name: ");
    scanf(" %[^\n]", user.name);

    printf("Enter Age: ");
    scanf("%d", &user.age);

    fprintf(fp, "%d|%s|%d\n", user.id, user.name, user.age);

    fclose(fp);

    printf("User added successfully.\n");
}

void readUsers()
{
    FILE *fp;
    struct User user;

    fp = fopen("users.txt", "r");

    if (fp == NULL)
    {
        printf("No users found.\n");
        return;
    }

    printf("\n ----- Users ----- \n");

    while (fscanf(fp, "%d|%[^|]|%d\n",
                  &user.id, user.name, &user.age) == 3)
    {
        printf("ID: %d, Name: %s, Age: %d\n",
               user.id, user.name, user.age);
    }

    fclose(fp);
}

void updateUser()
{
    FILE *fp;
    FILE *temp;
    struct User user;

    int id;
    int found = 0;

    fp = fopen("users.txt", "r");

    if (fp == NULL)
    {
        printf("No users found.\n");
        return;
    }

    temp = fopen("temp.txt", "w");

    if (temp == NULL)
    {
        printf("Unable to create temporary file.\n");
        fclose(fp);
        return;
    }

    printf("Enter ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d|%[^|]|%d\n",
                  &user.id, user.name, &user.age) == 3)
    {

        if (user.id == id)
        {
            printf("Enter new name: ");
            scanf(" %[^\n]", user.name);

            printf("Enter new age: ");
            scanf("%d", &user.age);

            found = 1;
        }

        fprintf(temp, "%d|%s|%d\n",
                user.id, user.name, user.age);
    }

    fclose(fp);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
    {
        printf("User updated successfully.\n");
    }
    else
    {
        printf("User not found.\n");
    }
}

void deleteUser()
{
    FILE *fp;
    FILE *temp;
    struct User user;

    int id;
    int found = 0;

    fp = fopen("users.txt", "r");

    if (fp == NULL)
    {
        printf("No users found.\n");
        return;
    }

    temp = fopen("temp.txt", "w");

    if (temp == NULL)
    {
        printf("Unable to create temporary file.\n");
        fclose(fp);
        return;
    }

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d|%[^|]|%d\n",
                  &user.id, user.name, &user.age) == 3)
    {

        if (user.id == id)
        {
            found = 1;
            continue;
        }

        fprintf(temp, "%d|%s|%d\n",
                user.id, user.name, user.age);
    }

    fclose(fp);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
    {
        printf("User deleted successfully.\n");
    }
    else
    {
        printf("User not found.\n");
    }
}

int main()
{

    int choice;

    while (1)
    {

        printf("\n User Management System \n");
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {

        case 1:
            createUser();
            break;

        case 2:
            readUsers();
            break;

        case 3:
            updateUser();
            break;

        case 4:
            deleteUser();
            break;

        case 5:
            printf("Program exited.\n");
            return 0;

        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}