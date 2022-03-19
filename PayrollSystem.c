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
#include<ctype.h>
#include<conio.h>

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

int addEmployee();
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
    initialize(); // ask for input of company name
    addEmployee();
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

int dateValidation(char date[])
{
    
}

int emailValidation(char email[])
{
    int emailCheck = 0;
    int i;

    for(i = 0; email[i]!='\0'; i++){
        if(!emailCheck && email[i]=='@'&& i!=0) {
            emailCheck++;
        }
        else if(email[i]=='@'){
            emailCheck=0;
        }
        if(emailCheck && email[i]=='.' && email[i-1]!='@') {
            emailCheck=(email[i-1]!='@')?0:emailCheck+1;
        }
    }

    return (emailCheck >= 2)? 1 : 0;
}

int phoneValidation(char phone[])
{
    int i;
    for(i = 0; phone[i]!='\0' && isdigit(phone[i]); i++){}
    return (i == 11)? 1 : 0;
}

int addEmployee() // returns 0 if unsuccessful and 1 if successful
{
    
    char choice; // e = exit, c = create
    char miString[5];
    char dateString[11];
    char emailString[32];
    char contactString[20];
    int validFlag[6];
    int retValue = 0;
    int exitFlag = 0;
    int i;

    employeeInfo newEmployee = 
    {
        .employee =
        {
            .name =
            {
                .fName = "",
                .LName = "",
                .MI = ' '
            },
            .contact = 
            {
                .email = "",
                .phone = ""
            },
            .dateEmployed = 
            {
                .month = 0,
                .day = 0,
                .year = 0
            },
            .status = ACTIVE,
            .details = 
            {
                .basicSalary = 0,
                .overtimePay = 0,
                .contributions = 0
            }
        },
        .history = NULL
    };

    while(!exitFlag)
    {
        system("cls");
        printf("\n==========================================");
        printf("\n        CIS 2206 - PAYROLL SYSTEM         ");
        printf("\n==========================================");
        printf("\n[1] Last Name: %s", newEmployee.employee.name.LName);
        printf("\n[2] First Name: %s", newEmployee.employee.name.fName);
        printf("\n[3] MI: %c", newEmployee.employee.name.MI);
        printf("\n[4] Date(MM/DD/YY): %02d/%02d/%02d", newEmployee.employee.dateEmployed.month, newEmployee.employee.dateEmployed.day, newEmployee.employee.dateEmployed.year);
        printf("\n[5] Email: %s", newEmployee.employee.contact.email);
        printf("\n[6] Contact No.: %s", newEmployee.employee.contact.phone);
        printf("\ne = to exit, c = to create employee ");
        printf("\n==========================================");
        
        printf("\nChoice: ");
        scanf(" %c",&choice);
        system("cls");
        switch(choice) 
        {
            case '1':
                printf("\nLast Name: ");
                scanf(" %s", newEmployee.employee.name.LName);
                validFlag[0] = 1;

                break;
                
            case '2':
                printf("\nFirst Name: ");
                scanf(" %s", newEmployee.employee.name.fName);
                validFlag[1] = 1;

                break;
                
            case '3':
                printf("\nMiddle Initial: ");
                scanf(" %c", miString);
                if(isalpha(miString[0])){
                    validFlag[2] = 1;
                    newEmployee.employee.name.MI=miString[0];
                }else{
                    printf("Invalid input");
                }

                break;

            case '4':
                printf("\nDateDate(MM/DD/YYYY): ");
                scanf(" %s", dateString);
                char * token = strtok(dateString, "/");
                newEmployee.employee.dateEmployed.month = atoi(token);
                token = strtok(NULL, "/");
                newEmployee.employee.dateEmployed.day = atoi(token);
                token = strtok(NULL, "/");
                newEmployee.employee.dateEmployed.year = atoi(token);
                validFlag[3] = 1;

                break;

            case '5':
                printf("\nEmail: ");
                scanf(" %s", emailString);              
                int valid = emailValidation(emailString);
                if(valid) {
                    strcpy(newEmployee.employee.contact.email, emailString);
                    validFlag[4] = 1;
                }
                else {
                    printf("Invalid Input");
                }
                break;

            case '6':
                printf("\nContact No.: ");
                scanf(" %s", contactString);
                int valid = phoneValidation(contactString);
                if(valid) {
                    strcpy(newEmployee.employee.contact.phone, contactString);
                    validFlag[5] = 1;
                }
                else {
                    printf("Input invalid");
                }
                break;

            case 'c':
                for(i = 0; validFlag[i] != 0 && i < 6; i++){}
                if(i == 6) { 
                    retValue = 1;
                    exitFlag = 1;
                }
                else {
                    printf("Missing input at [%d]", i+1);
                }

                break;

            case 'e':
                exitFlag = 1;

                break;

            default:
                printf("Not a valid choice!");
        }

        printf("Press any key to continue");
        getch();
    }

    if(retValue) 
    {
        printf("Successfully created employee");
    }
    
    getch();
    return retValue;
}