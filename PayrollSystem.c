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
#include <conio.h>

/***** CONSTANTS *****/
#define SIZE 512 // hash table size
#define EMP_FILENAME "_EmployeeFile.bin"
#define ATT_FILENAME "_AttendanceFile.bin"
#define PAY_FILENAME "_PayrollIDList.bin"
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
    char phone[11]; // 11-digit mobile number
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
} cellType, *attendanceHistory;

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
void initialize(employeeTable *empTable); // initialize by loading existing file. If none, new file will be created
int initEmpList(char companyName[]);
int initAttendanceList(char companyName[]);
/*End of initialization function Protypes */

void terminate(); // properly terminate the file by freeing all dynamic memory (attendance LL)
int saveData();   // returns 1 if successful save and 0 if not
int loadData();

void createEmployee(); // change accordingly
void viewEmployeeList();
void viewEmployee();
void editEmployee();
void createPayroll();
void confirmExit();

void loadMenu(); // displays the main menu for the user.

// addEmployee();
// editEmployee(char empID[], int mode);
// createPayroll();
//
//

int hash(char empID);
// date manipulation functions

/***** main() function - Handles the main menu and calls the subfunction *****/
int main()
{

    /* Employee hash table declaration */
    employeeTable empTable;

    printf("\n==========================================");
    printf("\n        CIS 2206 - PAYROLL SYSTEM         ");
    printf("\n==========================================");

    initialize(&empTable); // ask for input of company name
    loadMenu();

    /* MAIN MENU */

    /*

        Create employee
        View list of employees
            (Display ID and names)
        View specific employee info
            Enter employee ID
                1. (Display complete details
                2. (Display Periods)
                    Enter period to display
                        Retrieve desired attendance info -> Compute on the spot and display salary breakdown

        Edit employee info
            Enter employee ID
            Edit Personal Details
            Edit Attendance Info
                (Display Periods) Enter period to edit

        Create Payroll
            Enter Payroll Period and Group
                Confirm period entered or cancel
                For each employee -> Enter Attendance for Individual Employees -> insertFirst() to LL of the employee
        Reload
        Exit
            Confirm exit
    */

    getch();
    return 0;
}

/***** FUNCTION DEFINITIONS *****/

/**
 * @brief initializes the data and loads the file, sets up the internal memory
 * @param - gets pointer of employeeTable to initialize
 * @return - implicit return
 */
void initialize(employeeTable *empTable)
{
    /*Variable Declaration*/
    char companyName[32];
    int catcher;

    /*Clear the screen of Welcome" */
    printf("\nPress any key to start ");
    getch();
    printf("\e[1;1H\e[2J"); // Similar to clrscr() function. Can't use conio.h, only for _WIN32

    /*Start of the Program */
    printf("\nEnter Company Name: ");
    scanf("%s", &companyName);

    initEmpList(companyName);
    initAttendanceList(companyName);


    /* Variable declarations */

    /* Variable initializations */

    /* Body */

    /* Exit/return statement, if applicable */

    int i;
    for (i = 0; i < SIZE; i++) // loop for hash table initialization
    {
        strcpy((*empTable)[i].employee.empID, "EMPTY"); // sets each employee ID to EMPTY
        empTable[i]->history = NULL;                    // sets each head pointer to NULL
        printf("%s", (*empTable)[i].employee.empID);    // test
    }
}

int initEmpList(char companyName[])
{
    FILE *fp;
    employeeTable employeeCount;
    int count = 0;

    strcat(companyName, EMP_FILENAME);
    fp = fopen(companyName, "ab+");

    if (fp != NULL)
    { // Means that company is new and not yet created.
        while (fread(&employeeCount, sizeof(employeeInfo), 1, fp) != 0)
        {
            count++;
        }
        fclose(fp);
    }
    else
    {
        printf("Error unable to open file");
    }
    return count;
}
int initAttendanceList(char companyName[]){
    FILE *fp;
    employeeTable employeeCount;
    int count = 0;

    strcat(companyName, EMP_FILENAME);
    fp = fopen(companyName, "ab+");

    if (fp != NULL)
    { // Means that company is new and not yet created.
        while (fread(&employeeCount, sizeof(employeeInfo), 1, fp) != 0)
        {
            count++;
        }
        fclose(fp);
    }
    else
    {
        printf("Error unable to open file");
    }
    return count;
}


/**
 * @brief displays the menu of the entire program
 * @param - no arguments
 * @return - no return value
 */
void loadMenu()
{
    /* Variable declarations */
    int choice; // for switch statement main menu

    /* Body */
    do
    {
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
            createEmployee();
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
}

/**
 * @brief give a brief description of the function
 * @param describe the parameters
 * @return state what the function returns
 */
void func1(int args1, int args2)
{
    /* Variable declarations */
    /* Variable initializations */
    /* Body */
    /* Exit/return statement, if applicable */
}

/* change accordingly */
void createEmployee()
{
}

void viewEmployeeList()
{
}

void viewEmployee()
{
}

void editEmployee()
{
}

void createPayroll()
{
}

void confirmExit()
{
}
