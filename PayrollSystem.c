/**************************************************************
 *
 * CIS 2206 Practice Activity #1 - Payroll System using C
 * Created by: Team 1
 * Members:
 * Date: March 14-21, 2022
 *
 * ************************************************************/

/***** HEADER FILES TO INCLUDE *****/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>

/***** CONSTANTS *****/
#define SIZE 512 // hash table size
#define EMP_FILENAME "_EmployeeFile.txt"
#define ATT_FILENAME "_AttendanceFile.txt"
#define PAY_FILENAME "_PayrollIDList.txt"
/* Note: Actual file names will be '<CompanyName>_EmployeeFile.bin'.
 * Company name will be inputted at main menu upon program and data initialization.
 */

/***** DATA STRUCTURE DEFINITION *****/
typedef struct
{
    char LName[16]; // last name
    char fName[24]; // first name
    char MI;        // middle initial
} nameDetails;

typedef enum
{
    ACTIVE,  // only active employees will be prompted attendance entry
    INACTIVE // in the specified group for each new payroll entry
} empStatus;

typedef struct
{
    unsigned int year : 7;  // YY
    unsigned int month : 4; // MM
    unsigned int day : 5;   // DD
} dateDetails;              // CONSTRAINT: No negative values

typedef struct
{
    char email[32]; // must have '@' and '.' for valid email
    char phone[12]; // 11-digit mobile number
} contactDetails;

typedef struct
{
    float basicSalary;   // basic monthly salary, in PHP
    float overtimePay;   // overtime monthly pap, in PHP
    float contributions; // total government contributions, in PHP
} paymentDetails;        // CONSTRAINT: No negative values

typedef struct
{
    char empID[8];     // employee ID (CONSTRAINT: Must match an existing employee ID)
    char payrollID[6]; // payroll ID  (CONSTRAINT: See format below)
    int daysAbsent;    // number of days absent for the period
    int hoursOvertime; // overtime duration for the period, in hours
    int minsUndertime; // undertime duration for the period, in minutes
} attendanceDetails;   // the structure to be written in the 'EmployeeFile.bin'

/* Payroll ID is YYMMPG - Year, Month, Period, Group #
 * Example:      22011A - Period 1, January 2022, Group A
 */

/* Linked list for attendance details */
typedef struct cell
{
    attendanceDetails attendance;
    struct cell *link;
} cellType, *attendanceHistory, *List;

/* Employee structure, with attendance details LL pointer */
typedef struct
{
    char empID[8]; // employee ID (7 characters)
                   // for hash table: Dummy values - "EMPTY" and "DELETED"
    nameDetails name;
    contactDetails contact;
    dateDetails dateEmployed;
    empStatus status;
    char department;
    paymentDetails details;
} employeeDetails; // the structure to be written in the 'EmployeeFile.bin'

typedef struct
{
    employeeDetails employee;  // employee details
    attendanceHistory history; // head pointer of the attendance linked list
} employeeInfo;

/* Hash table of employees - CLOSED HASHING WITH MAX BUCKETS, WITH LINEAR PROBING
 *  I chose closed hashing ra para sayun ma clear ang memory by avoiding dynamic cells,
    since when we terminate a program we have to free all dynamic memory
 */
typedef employeeInfo employeeTable[SIZE]; // constant hash table

typedef struct node
{
    int index;
    struct node *link;
} nodeType, *employeeList[26], *List2; // the structure for sorting the employees by department

/***** FUNCTION PROTOTYPES *****/
void initialize(employeeTable empTable, char companyName[]);
int initEmpList(employeeTable empTable, char companyName[]);
int initAttendanceList(employeeTable empTable, char companyName[]);

int hash(char empID[]);
int insertEmployee(employeeTable empTable, employeeInfo emp);
int searchEmployee(employeeTable empTable, char empID[]);
int insertAttendance(employeeTable empTable, attendanceDetails att);
int insertEmployeeToFile(char companyName[], employeeInfo emp);
int insertAttendanceToFile(char companyName[], attendanceDetails att);

int assignEmployeeID(employeeTable empTable, employeeInfo *emp);
int nameValidation(char name[]);
int dateValidation(int month, int day, int year);
int emailValidation(char email[]);
int phoneValidation(char phone[]);
int payValidation(float amount);
int addEmployee(employeeTable empTable, char companyName[]);

