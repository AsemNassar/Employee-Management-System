#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

// Global variables for time-related operations

time_t t;
struct tm *local;

// Maximum number of employees the system can handle

#define MAX_EMPLOYEES 400

// ANSI color codes for terminal text formatting

#define RED "\x1b[38;5;203m"
#define GREEN "\x1b[38;5;42m"
#define YELLOW "\x1b[1;33m"
#define RESET "\x1b[0m"

typedef struct
{
    int day;
    int month;
    int year;
} date;

typedef struct
{
    char ID[20];
    char name[100];
    char salary[1000];
    date birth;
    char address[200];
    char phone[20];
    date En_date;
    char email[100];
} st_Employee;

// Array to store employee data
st_Employee Data_B_Employee[MAX_EMPLOYEES];

// Variable to keep track of the number of employees
int Num_of_Employee = 0;

// Function prototypes
void greet_based_on_time();
void load_from_file(const char *filename);
void clear_buffer();
void queryEmployee(char *s);
void addEmployee();
void validateID(char id[], int size);
void validate_ID_search(char id[], int size);
int Is_ID_Duplicate(const char id[]);
void FullNameValidation(char fullname[100], int size);
int isValidSalary(char num[]);
void SalaryValidation(char num[]);
void DateValidation(int *D, int *M, int *Y);
void AddressValidation(char address[], int size);
void PhoneNumberValidation(char phonenumber[100], int size, int current_index);
int Is_Ph_Duplicate(const char phonenumber[], int current_index);
int EmailValidation(char *email);
int Is_E_M_Duplicate(char email[], int current_index);
void getEmailAndValidate(char email[], int size, int index);
void deleteEmployee(char *W_ID);
void modifyEmployee(char W_ID[]);
void printEmployees(char sortBy);
void sortByName();
float GetSalaryAsFloat(const char *salary);
void SortBySalary();
void SortByDOB();
void saveToFile(const char *fileName);
int confirmQuit();

int main()
{
    greet_based_on_time();

    char filename[1000];
    char id_delete[20];
    char S_ID[20];
    char search[100];
    char sortBy;
    int choice;

    printf("\nEnter the Filename to load from [if exists]: ");
    int flag = 0;
    do
    {
        if (scanf("%99s", filename) != 1)
        {
            printf(RED "Error reading file name\n" RESET);
            clear_buffer();
            flag = 1;
        }
        else
        {
            flag = 0;
        }
    } while (flag);
    load_from_file(filename);
    clear_buffer();
    if (Num_of_Employee > 0)
    {
        printf(GREEN "\nLoaded %d Employees.\n" RESET, Num_of_Employee);
    }
    do
    {
        printf("\nMenu:\n");
        printf("1. Add Employee\n");
        printf("2. Delete Employee\n");
        printf("3. Modify Employee\n");
        printf("4. Search Employee\n");
        printf("5. Print Employees\n");
        printf("6. Save Employees to File\n");
        printf("7. Quit\n");
        printf("Enter your choice (1-7): ");

        if (scanf("%d", &choice) != 1)
        {
            clear_buffer();
            printf(RED "Invalid input...please enter an integer value.\n" RESET);
            continue;
        }

        switch (choice)
        {
        case 1:
            printf(GREEN "\nSelected: Add Employee\n" RESET);
            addEmployee();
            break;
        case 2:

            printf(GREEN "\nSelected: Delete Employee\n" RESET);

            validate_ID_search(id_delete, sizeof(id_delete));
            deleteEmployee(id_delete);
            break;
        case 3:
            printf(GREEN "\nSelected: Modify Employee\n\n" RESET);

            validate_ID_search(S_ID, sizeof(S_ID));

            modifyEmployee(S_ID);
            printf(GREEN "Data updated sucessfully." RESET);
            break;
        case 4:

            printf(GREEN "\nSelected: Search Employee\n" RESET);
            printf("Enter the employee's name: ");
            clear_buffer();
            if (fgets(search, sizeof(search), stdin) == NULL)
            {
                clear_buffer();
            }
            if (search[0] == '\n' || search[0] == ' ')
            {
                printf(RED "Invalid input...please enter a character value.\n" RESET);
                break;
            }
            printf("\n");
            sleep(1);
            queryEmployee(search);
            sleep(1);
            break;
        case 5:

            printf(GREEN "Selected: Print Employees\n" RESET);
            printf("Sort by (N)ame, (D)ate of Birth, or (S)alary: ");
            if (scanf(" %c", &sortBy) != 1)
            {
                clear_buffer();
                printf(RED "Invalid input...please enter a character value['N','D','S'].\n" RESET);
            }

            printEmployees(sortBy);
            sleep(1);
            break;
        case 6:

            printf(GREEN "Selected: Save Employees to File\n");
            saveToFile(filename);
            break;
        case 7:
            if (confirmQuit())
            {
                return 0;
            }
            break;

        default:
            printf(RED "\nInvalid input...try again.\n" RESET);
            break;
        }
    } while (1);

    return 0;
}

