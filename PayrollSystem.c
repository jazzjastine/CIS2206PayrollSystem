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
#define MAX 512
#define EMP_FILENAME 'EmployeeFile.bin'
#define ATT_FILENAME 'AttendanceFile.bin'
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
    char empID[8];          // employee ID
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
typedef enum{
    OCCUPIED, EMPTY, DELETED      // occupancy of a hash table bucket
}cellStatus;

typedef struct{
    cellStatus cellinfo;
    employeeInfo employee;
}employeeTable;

typedef struct node{
    int data;
    struct node *link;
}nodeType, *employeeList;             // the structure for sorting the employees by department

/***** FUNCTION PROTOTYPES *****/
void initialize();          // initialize by loading existing file. If none, new file will be created
void terminate();           // properly terminate the file by freeing all dynamic memory (attendance LL)
void viewEmployeeList();                                              // view entire list of employees
void sortEmployees(employeeList listEmployee[]);                      // sort employees by department
void displayEmployees(employeeList listEmployee[], char dept);        // display employee given by a department

typedef employeeInfo employeeTable[MAX];  // constant hash table

/* Employee hash table declaration */
employeeTable empTable;

/***** main() function - Handles the main menu and calls the subfunction *****/
int main(){
    printf("\n==========================================");
    printf("\n        CIS 2206 - PAYROLL SYSTEM         ");
    printf("\n==========================================");
    initialize();

    /* MAIN MENU */

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

/**
 * @brief - view entire list of employees or by department
 * @param describe the parameters
 * @return state what the function returns
 */
void viewEmployeeList(){
    printf("\n\n---------------------------------");
    printf("\nEmployee List");
    printf("\n---------------------------------");

    /* Employee linked list declaration */
    employeeList listEmployee[26]; 
    int x;
    for(x=0; x<26;x++){
        listEmployee[x] = NULL;
    }

    sortEmployees(listEmployee);
    
    for(x=0; x<26;x++){
        employeeList trav;
        for(trav=listEmployee[x]; trav!=NULL; trav=trav->link){
            printf("\n%d", trav->data);
        }
    }
    int choice, flag = 1;
    
    while(flag){
        printf("\n[1] Display all employees");
        printf("\n[2] Display employees of a department");
        printf("\nEnter a number: ");
        scanf("%d", &choice);
        switch(choice){
            char ch;
            case 1:
                //display all employees
                flag = 0;
                for(ch = 'A'; ch <= 'Z'; ch++){
                    displayEmployees(listEmployee, ch);
                }
                break;
            case 2:
                //display all employees of a department
                flag = 0;
                do{
                    printf("Please enter a department[A-Z]: ");
                    scanf(" %c", &ch);
                }while(!isalpha(ch)); 
                displayEmployees(listEmployee, ch);
                break;
            default:
                printf("Please enter 1 or 2");
                break;
        }
    }
}

/**
 * @brief - display all employees in a given department
 * @param - list of employees, department name
 * @return state what the function returns
 */
void displayEmployees(employeeList listEmployee[], char dept){
    dept = toupper(dept);
    printf("\nDepartment %c", dept);
    printf("\n----------------------------------");
    int index = dept - 'A';
    employeeList trav, ptr;

    for(trav = listEmployee[index]; trav!=NULL;){
        employeeDetails temp = empTable[trav->data].employee;
        printf("\n%10s %20s, %20s %10c", temp.empID, temp.name.LName, temp.name.fName, temp.name.MI);
        ptr = trav;
        trav = trav->link;
        free(ptr);
    }
}

/**
 * @brief - sort employees by department
 * @param - list of employees
 * @return state what the function returns
 */
void sortEmployees(employeeList listEmployee[]){
    int x;
    attendanceHistory ptr;
    for(x = 0; x < MAX; x++){
        employeeInfo temp = empTable[x];
        if(strcmp(temp.employee.empID, "EMPTY")!=0 && temp.history!=NULL){
            printf("\n%d");
            ptr = temp.history;
            int index = ptr->attendance.payrollID[strlen(ptr->attendance.payrollID)-1] - 'A';
            
            /*insert the index of the employee to their department - sorted by empID*/
            employeeList *trav;
            for(trav = &listEmployee[index]; *trav!=NULL && (strcmp(empTable[(*trav)->data].employee.empID, temp.employee.empID)<0); trav = &(*trav)->link){}
                employeeList new = calloc(1, sizeof(struct node));
                new->data = x;
                new->link = *trav;
                *trav = new;
        }
    }
}