void viewEmployeeList(employeeTable empTable);                                      // view entire list of employees
void sortEmployees(employeeTable empTable, employeeList listEmployee);              // sort employees by department
int displayEmployees(employeeTable empTable, employeeList listEmployee, char dept); // display employee given by a department
void clearEmployeeLL(employeeList listEmployee);

void viewEmployee(employeeTable empTable);

/*End of initialization function Protypes */
void editEmployee();
void createPayroll();
void confirmExit();

/***** main() function - Handles the main menu and calls the subfunction *****/
int main()
{

    /* Employee hash table declaration */
    employeeTable empTable;
    char companyName[32];

    /* Variable declarations */
    char choice; // for switch statement main menu
    int status;  // to check if operation was successful

    printf("\n===================================================");
    printf("\n|                     Welcome!                    |");
    printf("\n|            CIS 2206 - PAYROLL SYSTEM            |");
    printf("\n|             Version 1.0 - 3/21/2022             |");
    printf("\n|                                                 |");
    printf("\n| Developers: C. Stewart, J. Leano, J. Samson,    |");
    printf("\n|             J. Rosales, J. Tejada, I. Monzales, |");
    printf("\n|             J. Guzman, E. Celdran, I. Woogue,   |");
    printf("\n|             M. Valeros, W. Dayata               |");
    printf("\n===================================================");

    /*Start of the Program */
    printf("\n\nEnter Company Name: ");
    gets(companyName);
    initialize(empTable, companyName); // ask for input of company name
    /* Body */
    do
    {
        system("cls");
        printf("\n==========================================");
        printf("\n                MAIN MENU                 ");
        printf("\n==========================================");
        printf("\n| [ 1 ]      Add New Employee            |");
        printf("\n| [ 2 ]      View Employee List          |");
        printf("\n| [ 3 ]      View Specific Employee      |");
        printf("\n| [ 4 ]      Edit Employee Detais        |");
        printf("\n| [ 5 ]      Create Payroll              |");
        printf("\n| [ 6 ]      Exit                        |");
        printf("\n==========================================");
        printf("\n\nYour Choice: ");

        fflush(stdin);
        scanf("%c", &choice);

        switch (choice)
        {
        case '1':
            addEmployee(empTable, companyName);
            break;
        case '2':
            viewEmployeeList(empTable);
            break;
        case '3':
            viewEmployee(empTable);
            break;
        case '4':
            // editEmployee();
            break;
        case '5':
            // createPayroll();
            break;
        case '0':
            break;
        default:
            printf("\nInvalid Choice - Please use digits only");
            break;
        }
    } while (choice != '0');

    /* Exit/return statement, if applicable */

    getch();
    return 0;
}

/***** FUNCTION DEFINITIONS *****/

/**
 * @brief initializes the data and loads the file, sets up the internal memory
 * @param - gets pointer of employeeTable to initialize with the company name
 * @return - implicit return
 */
void initialize(employeeTable empTable, char companyName[])
{
    int i; // counter
    int empCount, attCount;
    int exit = 0;
    char newChoice;
    char fileName[40];
    FILE *fp;

    for (i = 0; i < SIZE; i++) // loop for hash table initialization
    {
        strcpy(empTable[i].employee.empID, "EMPTY"); // sets each employee ID to EMPTY
        empTable[i].history = NULL;                  // sets each head pointer to NULL
    }

    while (exit == 0)
    {
        empCount = initEmpList(empTable, companyName);
        attCount = initAttendanceList(empTable, companyName);

        if (empCount == -1)
        {
            printf("\nNo files found. Proceed to create new files?");
            printf("\n[ 1 ] Yes");
            printf("\n[ 2 ] No");
            printf("\n\nChoice: ");
            do
            {
                fflush(stdin);
                scanf("%c", &newChoice);
                switch (newChoice)
                {
                case '1':
                    strcpy(fileName, companyName);
                    strcat(fileName, EMP_FILENAME);
                    fp = fopen(fileName, "wb");
                    if (fp != NULL)
                    {
                        fclose(fp);
                        strcpy(fileName, "");
                        strcpy(fileName, companyName);
                        strcat(fileName, ATT_FILENAME);
                        fp = fopen(fileName, "wb");
                        if (fp != NULL)
                        {
                            fclose(fp);
                            printf("\nFiles successfully created.");
                            exit = 1;
                        }
                    }
                    break;
                case '2':
                    printf("\nEnter company name: ");
                    fflush(stdin);
                    gets(companyName);
                    break;
                default:
                    printf("\nInvalid choice. Please try again.");
                }
            } while (newChoice != '1' && newChoice != '2');
        }
        else
        {
            printf("\nFiles loaded. %d employee and %d attendance entries found.", empCount, attCount);
            exit = 1;
        }
    }

    printf("\nPress any key to proceed to main menu.");
    getch();
}