void greet_based_on_time()
{
    time_t t;
    struct tm *currentTime;

    // Get the current time
    time(&t);
    currentTime = localtime(&t);

    // Extract the hour from the current time
    int hour = currentTime->tm_hour;

    // Print greetings based on the current time
    if (hour >= 0 && hour < 12)
    {
        printf(GREEN "Good Morning! \n" RESET);
    }
    else if (hour >= 12 && hour < 18)
    {
        printf(GREEN "Good Afternoon!\n" RESET);
    }
    else
    {
        printf(GREEN "Good Evening!\n" RESET);
    }
}

void load_from_file(const char *filename)
{
    FILE *originalFile = fopen(filename, "r");
    if (originalFile == NULL)
    {

        printf(YELLOW "\n %s Does not exist\n creating %s\n" RESET, filename, filename);
        originalFile = fopen(filename, "w");
        if (originalFile == NULL)
        {
            printf(RED "Error creating %s file" RESET, filename);
            exit(2);
        }
        fclose(originalFile);
        return;
    }

    while (fscanf(originalFile, "%[^,],", Data_B_Employee[Num_of_Employee].ID) == 1)
    {
        fscanf(originalFile, "%[^,],", Data_B_Employee[Num_of_Employee].name);
        fscanf(originalFile, "%[^,],", Data_B_Employee[Num_of_Employee].salary);
        fscanf(originalFile, "%d-", &Data_B_Employee[Num_of_Employee].birth.day);
        fscanf(originalFile, "%d-", &Data_B_Employee[Num_of_Employee].birth.month);
        fscanf(originalFile, "%d,", &Data_B_Employee[Num_of_Employee].birth.year);
        fscanf(originalFile, "%[^,],", Data_B_Employee[Num_of_Employee].address);
        fscanf(originalFile, "%[^,],", Data_B_Employee[Num_of_Employee].phone);
        fscanf(originalFile, "%d-", &Data_B_Employee[Num_of_Employee].En_date.day);
        fscanf(originalFile, "%d-", &Data_B_Employee[Num_of_Employee].En_date.month);
        fscanf(originalFile, "%d,", &Data_B_Employee[Num_of_Employee].En_date.year);
        fscanf(originalFile, "%[^\n]\n", Data_B_Employee[Num_of_Employee].email);
        ++Num_of_Employee;
    }

    fclose(originalFile);
}

void clear_buffer()
{
    int clear;
    while ((clear = getchar()) != '\n' && clear != EOF)
        ;
}

void queryEmployee(char *s)
{
    s[strcspn(s, "\n")] = '\0'; // Remove the newline character
    int flag = 0;

    for (int i = 0; i < Num_of_Employee; i++)
    {

        if (strstr(Data_B_Employee[i].name, s) != NULL)
        {
            printf(" %s, %s , %s , %d-%d-%d , %s , %s , %d-%d-%d , %s \n", Data_B_Employee[i].ID, Data_B_Employee[i].name, Data_B_Employee[i].salary, Data_B_Employee[i].birth.day, Data_B_Employee[i].birth.month, Data_B_Employee[i].birth.year, Data_B_Employee[i].address, Data_B_Employee[i].phone, Data_B_Employee[i].En_date.day, Data_B_Employee[i].En_date.month, Data_B_Employee[i].En_date.year, Data_B_Employee[i].email);
            flag = 1;
        }
    }
    if (flag == 0)
    {
        printf(RED "\n Employee not found" RESET);
    }
}

