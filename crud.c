#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "users.txt"
#define TEMP_FILE "temp.txt"
#define BACKUP_FILE "backup.txt"

struct User
{
    int id;
    char name[50];
    int age;
};

int readInt(const char *message)
{
    char input[50];
    char extra;
    int value;

    while (1)
    {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error.\n");
            continue;
        }

        if (sscanf(input, "%d %c", &value, &extra) == 1)
        {
            return value;
        }

        printf("Invalid input. Please enter an integer.\n");
    }
}

void readName(char *name)
{
    while (1)
    {
        if (fgets(name, 50, stdin) == NULL)
        {
            printf("Input error.\n");
            continue;
        }

        if (strchr(name, '\n') != NULL)
        {
            name[strcspn(name, "\n")] = '\0';

            if (strlen(name) > 0)
            {
                return;
            }
        }
        else
        {
            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF)
            {
            }

            printf("Name is too long. Maximum 49 characters allowed.\n");
        }

        printf("Enter Name: ");
    }
}

int readUser(FILE *fp, struct User *user)
{
    return fscanf(fp, "%d|%49[^|]|%d\n",
                  &user->id, user->name, &user->age) == 3;
}

int writeUser(FILE *fp, struct User *user)
{
    return fprintf(fp, "%d|%s|%d\n",
                   user->id, user->name, user->age) >= 0;
}

int userExists(int id)
{
    FILE *fp;
    struct User user;

    fp = fopen(FILE_NAME, "r");

    if (fp == NULL)
    {
        return 0;
    }

    while (readUser(fp, &user))
    {
        if (user.id == id)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

int replaceFile(void)
{
    remove(BACKUP_FILE);

    if (rename(FILE_NAME, BACKUP_FILE) != 0)
    {
        printf("Failed to backup original file.\n");
        return 0;
    }

    if (rename(TEMP_FILE, FILE_NAME) != 0)
    {
        printf("Failed to replace users file.\n");

        if (rename(BACKUP_FILE, FILE_NAME) != 0)
        {
            printf("Failed to restore original file.\n");
        }

        return 0;
    }

    remove(BACKUP_FILE);
    return 1;
}

void createUser()
{
    FILE *fp;
    struct User user;

    user.id = readInt("Enter ID: ");

    if (userExists(user.id))
    {
        printf("ID already exists. User was not added.\n");
        return;
    }

    printf("Enter Name: ");
    readName(user.name);

    user.age = readInt("Enter Age: ");

    fp = fopen(FILE_NAME, "a");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    if (!writeUser(fp, &user))
    {
        printf("Failed to write user data.\n");
        fclose(fp);
        return;
    }

    if (fclose(fp) != 0)
    {
        printf("Failed to close file.\n");
        return;
    }

    printf("User added successfully.\n");
}

void readUsers()
{
    FILE *fp;
    struct User user;

    fp = fopen(FILE_NAME, "r");

    if (fp == NULL)
    {
        printf("No users found.\n");
        return;
    }

    printf("\n----- Users -----\n");

    while (readUser(fp, &user))
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

    fp = fopen(FILE_NAME, "r");

    if (fp == NULL)
    {
        printf("No users found.\n");
        return;
    }

    temp = fopen(TEMP_FILE, "w");

    if (temp == NULL)
    {
        printf("Unable to create temporary file.\n");
        fclose(fp);
        return;
    }

    id = readInt("Enter ID to update: ");

    while (readUser(fp, &user))
    {
        if (user.id == id)
        {
            printf("Enter new name: ");
            readName(user.name);

            user.age = readInt("Enter new age: ");
            found = 1;
        }

        if (!writeUser(temp, &user))
        {
            printf("Failed to write temporary data.\n");
            fclose(fp);
            fclose(temp);
            remove(TEMP_FILE);
            return;
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found)
    {
        remove(TEMP_FILE);
        printf("User not found.\n");
        return;
    }

    if (replaceFile())
    {
        printf("User updated successfully.\n");
    }
}

void deleteUser()
{
    FILE *fp;
    FILE *temp;
    struct User user;

    int id;
    int found = 0;

    fp = fopen(FILE_NAME, "r");

    if (fp == NULL)
    {
        printf("No users found.\n");
        return;
    }

    temp = fopen(TEMP_FILE, "w");

    if (temp == NULL)
    {
        printf("Unable to create temporary file.\n");
        fclose(fp);
        return;
    }

    id = readInt("Enter ID to delete: ");

    while (readUser(fp, &user))
    {
        if (user.id == id)
        {
            found = 1;
            continue;
        }

        if (!writeUser(temp, &user))
        {
            printf("Failed to write temporary data.\n");
            fclose(fp);
            fclose(temp);
            remove(TEMP_FILE);
            return;
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found)
    {
        remove(TEMP_FILE);
        printf("User not found.\n");
        return;
    }

    if (replaceFile())
    {
        printf("User deleted successfully.\n");
    }
}

int main()
{
    int choice;

    while (1)
    {
        printf("\n----- User Management System -----\n");
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");

        choice = readInt("Enter your choice: ");

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
}