/**
 * @brief fetches data from the employee file and loads to the hash table
 * @param - gets pointer of employeeTable and name of company
 * @return - returns the number of records read and -1 if no file found
 */
int initEmpList(employeeTable empTable, char companyName[])
{
    FILE *fp;
    employeeInfo catcher;
    catcher.history = NULL;
    int count = 0;
    char fileName[40];

    strcpy(fileName, companyName);
    strcat(fileName, EMP_FILENAME);
    fp = fopen(fileName, "rb");

    if (fp != NULL)
    { // Read each record and insert to hash table
        while (fread(&catcher.employee, sizeof(employeeDetails), 1, fp) != 0)
        {
            count++;
            insertEmployee(empTable, catcher);
        }
        fclose(fp);
    }
    else
    { // Means that company is new and not yet created.
        count = -1;
    }
    return count;
}

/**
 * @brief fetches data from the attendance file and loads to the LL portion of the hash table
 * @param - gets pointer of employeeTable and name of company
 * @return - returns the number of records read and -1 if no file found
 */
int initAttendanceList(employeeTable empTable, char companyName[])
{
    FILE *fp;
    attendanceDetails catcher;
    int count = 0;
    char fileName[40];

    strcpy(fileName, companyName);
    strcat(fileName, ATT_FILENAME);
    fp = fopen(fileName, "rb");

    if (fp != NULL)
    { // Means that company is new and not yet created.
        while (fread(&catcher, sizeof(attendanceDetails), 1, fp) != 0)
        {
            count++;
            insertAttendance(empTable, catcher);
        }
        fclose(fp);
    }
    else
    {
        // Means that company is new and not yet created.
        count = -1;
        fp = fopen(fileName, "wb");
        fclose(fp);
    }
    return count;
}

/**
 * @brief multiplication hashing method
 * @param employee id
 * @return returns the hash value
 */
int hash(char empID[])
{
    int i;
    unsigned long sum = 1;
    for (i = 0; i < 7; i++)
    {
        sum += sum * empID[i];
    }
    return sum % SIZE;
}

/**
 * @brief inserts employee to the hash table
 * @param hash table and employee structure
 * @return returns 1 if successful and 0 if unsuccessful
 */
int insertEmployee(employeeTable empTable, employeeInfo emp)
{
    int ctr, index, retVal = 0;
    for (ctr = 0, index = hash(emp.employee.empID); ctr < SIZE; ctr++)
    {
        if (strcmp(empTable[index].employee.empID, "EMPTY") == 0 || strcmp(empTable[index].employee.empID, "DELETED") == 0)
        {
            empTable[index] = emp;
            break;
        }
        index = (index + 1) % SIZE;
    }
    return (ctr < SIZE) ? 1 : 0;
}

/**
 * @brief checks the position of a given employee in the hash table
 * @param hash table and the employee id
 * @return returns the index of the employee table where it exists and -1 if does not exist
 */
int searchEmployee(employeeTable empTable, char empID[])
{
    int ctr, index;
    for (ctr = 0, index = hash(empID); ctr < SIZE && strcmp(empTable[index].employee.empID, empID) != 0; ctr++)
    {
        if (strcmp(empTable[index].employee.empID, "EMPTY") == 0)
        {
            ctr = SIZE;
        }
        index = (index + 1) % SIZE;
    }
    return ctr < SIZE ? index : -1;
}

/**
 * @brief inserts attendance record to the hash table
 * @param hash table and attendance details structure
 * @return returns 1 if successful and 0 if unsuccessful
 */
int insertAttendance(employeeTable empTable, attendanceDetails att)
{
    int retVal = 0;
    List *ptr, temp;
    int index = searchEmployee(empTable, att.empID);
    if (index != -1)
    {
        // traverse linked list (sorted in reverse order)
        for (ptr = &empTable[index].history; *ptr != NULL && strcmp((*ptr)->attendance.payrollID, att.payrollID) > 0; ptr = &(*ptr)->link)
        {
        }
        temp = (List)malloc(sizeof(struct cell));
        if (temp != NULL)
        {
            temp->attendance = att;
            temp->link = *ptr;
            *ptr = temp;
            retVal = 1;
        }
    }
    return retVal;
}