void addEmployee()
{
    int i = Num_of_Employee;
    t = time(NULL);
    local = localtime(&t);
    Data_B_Employee[i].En_date.year = local->tm_year + 1900;
    Data_B_Employee[i].En_date.month = local->tm_mon + 1;
    Data_B_Employee[i].En_date.day = local->tm_mday;

    clear_buffer();

    validateID(Data_B_Employee[i].ID, sizeof(Data_B_Employee[i].ID));

    FullNameValidation(Data_B_Employee[i].name, sizeof(Data_B_Employee[i].name));

    SalaryValidation(Data_B_Employee[i].salary);

    DateValidation(&Data_B_Employee[i].birth.day, &Data_B_Employee[i].birth.month, &Data_B_Employee[i].birth.year);
    clear_buffer();
    AddressValidation(Data_B_Employee[i].address, sizeof(Data_B_Employee[i].address));

    PhoneNumberValidation(Data_B_Employee[i].phone, sizeof(Data_B_Employee[i]), i);

    getEmailAndValidate(Data_B_Employee[i].email, sizeof(Data_B_Employee[i].email), i);

    Num_of_Employee = Num_of_Employee + 1;
    printf(GREEN "\nEmployee added successfully.\n" RESET);
}

void validateID(char id[], int size)
{

    while (1)
    {
        printf("Enter ID : ");

        if (fgets(id, size, stdin) == NULL)
        {
            clear_buffer();
        }

        id[strcspn(id, "\n")] = '\0'; // Remove the newline character

        if (strcmp(id, "0") == 0)
        {
            printf(RED "ID cannot equal 0. Re-enter the ID.\n" RESET);
        }
        else if (strlen(id) == 0)
        {
            printf(RED "Please enter a valid ID.\n" RESET);
        }
        else if (strlen(id) > 8)
        {
            printf(RED "ID must not exceed 7 digits. Validation failed.\n" RESET);
        }
        else if (Is_ID_Duplicate(id))
        {
            printf(RED "ID already exists\n" RESET);
        }
        else
        {
            int flag = 1;
            for (int i = 0; id[i] != '\0'; i++)
            {
                if (!isdigit(id[i]) || isspace(id[i]))
                {
                    printf(RED "Please enter a valid ID.\n" RESET);
                    flag = 0;
                    break;
                }
            }

            if (flag == 1)
            {
                break;
            }
        }
    }
}
void validate_ID_search(char id[], int size)
{
    clear_buffer();
    while (1)
    {
        printf("Enter ID : ");

        if (fgets(id, size, stdin) == NULL)
        {
            clear_buffer();
        }
        printf("\n");
        id[strcspn(id, "\n")] = '\0'; // Remove the newline character

        if (strcmp(id, "0") == 0)
        {
            printf(RED "ID cannot equal 0. Re-enter the ID.\n" RESET);
        }
        else if (strlen(id) == 0)
        {
            printf(RED "Please enter a valid ID.\n" RESET);
        }
        else if (strlen(id) > 8)
        {
            printf(RED "ID must not exceed 7 digits. Validation failed.\n" RESET);
        }

        else
        {
            int flag = 1;
            for (int i = 0; id[i] != '\0'; i++)
            {
                if (!isdigit(id[i]) || isspace(id[i]))
                {
                    printf(RED "Please enter a valid ID.\n" RESET);
                    flag = 0;
                    break;
                }
            }

            if (flag == 1)
            {
                break;
            }
        }
    }
}
int Is_ID_Duplicate(const char id[])
{
    for (int i = 0; i < Num_of_Employee; i++)
    {
        if (strcmp(id, Data_B_Employee[i].ID) == 0)
        {
            return 1;
        }
    }
    return 0;
}
void FullNameValidation(char fullname[100], int size)
{
    int flag = 0;

    while (flag == 0)
    {
        printf("Enter the name:");

        if (fgets(fullname, size, stdin) == NULL)
        {
            clear_buffer();
        }

        fullname[strcspn(fullname, "\n")] = '\0'; // Remove the newline character

        // Check if the string has at least two characters
        if (strlen(fullname) < 2)
        {
            flag = 0;
            printf(RED "Please enter a valid full name with at least 2 characters.\n" RESET);
        }
        else
        {
            flag = 1;
            for (int i = 0; fullname[i] != '\0'; i++)
            {
                if (!(isalpha(fullname[i])) && !isspace(fullname[i]))
                {
                    flag = 0;
                    printf(RED "Please enter a valid full name.\n" RESET);
                    break;
                }
            }
        }
    }
}

