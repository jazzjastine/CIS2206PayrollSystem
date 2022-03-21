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

/***** FUNCTION PROTOTYPES *****/
int hash(char empID[]);
void initialize(employeeTable empTable, char companyName[]); 
int initEmpList(employeeTable empTable, char companyName[]); 
int initAttendanceList(employeeTable empTable, char companyName[]);
/*End of initialization function Protypes */

void terminate(); // properly terminate the file by freeing all dynamic memory (attendance LL)
int saveData();   // returns 1 if successful save and 0 if not
int loadData();

int addEmployee(); // change accordingly
void viewEmployeeList();
void viewEmployee();
void editEmployee();
void createPayroll();
void confirmExit();

void loadMenu(); // displays the main menu for the user.

// editEmployee(char empID[], int mode);
// createPayroll();
//
//

// date manipulation functions

/***** main() function - Handles the main menu and calls the subfunction *****/
int main()
{

    /* Employee hash table declaration */
    employeeTable empTable;
    char companyName[32];

    /* Variable declarations */
    int choice; // for switch statement main menu

    printf("\n==========================================");
    printf("\n        CIS 2206 - PAYROLL SYSTEM         ");
    printf("\n         Version 1.0 - 3/21/2022          ");
    printf("\n==========================================");

    /*Start of the Program */
    printf("\nEnter Company Name: ");
    scanf("%s", &companyName);
    initialize(empTable, companyName); // ask for input of company name
    /* Body */
    do
    {
        printf("\e[1;1H\e[2J"); // Similar to clrscr() function. Can't use conio.h, only for _WIN32
        printf("\n==========================================");
        printf("\n                Main Menu                 ");
        printf("\n==========================================");
        printf("\n||  1  ||   Create Employee             ||");
        printf("\n||  2  ||   View Employee List          ||");
        printf("\n||  3  ||   View Specific Employee      ||");
        printf("\n||  4  ||   Edit Employee Detais        ||");
        printf("\n||  5  ||   Create Payroll              ||");
        printf("\n||  0  ||   Exit                        ||");
        printf("\n\nYour Choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addEmployee(empTable, companyName);
            break;
        case 2:
            viewEmployeeList();
            break;
        case 3:
            viewEmployee();
            break;
        case 4:
            editEmployee();
            break;
        case 5:
            createPayroll();
            break;
        case 0:
            confirmExit();
            break;
        default:
            printf("\nInvalid Choice - Please use digits only");
            break;
        }
    } while (choice != 0);

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
    /*Variable Declaration*/
    int i; // counter
    int count;

    for (i = 0; i < SIZE; i++) // loop for hash table initialization
    {
        strcpy(empTable[i].employee.empID, "EMPTY"); // sets each employee ID to EMPTY
        empTable[i].history = NULL;                  // sets each head pointer to NULL
    }

    count = initEmpList(empTable, companyName);
    initAttendanceList(empTable, companyName);
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
    char fileName;

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
        printf("No file found. A new employee file will be created");
        fp = fopen(fileName, "w");
        fclose(fp);
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
    char fileName;

    strcpy(fileName, companyName);
    strcat(fileName, ATT_FILENAME);
    fp = fopen(fileName, "rb");

    if (fp != NULL)
    { // Means that company is new and not yet created.
        while (fread(&catcher, sizeof(attendanceDetails), 1, fp) != 0)
        {
            count++;
            insertAttendance(empTable,catcher);
        }
        fclose(fp);
    }
    else
    {
        // Means that company is new and not yet created.
        count = -1;
        printf("No file found. A new attendance file will be created");
        fp = fopen(fileName, "w");
        fclose(fp);
    }
    return count;
}

/**
 * @brief multiplication hashing method
 * @param employee id
 * @return returns the hash value
 */
int hash(char empID[8])
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
 * @brief inserts attendance record to the hash table
 * @param hash table and attendance details structure
 * @return returns 1 if successful and 0 if unsuccessful
 */