/**
 * @brief appends new employee record to the file
 * @param company name and employee structure
 * @return returns 1 if successful and 0 if unsuccessful
 */
int insertEmployeeToFile(char companyName[], employeeInfo emp)
{
    int retVal = 0;
    FILE *fp;
    employeeDetails newEmployee;
    char fileName[40];

    strcpy(fileName, companyName);
    strcat(fileName, EMP_FILENAME);
    newEmployee = emp.employee; // datatype conversion, removing LL pointer

    fp = fopen(fileName, "ab");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        fwrite(&newEmployee, sizeof(employeeDetails), 1, fp);
        fclose(fp);
        retVal = 1;
    }
    return retVal;
}

/**
 * @brief appends new attendance record to the file
 * @param company name and employee structure
 * @return returns 1 if successful and 0 if unsuccessful
 */
int insertAttendanceToFile(char companyName[], attendanceDetails att)
{
    int retVal = 0;
    FILE *fp;
    char fileName[40];

    strcpy(fileName, companyName);
    strcat(fileName, ATT_FILENAME);

    fp = fopen(fileName, "ab");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        fwrite(&att, sizeof(attendanceDetails), 1, fp);
        fclose(fp);
        retVal = 1;
    }
    return retVal;
}

/**
 * @brief assigns a newly created employee an id
 * @param hash table and the newly created employee
 * @return returns 1 if successful and 0 if unsuccessful
 */
int assignEmployeeID(employeeTable empTable, employeeInfo *emp)
{
    int i;
    char newID[8] = {0}, year[3], month[3], empNum[4];      /* Employee ID format = year(2) month(2) increment(3) ex. 2203001 */
    sprintf(year, "%02d", emp->employee.dateEmployed.year); /* converts int to string with leading zeroes */
    sprintf(month, "%02d", emp->employee.dateEmployed.month);
    for (i = 1; i < 1000; i++)
    {
        memset(newID, 0, 8); /* sets string to empty */
        strcat(newID, year); /* catenate strings in order for id format */
        strcat(newID, month);
        sprintf(empNum, "%03d", i);
        strcat(newID, empNum);
        if (searchEmployee(empTable, newID) == -1) /* checks if id is unique */
        {
            strcpy(emp->employee.empID, newID);
            break;
        }
    }
    return (i < 1000) ? 1 : 0;
}

/**
 * @brief checks if name is valid (only alphabet characters and spaces)
 * @param name
 * @return returns 1 if valid and 0 if invalid
 */
int nameValidation(char name[])
{
    int x;
    for (x = 0; name[x] != '\0' && (isalpha(name[x]) != 0 || isspace(name[x]) != 0); x++)
    {
    }
    return (name[x] == '\0') ? 1 : 0;
}

/**
 * @brief checks if date is valid
 * @param month, day, and year
 * @return returns 1 if valid and 0 if invalid
 */
int dateValidation(int month, int day, int year)
{
    int retval = 0;
    int daysOfMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month > 0 && month < 13)
    {
        if (month == 2 && year % 4 == 0)
        { /*check if leap year*/
            if (daysOfMonth[month - 1] + 1 >= day)
            {
                retval = 1;
            }
        }
        else if (daysOfMonth[month - 1] >= day)
        {
            retval = 1;
        }
    }
    return retval;
}

/**
 * @brief checks if email is valid
 * @param email string
 * @return returns 1 if valid and 0 if invalid
 */
int emailValidation(char email[]) /* magic code, sorry daan wayne hahaha */
{
    int emailCheck = 0;
    int i;

    for (i = 0; email[i] != '\0'; i++)
    {
        switch (email[i])
        {
        case '@':
            if (!emailCheck && email[i] == '@' && i != 0)
            { /*check if '@' exists and is preceeded by characters*/
                emailCheck++;
            }
            else if (email[i] == '@')
            { /*check multiple @s*/
                emailCheck = 0;
            }
            break;
        case '.':
            if (email[i] == '.' && email[i + 1] == '\0')
            { /*check if '.' not at end*/
                emailCheck = 0;
            }
            if (emailCheck && email[i] == '.')
            { /*check if '.' exists*/
                emailCheck = (email[i - 1] != '@') ? emailCheck + 1 : 0;
            }
            break;
        }
    }

    return (emailCheck >= 2) ? 1 : 0;
}