int isValidSalary(char num[])
{
    int i;
    int dotCount = 0;

    for (i = 0; num[i] != '\0'; i++)
    {
        if (!isdigit(num[i]))
        {
            if (num[i] == '.' && dotCount == 0)
            {
                dotCount++;
            }
            else
            {
                return 0;
            }
        }
    }

    return 1;
}

void SalaryValidation(char num[])
{
    int flag = 0;

    while (flag == 0)
    {
        printf("Enter employee's salary: ");
        scanf("%s", num);

        if (isValidSalary(num))
        {
            flag = 1;
        }
        else
        {
            printf(RED "Please enter a valid salary without any special characters except '.'\n" RESET);
        }
        if (num[0] == '0')
        {
            printf(RED "Please enter a valid salary\n" RESET);
            flag = 0;
        }
    }
}

void DateValidation(int *D, int *M, int *Y)
{
    int flag = 0;
    while (flag == 0)
    {
        printf("Enter Date of birth (DD-MM-YYYY): ");
        if (scanf("%d-%d-%d", D, M, Y) != 3)
        {
            clear_buffer();
            printf(RED "Invalid input! Please enter numeric values.\n" RESET);
            continue;
        }
        if ((*D > 31 || *D <= 0) || (*M > 12 || *M <= 0) || (*Y < 1963 || *Y >= 2023))
        {
            printf(RED "Invalid date! Please re-enter.\n" RESET);
        }
        else if (*D > 30 && (*M == 4 || *M == 6 || *M == 9 || *M == 11))
        {
            printf(RED "Invalid date! Please re-enter.\n" RESET);
        }
        else if (*D > 28 && *M == 2)
        {
            printf(RED "Invalid date! Please re-enter.\n" RESET);
        }
        else
        {
            flag = 1;
        }
    }
}

void AddressValidation(char address[], int size)
{

    int i, flag = 0;
    while (flag == 0)
    {
        printf("Enter the employee's address:");

        if (fgets(address, size, stdin) == NULL)
        {
            clear_buffer();
        }
        address[strcspn(address, "\n")] = '\0';

        for (i = 0; address[i]; i++)
        {
            if (isalpha(address[i]))
            {
                if (isalpha(address[i]) || isdigit(address[i]))
                {
                    flag = 1;
                    break;
                }
                else if (!isalpha(address[i]) || isdigit(address[i]))
                {
                    flag = 0;
                    break;
                }
            }
        }
    }
}

void PhoneNumberValidation(char phonenumber[100], int size, int current_index)
{
    int i, flag = 0;

    while (flag == 0)
    {
        printf("Enter the employee's phone number: ");
        if (fgets(phonenumber, size, stdin) == NULL)
        {
            clear_buffer();
        }
        phonenumber[strcspn(phonenumber, "\n")] = '\0'; // function for removing \n
        flag = 1;

        if (strlen(phonenumber) != 11)
        {
            flag = 0;
        }

        if (Is_Ph_Duplicate(phonenumber, current_index))
        {
            flag = 0;
            printf(RED "Phone number is duplicated. Please try again.\n" RESET);
            continue;
        }
        else
        {
            for (i = 0; phonenumber[i] != '\0'; i++)
            {
                if (!isdigit(phonenumber[i]))
                {
                    flag = 0;
                    break;
                }
            }
        }

        if (flag == 0)
        {
            printf(RED "Please enter a valid 11-digit phone number.\n" RESET);
        }
    }
}