int insertAttendance(employeeTable empTable, attendanceDetails att)
{
    int retVal = 0;
    List *ptr, temp;
    int index = searchEmployee(empTable,att.empID);
    if(index != -1){
        // traverse linked list (sorted in reverse order)
        for(ptr = &empTable[index].history; *ptr != NULL && strcmp((*ptr)->attendance.payrollID,att.payrollID > 0); ptr=&(*ptr)->link) {}
        temp = (List)malloc(sizeof(struct cell));
        if(temp != NULL){
            temp->attendance = att;
            temp->link = *ptr;
            *ptr = temp;
        }
    }
    return retVal;
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
 * @brief appends new employee record to the file
 * @param company name and employee structure
 * @return returns 1 if successful and 0 if unsuccessful
 */
int insertEmployeeToFile(char companyName[], employeeInfo emp)
{
    int retVal = 0;
    FILE *fp;
    employeeDetails newEmployee;
    char fileName;

    strcpy(fileName, companyName);
    strcat(fileName, EMP_FILENAME);
    newEmployee = emp.employee;

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
    char fileName;

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
 * @return returns 1 if successful and 0 if unsuccessful
 */
int addEmployee(employeeTable empTable, char companyName[]) /* returns 0 if unsuccessful and 1 if successful */
{
    /* buffer for input validation */
    char dateString[11], emailString[32], contactString[12];
    int month, day, year;
    float basicSalary = 0, overtimePay = 0, contributions = 0;

    int validFlag[9] = {0}; /* check if all fields are correctly inputted */
    int retValue = 0;       /* return value */
    int exitFlag = 0;       /* condition if the loop will continue */
    int i;                  /* counter for loops */
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
        // system("cls");
        printf("\n==========================================");
        printf("\n        CIS 2206 - PAYROLL SYSTEM         ");
        printf("\n==========================================");
        printf("\n[1] Last Name:            \t%s", newEmployee.employee.name.LName);
        printf("\n[2] First Name:           \t%s", newEmployee.employee.name.fName);
        printf("\n[3] MI:                   \t%c.", newEmployee.employee.name.MI);
        printf("\n[4] Date(MM/DD/YY):       \t%02d/%02d/%02d", newEmployee.employee.dateEmployed.month, newEmployee.employee.dateEmployed.day, newEmployee.employee.dateEmployed.year);
        printf("\n[5] Email:                \t%s", newEmployee.employee.contact.email);
        printf("\n[6] Contact No.:          \t%s", newEmployee.employee.contact.phone);
        printf("\n[7] Basic Salary:         \t%.02f", newEmployee.employee.details.basicSalary);
        printf("\n[8] Overtime Pay:         \t%.02f", newEmployee.employee.details.overtimePay);
        printf("\n[9] Total Contributions:  \t%.02f", newEmployee.employee.details.contributions);
        printf("\n[0] Employee Status:      \t%s", (newEmployee.employee.status) ? "Inactive" : "Active");
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
            getchar();
            gets(newEmployee.employee.name.LName);
            // nameUpper(newEmployee.employee.name.LName);
            validFlag[0] = 1;
            break;

        case '2':
            printf("\nFirst Name: ");
            getchar();
            gets(newEmployee.employee.name.fName);
            // nameUpper(newEmployee.employee.name.fName);
            validFlag[1] = 1;
            break;

        case '3':
            printf("\nMiddle Initial: ");
            scanf(" %c", &newEmployee.employee.name.MI);
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            newEmployee.employee.name.MI = toupper(newEmployee.employee.name.MI);
            validFlag[2] = 1;
            break;

        case '4':
            printf("\nDate employed(MM/DD/YY): ");
            getchar();
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
            }

            break;

        case '5':
            printf("\nEmail: ");
            getchar();
            gets(emailString);
            if (emailValidation(emailString))
            {
                strcpy(newEmployee.employee.contact.email, emailString);
                validFlag[4] = 1;
            }
            else
            {
                printf("Invalid Input\n");
            }

            break;

        case '6':
            printf("\nContact No.: ");
            getchar();
            gets(contactString);
            if (phoneValidation(contactString))
            {
                strcpy(newEmployee.employee.contact.phone, contactString);
                validFlag[5] = 1;
            }
            else
            {
                printf("Input invalid\n");
            }

            break;

        case '7':
            printf("\nBasic Salary: ");
            scanf(" %f", &basicSalary);
            if (payValidation(basicSalary))
            {
                newEmployee.employee.details.basicSalary = basicSalary;
                validFlag[6] = 1;
            }
            else
            {
                printf("Input invalid");
            }
            break;

        case '8':
            printf("\nOvertime Pay: ");
            scanf(" %f", &overtimePay);
            if (payValidation(overtimePay))
            {
                newEmployee.employee.details.overtimePay = overtimePay;
                validFlag[7] = 1;
            }
            else
            {
                printf("Input invalid");
            }
            break;

        case '9':
            printf("\nTotal Contributions: ");
            scanf(" %f", &contributions);
            if (payValidation(contributions))
            {
                newEmployee.employee.details.contributions = contributions;
                validFlag[8] = 1;
            }
            else
            {
                printf("Input invalid");
            }
            break;

        case '0':
            printf("\n[0] Active");
            printf("\n[1] Inactive");
            printf("\nStatus: ");
            scanf(" %c", &choice);
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            if (choice == '0' || choice == '1')
            { /*check if input is valid*/
                newEmployee.employee.status = choice - '0';
            }
            else
            {
                printf("Invalid input");
            }
            break;

        case 'c':
            for (i = 0; validFlag[i] != 0 && i < 6; i++)
            {
            }
            if (i == 6)
            {
                if (assignEmployeeID(empTable, &newEmployee))
                {
                    if (insertEmployee(empTable, newEmployee) && insertEmployeeToFile(companyName, newEmployee))
                    {
                        printf("Successfully created employee");
                    }
                    else
                    {
                        printf("Employee Table is full\n");
                    }
                }
                else
                {
                    printf("Failed to assign Employee ID\n");
                }
                retValue = 1;
                exitFlag = 1; /* prompts to exit the loop */
            }
            else
            {
                printf("Missing input at [%d]\n", i + 1);
            }
            break;

        case 'e':
            exitFlag = 1; /* prompts to exit the loop with the default return value of 0*/
            break;

        default:
            printf("Not a valid choice!\n");
        }

        printf("\nPress any key to continue");
        getch();
    }

    return retValue;
}