/**
 * @brief checks if phone number is valid
 * @param phone string
 * @return returns 1 if valid and 0 if invalid
 */
int phoneValidation(char phone[])
{
    int i;
    for (i = 0; phone[i] != '\0' && isdigit(phone[i]); i++)
    {
    } /*check if correct length and is num*/
    return (i == 11) ? 1 : 0;
}

/**
 * @brief checks if salary or payment is valid
 * @param amount
 * @return returns 1 if valid and 0 if invalid
 */
int payValidation(float amount)
{
    return (amount > 0) ? 1 : 0; /*check if non-negative*/
}

/**
 * @brief adds employee to the hash table based on user input
 * @param hash table
 * @return returns 1 if successful, 0 if unsuccessful, and -1 if cancelled
 */
int addEmployee(employeeTable empTable, char companyName[])
{
    /* buffer for input validation */
    char dateString[11], emailString[32], contactString[12];
    int month, day, year;
    float basicSalary = 0, overtimePay = 0, contributions = 0;

    int validFlag[10] = {0}; /* check if all fields are correctly inputted */
    int retValue = 0;        /* return value */
    int exitFlag = 0;        /* condition if the loop will continue */
    int i;                   /* counter for loops */
    int ch;

    char choice; /* 0-9 = inputs, e = exit, c = create */

    /* initialize empty employee */
    employeeInfo newEmployee =
        {
            .employee =
                {
                    .empID = 0,
                    .name =
                        {
                            .fName = "",
                            .LName = "",
                            .MI = ' '},
                    .contact =
                        {
                            .email = "",
                            .phone = ""},
                    .dateEmployed =
                        {
                            .month = 0,
                            .day = 0,
                            .year = 0},
                    .status = ACTIVE,
                    .details =
                        {
                            .basicSalary = 0,
                            .overtimePay = 0,
                            .contributions = 0}},
            .history = NULL};

    while (!exitFlag) /* Loop until the exit flag is changed */
    {
        system("cls");
        printf("\n==========================================");
        printf("\n            CREATE NEW EMPLOYEE	        ");
        printf("\n==========================================");
        printf("\n[ 1 ] Last Name:              \t%s", newEmployee.employee.name.LName);
        printf("\n[ 2 ] First Name:             \t%s", newEmployee.employee.name.fName);
        printf("\n[ 3 ] MI:                     \t%c.", newEmployee.employee.name.MI);
        printf("\n[ 4 ] Date employed(MM/DD/YY):\t%02d/%02d/%02d", newEmployee.employee.dateEmployed.month, newEmployee.employee.dateEmployed.day, newEmployee.employee.dateEmployed.year);
        printf("\n[ 5 ] Email:                  \t%s", newEmployee.employee.contact.email);
        printf("\n[ 6 ] Contact No.:            \t%s", newEmployee.employee.contact.phone);
        printf("\n[ 7 ] Basic Salary:           \t%.02f", newEmployee.employee.details.basicSalary);
        printf("\n[ 8 ] Overtime Pay:           \t%.02f", newEmployee.employee.details.overtimePay);
        printf("\n[ 9 ] Total Contributions:    \t%.02f", newEmployee.employee.details.contributions);
        printf("\n[ 0 ] Department:             \t%c", newEmployee.employee.department);
        printf("\n[ - ] Employee Status:        \t%s", (newEmployee.employee.status) ? "Inactive" : "Active");
        printf("\n==========================================");
        printf("\n[c] create employee | [e] exit ");
        printf("\n\nChoice: ");
        fflush(stdin);
        scanf("%c", &choice);
        while ((ch = getchar()) != '\n' && ch != EOF)
            ; // To eat up all the characters left behind by scanf call including '\n'.

        switch (choice)
        {
        case '1':
            printf("\nLast Name: ");
            fflush(stdin);
            gets(newEmployee.employee.name.LName);
            if (nameValidation(newEmployee.employee.name.LName))
            {
                validFlag[0] = 1;
            }
            else
            {
                strcpy(newEmployee.employee.name.LName, "");
                printf("Invalid input\n");
                printf("\nPress any key to continue");
                getch();
            }
            break;

        case '2':
            printf("\nFirst Name: ");
            fflush(stdin);
            gets(newEmployee.employee.name.fName);
            if (nameValidation(newEmployee.employee.name.fName))
            {
                validFlag[1] = 1;
            }
            else
            {
                strcpy(newEmployee.employee.name.fName, "");
                printf("Invalid input\n");
                printf("\nPress any key to continue");
                getch();
            }
            break;

        case '3':
            printf("\nMiddle Initial: ");
            fflush(stdin);
            scanf("%c", &newEmployee.employee.name.MI);
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            if (isalpha(newEmployee.employee.name.MI) || isspace(newEmployee.employee.name.MI))
            {
                newEmployee.employee.name.MI = toupper(newEmployee.employee.name.MI);
                validFlag[2] = 1;
            }
            else
            {
                newEmployee.employee.name.MI = ' ';
                printf("Invalid input\n");
                printf("\nPress any key to continue");
                getch();
            }

            break;

        case '4':
            printf("\nDate employed(MM/DD/YY): ");
            fflush(stdin);
            gets(dateString);

            char *token = strtok(dateString, "/"); /* separates date by '/' */
            month = atoi(token);                   /* converts string to int */
            token = strtok(NULL, "/");
            day = atoi(token);
            token = strtok(NULL, "/");
            year = atoi(token);

            if (dateValidation(month, day, year))
            {
                newEmployee.employee.dateEmployed.month = month;
                token = strtok(NULL, "/");
                newEmployee.employee.dateEmployed.day = day;
                token = strtok(NULL, "/");
                newEmployee.employee.dateEmployed.year = year;
                validFlag[3] = 1;
            }
            else
            {
                printf("Invalid input\n");
                printf("\nPress any key to continue");
                getch();
            }

            break;

        case '5':
            printf("\nEmail: ");
            fflush(stdin);
            gets(emailString);
            if (emailValidation(emailString))
            {
                strcpy(newEmployee.employee.contact.email, emailString);
                validFlag[4] = 1;
            }
            else
            {
                printf("Invalid Input");
                printf("\nPress any key to continue");
                getch();
            }

            break;

        case '6':
            printf("\nContact No.: ");
            fflush(stdin);
            gets(contactString);
            if (phoneValidation(contactString))
            {
                strcpy(newEmployee.employee.contact.phone, contactString);
                validFlag[5] = 1;
            }
            else
            {
                printf("Input invalid");
                printf("\nPress any key to continue");
                getch();
            }

            break;

        case '7':
            printf("\nBasic Monthly Salary: ");
            fflush(stdin);
            scanf("%f", &basicSalary);
            if (payValidation(basicSalary))
            {
                newEmployee.employee.details.basicSalary = basicSalary;
                validFlag[6] = 1;
            }
            else
            {
                printf("Input invalid");
                printf("\nPress any key to continue");
                getch();
            }
            break;

        case '8':
            printf("\nOvertime Hourly Pay: ");
            fflush(stdin);
            scanf("%f", &overtimePay);
            if (payValidation(overtimePay))
            {
                newEmployee.employee.details.overtimePay = overtimePay;
                validFlag[7] = 1;
            }
            else
            {
                printf("Input invalid - please enter a positive amount");
                printf("\nPress any key to continue");
                getch();
            }
            break;

        case '9':
            printf("\nTotal Monthly Contributions: ");
            fflush(stdin);
            scanf("%f", &contributions);
            if (payValidation(contributions))
            {
                newEmployee.employee.details.contributions = contributions;
                validFlag[8] = 1;
            }
            else
            {
                printf("Input invalid - please enter a positive amount");
                printf("\nPress any key to continue");
                getch();
            }
            break;

        case '0':
            printf("\nDepartment [A-Z]: ");
            fflush(stdin);
            scanf("%c", &newEmployee.employee.department);
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            if (isalpha(newEmployee.employee.department))
            {
                newEmployee.employee.department = toupper(newEmployee.employee.department);
                validFlag[9] = 1;
            }
            else
            {
                newEmployee.employee.name.MI = ' ';
                printf("Invalid input. Please enter a letter\n");
                printf("\nPress any key to continue");
                getch();
            }

            break;

        case '-':
            printf("\n[ 0 ] Active");
            printf("\n[ 1 ] Inactive");
            printf("\nStatus: ");
            fflush(stdin);
            scanf("%c", &choice);
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            if (choice == '0' || choice == '1')
            { /*check if input is valid*/
                newEmployee.employee.status = choice - '0';
            }
            else
            {
                printf("Invalid input");
                printf("\nPress any key to continue");
                getch();
            }
            break;

        case 'c':
            for (i = 0; i < 10 && validFlag[i] != 0; i++)
            {
            }
            if (i == 10)
            {
                if (assignEmployeeID(empTable, &newEmployee))
                {
                    if (insertEmployee(empTable, newEmployee) && insertEmployeeToFile(companyName, newEmployee))
                    {
                        printf("\nSuccessfully created employee!");
                        printf("\nYour employee ID is %s", newEmployee.employee.empID);
                        printf("\n\nPress any key to continue");
                        getch();
                        retValue = 1;
                    }
                    else
                    {
                        printf("Employee Table is full\n");
                        printf("\nPress any key to continue");
                        getch();
                        retValue = 0;
                    }
                }
                else
                {
                    printf("Error: Failed to assign Employee ID\n");
                    printf("\nPress any key to continue");
                    getch();
                    retValue = 0;
                }
                exitFlag = 1; /* prompts to exit the loop */
            }
            else
            {
                printf("Missing input at [%d]\n", i + 1);
                printf("\nPress any key to continue");
                getch();
            }
            break;

        case 'e':
            retValue = -1;
            exitFlag = 1; /* prompts to exit the loop with the default return value of 0*/
            break;

        default:
            printf("Not a valid choice!\n");
            printf("\nPress any key to continue");
            getch();
        }
    }

    return retValue;
}