int Is_Ph_Duplicate(const char phonenumber[], int current_index)
{
    for (int i = 0; i < Num_of_Employee; i++)
    {
        // Skip checking for duplicate at the current index and newly added employees
        if (i == current_index || i >= Num_of_Employee - 1)
        {
            continue;
        }

        if (strcmp(phonenumber, Data_B_Employee[i].phone) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int EmailValidation(char *email)
{
    int atCount = 0;
    unsigned long i;

    for (i = 0; i < strlen(email); i++)
    {
        if (email[i] == '@')
        {
            atCount++;
        }

        if (email[i] == ' ' || email[i] == '/' || email[i] == ':' || email[i] == ';' || email[i] == '<' || email[i] == '>' || email[i] == ',' || email[i] == '[' || email[i] == ']')
        {
            return 0;
        }
    }

    if (atCount == 1)
    {
        if (email[0] != '@')
        {
            char *dot = strchr(email, '.');

            if (dot != NULL && dot > strchr(email, '@'))
            {
                return 1;
            }
        }
    }
    return 0;
}
int Is_E_M_Duplicate(char email[], int current_index)
{
    for (int i = 0; i < Num_of_Employee; i++)
    {

        if (i == current_index || i >= Num_of_Employee - 1)
        {
            continue;
        }

        if (strcmp(email, Data_B_Employee[i].email) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void getEmailAndValidate(char email[], int size, int index)
{
    do
    {

        printf("Enter employee's email address: ");
        if (fgets(email, size, stdin) == NULL)
        {
            clear_buffer();
        }
        email[strcspn(email, "\n")] = '\0'; // function for removing \n

        int statusOfValidation = 0;
        statusOfValidation = EmailValidation(email);

        if (statusOfValidation == 0)
        {
            printf(RED "Invalid email address. Please try again.\n" RESET);
            continue;
        }
        int statusOfValidation2 = 0;
        statusOfValidation2 = Is_E_M_Duplicate(email, index);
        if (statusOfValidation2 == 1)
        {
            printf(RED "duplicate email address. Please try again.\n" RESET);
            continue;
        }
        break;

    } while (1);
}

void deleteEmployee(char *W_ID)
{
    int found = 0;

    for (int i = 0; i < Num_of_Employee; i++)
    {
        if (strcmp(Data_B_Employee[i].ID, W_ID) == 0)
        {
            found = 1;

            for (int j = i; j < Num_of_Employee - 1; j++)
            {
                Data_B_Employee[j] = Data_B_Employee[j + 1];
            }

            memset(&Data_B_Employee[Num_of_Employee - 1], 0, sizeof(st_Employee));

            Num_of_Employee--;
            break;
        }
    }

    if (found)
    {

        printf(GREEN "Employee with ID %s deleted successfully.\n" RESET, W_ID);
    }
    else
    {
        printf(RED "Employee with ID %s not found.\n" RESET, W_ID);
    }
    sleep(1);
}

void modifyEmployee(char W_ID[])
{
    int found = 0;
    int choice;

    for (int i = 0; i < Num_of_Employee; i++)
    {
        if (strcmp(Data_B_Employee[i].ID, W_ID) == 0)
        {
            found = 1;
            sleep(1);
            printf("Current details for Employee with ID %s:\n\n", W_ID);
            printf("Name: %s\n", Data_B_Employee[i].name);
            printf("Salary: %s\n", Data_B_Employee[i].salary);
            printf("Birth Date: %d-%d-%d\n", Data_B_Employee[i].birth.day, Data_B_Employee[i].birth.month, Data_B_Employee[i].birth.year);
            printf("En_date Date: %d-%d-%d\n", Data_B_Employee[i].En_date.day, Data_B_Employee[i].En_date.month, Data_B_Employee[i].En_date.year);
            printf("Address: %s\n", Data_B_Employee[i].address);
            printf("Phone: %s\n", Data_B_Employee[i].phone);
            printf("Email: %s\n\n", Data_B_Employee[i].email);

            do
            {
                sleep(1);
                printf("\n\nSelect field to modify:\n");
                printf("1. Full Name\n");
                printf("2. Salary\n");
                printf("3. Mobile Number\n");
                printf("4. Address\n");
                printf("5. Email\n");
                printf("6. Back to main menu\n");
                printf("Enter your choice (1-6): ");
                if (scanf("%d", &choice) != 1)
                {
                    clear_buffer();
                    printf(RED "Enter an integer value please..." RESET);
                    continue;
                }
                switch (choice)
                {
                case 1:

                    clear_buffer();
                    FullNameValidation(Data_B_Employee[i].name, sizeof(Data_B_Employee[i].name));
                    printf(GREEN "Modified\n" RESET);
                    break;
                case 2:
                    clear_buffer();
                    SalaryValidation(Data_B_Employee[i].salary);
                    printf(GREEN "Modified\n" RESET);
                    break;
                case 3:
                    clear_buffer();
                    PhoneNumberValidation(Data_B_Employee[i].phone, sizeof(Data_B_Employee[i].phone), i);
                    printf(GREEN "Modified\n" RESET);
                    break;
                case 4:
                    clear_buffer();
                    AddressValidation(Data_B_Employee[i].address, sizeof(Data_B_Employee[i].address));
                    printf(GREEN "Modified\n" RESET);
                    break;
                case 5:
                    clear_buffer();

                    getEmailAndValidate(Data_B_Employee[i].email, sizeof(Data_B_Employee[i].email), i);
                    printf(GREEN "Modified\n" RESET);
                    break;
                case 6:

                    return;
                default:
                    printf(RED "Invalid choice. No modifications made.\n" RESET);
                    break;
                }
            } while (choice != 6);

            break;
        }
    }
    if (!found)
    {
        printf(RED "Employee with ID %s not found.\n" RESET, W_ID);
    }
}

void printEmployees(char sortBy)
{
    // Convert sortBy to uppercase
    sortBy = toupper((unsigned char)sortBy);

    if (Num_of_Employee == 0)
    {
        printf(RED "No employees to print.\n" RESET);
        return;
    }

    switch (sortBy)
    {
    case 'N':
        sortByName();
        break;
    case 'D':

        SortByDOB();
        break;
    case 'S':

        SortBySalary();
        break;
    default:
        printf("Invalid sorting option.\n");
        return;
    }
    printf("\n");
    sleep(1);
    for (int i = 0; i < Num_of_Employee; i++)
    {
        printf("%s, %s, %s,%d-%d-%d, %s, %s,%d-%d-%d, %s\n", Data_B_Employee[i].ID,
               Data_B_Employee[i].name, Data_B_Employee[i].salary, Data_B_Employee[i].birth.day,
               Data_B_Employee[i].birth.month, Data_B_Employee[i].birth.year, Data_B_Employee[i].address,
               Data_B_Employee[i].phone, Data_B_Employee[i].En_date.day, Data_B_Employee[i].En_date.month,
               Data_B_Employee[i].En_date.year, Data_B_Employee[i].email);
    }
    sleep(1);
}

void sortByName()
{
    st_Employee temp;
    for (int i = 0; i < Num_of_Employee - 1; i++)
    {
        for (int j = i + 1; j < Num_of_Employee; j++)
        {
            if (strcmp(Data_B_Employee[i].name, Data_B_Employee[j].name) > 0)
            {
                temp = Data_B_Employee[i];
                Data_B_Employee[i] = Data_B_Employee[j];
                Data_B_Employee[j] = temp;
            }
        }
    }
}

float GetSalaryAsFloat(const char *salary)
{
    return atof(salary);
}

void SortBySalary()
{
    st_Employee temp;
    for (int i = 0; i < Num_of_Employee - 1; i++)
    {
        for (int j = i + 1; j < Num_of_Employee; j++)
        {
            // Compare salaries as floats
            float salary1 = GetSalaryAsFloat(Data_B_Employee[i].salary);
            float salary2 = GetSalaryAsFloat(Data_B_Employee[j].salary);

            // Compare in descending order
            if (salary1 < salary2)
            {
                temp = Data_B_Employee[i];
                Data_B_Employee[i] = Data_B_Employee[j];
                Data_B_Employee[j] = temp;
            }
        }
    }
}

void SortByDOB()
{
    st_Employee temp;
    for (int i = 0; i < Num_of_Employee - 1; i++)
    {
        for (int j = i + 1; j < Num_of_Employee; j++)
        {
            if (Data_B_Employee[i].birth.year > Data_B_Employee[j].birth.year)
            {
                temp = Data_B_Employee[i];
                Data_B_Employee[i] = Data_B_Employee[j];
                Data_B_Employee[j] = temp;
            }
            else if (Data_B_Employee[i].birth.year == Data_B_Employee[j].birth.year)
            {
                if (Data_B_Employee[i].birth.month > Data_B_Employee[j].birth.month)
                {
                    temp = Data_B_Employee[i];
                    Data_B_Employee[i] = Data_B_Employee[j];
                    Data_B_Employee[j] = temp;
                }
                else if (Data_B_Employee[i].birth.month == Data_B_Employee[j].birth.month)
                {
                    if (Data_B_Employee[i].birth.day > Data_B_Employee[j].birth.day)
                    {
                        temp = Data_B_Employee[i];
                        Data_B_Employee[i] = Data_B_Employee[j];
                        Data_B_Employee[j] = temp;
                    }
                }
            }
        }
    }
}

void saveToFile(const char *filename)
{

    FILE *save_File = fopen(filename, "w");

    if (save_File == NULL)
    {
        printf(RED "Error opening file" RESET);
        exit(4);
    }

    for (int i = 0; i < Num_of_Employee; i++)
    {

        if (strcmp(Data_B_Employee[i].ID, "NULL") == 0)
        {
            continue;
        }
        else
        {
            fprintf(save_File, "%s,%s,%s,%d-%d-%d,%s,%s,%d-%d-%d,%s", Data_B_Employee[i].ID,
                    Data_B_Employee[i].name, Data_B_Employee[i].salary, Data_B_Employee[i].birth.day,
                    Data_B_Employee[i].birth.month, Data_B_Employee[i].birth.year, Data_B_Employee[i].address,
                    Data_B_Employee[i].phone, Data_B_Employee[i].En_date.day, Data_B_Employee[i].En_date.month,
                    Data_B_Employee[i].En_date.year, Data_B_Employee[i].email);

            if (i < Num_of_Employee - 1)
            {
                fprintf(save_File, "\n");
            }
        }
    }
    fclose(save_File);
    sleep(1);
    printf(GREEN "\nSuccessfully Saved to file.\n" RESET);
}

int confirmQuit()
{
    char choice;

    printf(YELLOW "!! Make sure to save your progress. !!\n");
    printf("Are you sure you want to quit?\n" RESET);
    printf("Enter (Y) Yes, (N) No: ");
    clear_buffer();

    if (scanf(" %c", &choice) != 1)
    {
        clear_buffer();
        printf(RED "\nPlease enter 'Y' or 'N' only\n" RESET);
        return 0; // Retry the quit confirmation
    }

    if (choice == 'Y' || choice == 'y')
    {
        printf(GREEN "Quitting the program...\n" RESET);
        return 1; // Quit the program
    }
    else if (choice == 'N' || choice == 'n')
    {
        return 0; // Continue with the program
    }
    else
    {
        printf(RED "\nInvalid input...Back to the main menu.\n" RESET);
        return 0; // Retry the quit confirmation
    }
}