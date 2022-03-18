/**************************************************************
 *
 * CIS 2206 Practice Activity #1 - Payroll System using C
 * Created by: Team 1
 * Members: 
 * Date: March 14-21, 2022
 *  
 * ************************************************************/


/***** HEADER FILES TO INCLUDE *****/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/***** CONSTANTS *****/
#define SIZE 512                    // hash table size
#define EMP_FILENAME 'EmployeeFile.bin'
#define ATT_FILENAME 'AttendanceFile.bin'
#define PAY_FILENAME 'PayrollIDList.bin'
/* Note: Actual file names will be '<CompanyName>_EmployeeFile.bin'. 
 * Company name will be inputted at main menu upon program and data initialization.
 */

/***** DATA STRUCTURE DEFINITION *****/
typedef struct{
    char LName[16];         // last name
    char fName[24];         // first name
    char MI;                // middle initial
}nameDetails;

typedef enum{
    ACTIVE,                 // only active employees will be prompted attendance entry
    INACTIVE                // in the specified group for each new payroll entry
}empStatus;

typedef struct{
    unsigned int year:7;    // YY
    unsigned int month:4;   // MM
    unsigned int day:5;     // DD
}dateDetails;                      // CONSTRAINT: No negative values

typedef struct{
    char email[32];         // must have '@' and '.' for valid email
    char phone[11];         // 11-digit mobile number
}contactDetails;

typedef struct{
    float basicSalary;      // basic monthly salary, in PHP
    float overtimePay;      // overtime monthly pap, in PHP
    float contributions;    // total government contributions, in PHP
}paymentDetails;            // CONSTRAINT: No negative values

typedef struct{
    char empID[8];          // employee ID (CONSTRAINT: Must match an existing employee ID)
    char payrollID[6];      // payroll ID  (CONSTRAINT: See format below)
    int daysAbsent;         // number of days absent for the period
    int hoursOvertime;      // overtime duration for the period, in hours
    int minsUndertime;      // undertime duration for the period, in minutes
}attendanceDetails;         // the structure to be written in the 'EmployeeFile.bin'

/* Payroll ID is YYMMPG - Year, Month, Period, Group #
 * Example:      22011A - Period 1, January 2022, Group A
 */

/* Linked list for attendance details */
typedef struct cell{
    attendanceDetails attendance;
    struct cell* link;
}cellType, *attendanceHistory;

/* Employee structure, with attendance details LL pointer */
typedef struct{
    char empID[8];          // employee ID (7 characters)
                            // for has table: Dummy values - "EMPTY" and "DELETED"
    nameDetails name;       
    contactDetails contact;
    dateDetails dateEmployed;
    empStatus status;
    paymentDetails details;
}employeeDetails;           // the structure to be written in the 'EmployeeFile.bin'

typedef struct{
    employeeDetails employee;   //employee details
    attendanceHistory history;  //head pointer of the attendance linked list
}employeeInfo;

/* Hash table of employees - CLOSED HASHING WITH MAX BUCKETS, WITH LINEAR PROBING
 *  I chose closed hashing ra para sayun ma clear ang memory by avoiding dynamic cells,
    since when we terminate a program we have to free all dynamic memory
 */
typedef employeeInfo employeeTable[SIZE];  // constant hash table

/***** FUNCTION PROTOTYPES *****/
void initialize();          // initialize by loading existing file. If none, new file will be created
void terminate();           // properly terminate the file by freeing all dynamic memory (attendance LL)
int saveData();             // returns 1 if successful save and 0 if not
int loadData();
void loadMenu();            // displays the main menu for the user.

// addEmployee();
// editEmployee(char empID[], int mode);
// createPayroll();
// 
//

int hash(char empID);
// date manipulation functions


/***** main() function - Handles the main menu and calls the subfunction *****/
int main(){
    
    /* Employee hash table declaration */
    employeeTable empTable;
    
    printf("\n==========================================");
    printf("\n        CIS 2206 - PAYROLL SYSTEM         ");
    printf("\n==========================================");
    loadMenu();
    initialize(); // ask for input of company name

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
 * @param -
 * @return -
 */
void initialize(){
    /* Variable declarations */
    /* Variable initializations */
    /* Body */
    /* Exit/return statement, if applicable */
}

/**
 * @brief displays the menu of the entire program
 * @param - no arguments
 * @return - no return value
 */
void loadMenu(){
    int choice; // for switch statement main menu

    do
    {
        printf("\n==========================================");
        printf("\n                Main Menu                 ");
        printf("\n==========================================");
        printf("\n1 || Create Employee");
        printf("\n2 || View Employee List");
        printf("\n3 || View Specific Employee");
        printf("\n4 || Edit Employee Details");
        printf("\n5 || Create Payroll");
        printf("\n6 || Reload");
        printf("\n0 || Exit");

        scanf("%d", &choice);

        switch(choice) {
            case 1: 
                    break;
            case 2: 
                    break;
            case 3: 
                    break;
            case 4: 
                    break;
            case 5: 
                    break;
            case 6: 
                    break;
            case 0: printf("\nProgram Closing...");
                    exit(0);
                    break;
            default:printf("\nInvalid Choice - Please use digits only");
                    break;

        }
    } while (choice != 0);
}

/**
 * @brief give a brief description of the function
 * @param describe the parameters
 * @return state what the function returns
 */
void func1(int args1,int args2){
    /* Variable declarations */
    /* Variable initializations */
    /* Body */
    /* Exit/return statement, if applicable */
}