/**
 * @brief - view entire list of employees or by department
 * @param describe the parameters
 * @return state what the function returns
 */
void viewEmployeeList(employeeTable empTable)
{
    system("cls");
    printf("\n==========================================");
    printf("\n            VIEW EMPLOYEE LIST	        ");
    printf("\n==========================================");

    /* Employee linked list declaration and initialization */
    employeeList listEmployee;
    int x;
    char choice, ch, flag = 1;

    for (x = 0; x < 26; x++)
    {
        listEmployee[x] = NULL;
    }

    sortEmployees(empTable, listEmployee);

    while (flag)
    {
        printf("\n[ 1 ] Display all employees");
        printf("\n[ 2 ] Display employees of a department");
        printf("\n[ 3 ] Main menu");
        printf("\n\nChoice: ");
        fflush(stdin);
        scanf("%c", &choice);
        switch (choice)
        {
        case '1':
            // display all employees
            flag = 0;
            for (ch = 'A'; ch <= 'Z'; ch++)
            {
                displayEmployees(empTable, listEmployee, ch);
            }
            break;
        case '2':
            // display all employees of a department
            do
            {
                printf("\nPlease enter a department[A-Z] (0 to exit): ");
                fflush(stdin);
                scanf("%c", &ch);
            } while (!(isalpha(ch)!=0 || ch=='0'));
            if (ch !='0' && displayEmployees(empTable, listEmployee, ch) == 0)
            {
                printf("\nError: No employees found in the selected department [%c]. ",toupper(ch));
            }
            else
            {
                flag = 0;
            }
            break;
        case '3':
            clearEmployeeLL(listEmployee);
            return;  
            break;
        default:
            printf("Invalid input. Please enter 1 or 2");
            break;
        }
    }
    printf("\n\nPress any key to exit...");
    getch();
    clearEmployeeLL(listEmployee);
}

