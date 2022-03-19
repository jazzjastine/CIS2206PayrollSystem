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
    char phone[12];         // 11-digit mobile number
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

int hash(char empID[]);
// date manipulation functions


/***** main() function - Handles the main menu and calls the subfunction *****/
int main(){
    /* Employee hash table declaration */
    employeeTable empTable;
    
    printf("\n==========================================");
    printf("\n        CIS 2206 - PAYROLL SYSTEM         ");
    printf("\n==========================================");
    initialize(empTable); // ask for input of company name
    addEmployee(empTable);
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
void initialize(employeeTable empTable)
{
    /*Variable Declaration*/
    // char companyName[32];
    // int catcher;

    // /*Clear the screen of Welcome" */
    // printf("\nPress any key to start ");
    // getch();
    // printf("\e[1;1H\e[2J"); // Similar to clrscr() function. Can't use conio.h, only for _WIN32

    // /*Start of the Program */
    // printf("\nEnter Company Name: ");
    // scanf("%s", &companyName);

    // initEmpList(companyName);
    // initAttendanceList(companyName);


    /* Variable declarations */

    /* Variable initializations */

    /* Body */

    /* Exit/return statement, if applicable */

    int i;
    for (i = 0; i < SIZE; i++) // loop for hash table initialization
    {
        strcpy(empTable[i].employee.empID, "EMPTY"); // sets each employee ID to EMPTY
        empTable[i].history = NULL;                    // sets each head pointer to NULL
        printf("%s", empTable[i].employee.empID);    // test
    }
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

int hash(char empID[8]) // String folding
{
    int i;
    char hello;
    unsigned long sum = 1;
    for(i = 0; i < 7; i++) {
        hello = empID[i];
        sum += sum * empID[i];
    }

    return sum % SIZE;
}

int insertEmployee(employeeTable empTable, employeeInfo * emp) {
    int ctr, index, value = 0;
    for(ctr = 0, index = hash(emp->employee.empID); ctr < SIZE; ctr++) {
        if(strcmp(empTable[index].employee.empID, "EMPTY") == 0) {
            empTable[index] = *emp;
            break;
        }
        index = (index%SIZE)+1;
    }
    return ctr < SIZE ? 1 : 0; // returns 1 if successful and 0 if unsuccessful
}

int isMemberEmployee(employeeTable empTable, char empID[])
{
    int ctr, index;
    for(ctr = 0, index = hash(empID); ctr < SIZE && empTable[index].employee.empID != empID; ctr++){
        if(strcmp(empTable[index].employee.empID, "EMPTY") == 0) {
            ctr = SIZE;
        }
        index = (index%SIZE)+1;
    }
    return ctr < SIZE ? 1 : 0; // returns 1 if employee exists and 0 if employee does not exist
}

int assignEmployeeID(employeeTable empTable, employeeInfo * emp)
{
    int i;
    char newID[8] = {0}, year[3], month[3], empNum[4]; // Employee ID format = year(2) month(2) increment(3) ex. 2203001
    sprintf(year, "%02d", emp->employee.dateEmployed.year);
    sprintf(month, "%02d", emp->employee.dateEmployed.month);
    for(i = 1; i < 1000; i++) { // Increments if id is already taken
        memset(newID, 0, 8); // Sets string to empty
        strcat(newID, year);
        strcat(newID, month);
        sprintf(empNum, "%03d", i);
        strcat(newID, empNum);
        if(!isMemberEmployee(empTable, newID)) {
            strcpy(emp->employee.empID, newID);
            break;
        }
    }
    return (i < 1000)? 1 : 0; // returns 1 if successful and 0 if unsuccessful
}

int dateValidation(int month, int day, int year)
{
    int retval=0;
    int daysOfMonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};

    if(month>0&&month<13){
    	printf("\nwhy\n");
        if(month==2&&year%4==0){
           if(daysOfMonth[month-1]+1>=day){
            	retval=1;
            } 
        }else if(daysOfMonth[month-1]>=day){
        	retval=1;
        } 
    }
    return retval;
}

int emailValidation(char email[]) // magic code
{
    int emailCheck = 0;
    int i;

    for(i = 0; email[i] != '\0'; i++){
        if(!emailCheck && email[i] == '@' && i != 0) {
            emailCheck++;
        }
        else if(email[i] == '@'){
            emailCheck = 0;
        }
        if(emailCheck && email[i] == '.') {
            emailCheck = (email[i-1] != '@')? emailCheck+1 : 0;

        }
    }

    printf("emailcheckfinal: %d", emailCheck);
    return (emailCheck >= 2)? 1 : 0;
}

int phoneValidation(char phone[])
{
    int i;
    for(i = 0; phone[i]!='\0' && isdigit(phone[i]); i++){}
    return (i == 11)? 1 : 0;
}

int payValidation(float amount)
{
	return (amount>0)?1:0;	
}

int addEmployee(employeeTable empTable) // returns 0 if unsuccessful and 1 if successful
{
    char choice; // e = exit, c = create
    char dateString[11];
    char emailString[32];
    char contactString[12];
    int validFlag[9] = {0};
    int retValue = 0;
    int exitFlag = 0;
    int month, day, year;
    float basicSalary=0, overtimePay=0, contributions=0;
    int status;
    int i;

    employeeInfo newEmployee = 
    {
        .employee =
        {
            .empID = 0,
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
        printf("\n[1] Last Name:            \t%s", newEmployee.employee.name.LName);
        printf("\n[2] First Name:           \t%s", newEmployee.employee.name.fName);
        printf("\n[3] MI:                   \t%c", newEmployee.employee.name.MI);
        printf("\n[4] Date(MM/DD/YY):       \t%02d/%02d/%02d", newEmployee.employee.dateEmployed.month, newEmployee.employee.dateEmployed.day, newEmployee.employee.dateEmployed.year);
        printf("\n[5] Email:                \t%s", newEmployee.employee.contact.email);
        printf("\n[6] Contact No.:          \t%s", newEmployee.employee.contact.phone);
        printf("\n[7] Basic Salary:         \t%.02f", newEmployee.employee.details.basicSalary);
        printf("\n[8] Overtime Pay:         \t%.02f", newEmployee.employee.details.overtimePay);
        printf("\n[9] Total Contributions:  \t%.02f", newEmployee.employee.details.contributions);
        printf("\n[0] Employee Status:      \t%s", (newEmployee.employee.status)?"Inactive":"Active");
        printf("\n==========================================");
        printf("\n[c] create employee | [e] exit ");
        printf("\n\nChoice: ");
        scanf(" %c", &choice);
        getchar();
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
                scanf(" %c", &newEmployee.employee.name.MI);
                getchar();
                validFlag[2] = 1;
                break;

            case '4':
                printf("\nDateDate(MM/DD/YY): ");
                scanf(" %s", dateString);
                
                char * token = strtok(dateString, "/");
                month = atoi(token);
                token = strtok(NULL, "/");
                day = atoi(token);
                token = strtok(NULL, "/");
                year = atoi(token);
                
                if(dateValidation(month, day, year)){
                	getch();
	                newEmployee.employee.dateEmployed.month = month;
	                token = strtok(NULL, "/");
	                newEmployee.employee.dateEmployed.day = day;
	                token = strtok(NULL, "/");
	                newEmployee.employee.dateEmployed.year = year;
                    validFlag[3] = 1;
                }
                else {
                    printf("Invalid input\n");
                }
                
                break;

            case '5':
                printf("\nEmail: ");
                scanf(" %s", emailString);              
                if(emailValidation(emailString)){
                    strcpy(newEmployee.employee.contact.email, emailString);
                    validFlag[4] = 1;
                }
                else {
                    printf("Invalid Input\n");
                }

                break;

            case '6':
                printf("\nContact No.: ");
                scanf(" %s", contactString);
                if(phoneValidation(contactString)) {
                    strcpy(newEmployee.employee.contact.phone, contactString);
                    validFlag[5] = 1;   
                }
                else {
                    printf("Input invalid\n");
                }
                
                break;

			case '7':
				printf("\nBasic Salary: ");
				scanf(" %f", &basicSalary);
				if(payValidation(basicSalary)){
					newEmployee.employee.details.basicSalary=basicSalary;
					validFlag[6] = 1;
				}else{
					printf("Input invalid");
				}
				break;
				
			case '8':
				printf("\nOvertime Pay: ");
				scanf(" %f", &overtimePay);
				if(payValidation(overtimePay)){
					newEmployee.employee.details.overtimePay=overtimePay;
					validFlag[7] = 1;
				}else{
					printf("Input invalid");
				}
				break;
				
			case '9':
				printf("\nTotal Contributions ");
				scanf(" %f", &contributions);
				if(payValidation(contributions)){
					newEmployee.employee.details.contributions=contributions;
					validFlag[8] = 1;
				}else{
					printf("Input invalid");
				}
				break;
				
			case '0':
                printf("\n[0] Active");
				printf("\n[1] Inactive");
				printf("\nStatus: ");
                scanf(" %c", &choice);
                if(choice=='0'||choice=='1'){
                        newEmployee.employee.status=choice-'0';
                }else{
                    printf("Invalid input");
                }
				break;
				
            case 'c':
                for(i = 0; validFlag[i] != 0 && i < 6; i++){}
                if(i == 6) { 
                    retValue = 1;
                    exitFlag = 1;
                    if(assignEmployeeID(empTable, &newEmployee)) {
                        if(insertEmployee(empTable, &newEmployee)) {
                            printf("Successfully created employee");
                        }
                        else {
                            printf("Employee Table is full\n");
                        }
                    }
                    else {
                        printf("Failed to assign Employee ID\n");
                    }
                }
                else {
                    printf("Missing input at [%d]\n", i+1);
                }
                break;

            case 'e':
                exitFlag = 1;
                break;

            default:
                printf("Not a valid choice!\n");
        }

        printf("\nPress any key to continue");
        getch();
    }
    
    getch();
    return retValue;
}