/**
 * @brief - display all employees in a given department
 * @param - list of employees, department name
 * @return number of entries displayed, 0 if none displayed
 */
int displayEmployees(employeeTable empTable, employeeList listEmployee, char dept)
{
    dept = toupper(dept);
    int index = dept - 'A';
    int count = 0;
    List2 trav, ptr;

    if (listEmployee[index] != NULL)
    {
        printf("\nDEPARTMENT %c", dept);
        printf("\n----------------------------------------------------------------------");

        for (trav = listEmployee[index]; trav != NULL; trav = trav->link)
        {
            employeeDetails temp = empTable[trav->index].employee;
            printf("\n  %-8s %-15s, %-26s %c.   %s", temp.empID, temp.name.LName, temp.name.fName, temp.name.MI, (temp.status) ? "Inactive" : "Active");
            count++;
        }
    }
    return count;
}

/**
 * @brief - sort employees by department
 * @param - list of employees
 * @return state what the function returns
 */
void sortEmployees(employeeTable empTable, employeeList listEmployee)
{
    int x;
    List2 ptr, *trav, new;
    employeeDetails temp;
    for (x = 0; x < SIZE; x++)
    {
        temp = empTable[x].employee;
        if (strcmp(temp.empID, "EMPTY") != 0 && strcmp(temp.empID, "DELETED") != 0)
        {
            int index = temp.department - 'A';

            /*insert the index of the employee to their department - sorted by empID*/
            for (trav = &listEmployee[index]; *trav != NULL && (strcmp(empTable[(*trav)->index].employee.name.LName, temp.name.LName) <= 0); trav = &(*trav)->link)
            {
            }
            new = (List2)calloc(1, sizeof(struct node));
            new->index = x;
            new->link = *trav;
            *trav = new;
        }
    }
}

void clearEmployeeLL(employeeList listEmployee)
{
    int index;
    List2 ptr, *trav;
    for (index = 0; index < 26; index++)
    {
        for (trav = &listEmployee[index]; *trav != NULL;)
        {
            ptr = *trav;
            *trav = ptr->link;
            free(ptr);
        }
    }
}

void viewEmployee(employeeTable empTable){
    int index, period;
    char givenID[8], flag = 1, choice;
    List trav;

    system("cls");
    printf("\n==========================================");
    printf("\n           VIEW SPECIFIC EMPLOYEE	        ");
    printf("\n==========================================");
    printf("\n\nEnter Employee ID: ");
    fflush(stdin);
    scanf("%s", givenID);

    index = searchEmployee(empTable, givenID);

    if(index != -1){
        while (flag){
            system("cls");
            printf("\n==========================================");
            printf("\n       VIEW SPECIFIC EMPLOYEE DETAILS     ");
            printf("\n==========================================");
            printf("\n[ 1 ] Display complete details");
            printf("\n[ 2 ] Display periods");
            printf("\n[ 3 ] Main menu");
            printf("\n\nChoice: ");
            fflush(stdin);
            scanf("%c", &choice);
            switch (choice)
            {
            case '1':
                flag = 0;
                system("cls");
                printf("\n===================================================");
                printf("\n             COMPLETE EMPLOYEE DETAILS     	     ");
                printf("\n===================================================");
                printf("\nEmployee ID:            \t%s", empTable[index].employee.empID);
                printf("\nLast Name:              \t%s", empTable[index].employee.name.LName);
                printf("\nFirst Name:             \t%s", empTable[index].employee.name.fName);
                printf("\nMI:                     \t%c.", empTable[index].employee.name.MI);
                printf("\nDate employed(MM/DD/YY):\t%02d/%02d/%02d", empTable[index].employee.dateEmployed.month, empTable[index].employee.dateEmployed.day, empTable[index].employee.dateEmployed.year);
                printf("\nEmail:                  \t%s", empTable[index].employee.contact.email);
                printf("\nContact No.:            \t%s", empTable[index].employee.contact.phone);
                printf("\nBasic Salary:           \t%.02f", empTable[index].employee.details.basicSalary);
                printf("\nOvertime Pay:           \t%.02f", empTable[index].employee.details.overtimePay);
                printf("\nTotal Contributions:    \t%.02f", empTable[index].employee.details.contributions);
                printf("\nDepartment:             \t%c", empTable[index].employee.department);
                printf("\nEmployee Status:        \t%s", (empTable[index].employee.status) ? "Inactive" : "Active");
                printf("\n===================================================");
                break;
            case '2':
                flag = 0;
                system("cls");
                printf("\n==========================================");
                printf("\n          EMPLOYEE PERIOD DETAILS         ");
                printf("\n==========================================");
                printf("\n\nEnter period to display: ");
                fflush(stdin);
                scanf("%d", &period);

                system("cls");
                printf("\nATTENDANCE DETAILS OF %s, %s %c.", empTable[index].employee.name.LName, empTable[index].employee.name.fName, empTable[index].employee.name.MI);
                printf("\n----------------------------------------------------------------------");
                printf("\nPeriod %d", period);

                printf("\n\nPayroll ID");

                for (trav = empTable[index].history; trav != NULL; trav = trav->link)
                {
                    attendanceDetails temp = trav->attendance;
                    printf("\n  %-8s", temp.payrollID);
                }
                break;
            case '3':
                return;  
                break;
            default:
                printf("Invalid input. Please enter 1 or 2");
                break;
            }
        }
    }
    else{
        printf("\nError: Emplyoee does not exist. ");
    }
    printf("\n\nPress any key to exit...");
    getch();
}
