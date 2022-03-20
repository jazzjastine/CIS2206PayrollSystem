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
#define EMP_FILENAME "EmployeeFile.bin"
#define ATT_FILENAME "AttendanceFile.bin"
#define PAY_FILENAME "PayrollIDList.bin"
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
    unsigned int year:11;    // YY
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

// addEmployee();
// editEmployee(char empID[], int mode);
void createPayroll();
dateDetails getDate();
char * generatePayrollID(char group[],dateDetails date);
long dateDiff(dateDetails s,dateDetails e);
long checkYear(int xyr,int yyr);
long dayOfYear(dateDetails date);
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

   createPayroll();

    // getch();
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

void createPayroll(){
    long diff;
    char group[10];
    char *payrollID;
    dateDetails start_date ,end_date;

    printf("\nEnter Department/Group #: ");
    gets(group);
    printf("\nEnter Start Date [MM/DD/YYYY]: ");  
    start_date = getDate();
    printf("\nEnter End Date [MM/DD/YYYY]: ");  
    end_date = getDate();
 
    // generate Payroll ID
    payrollID = generatePayrollID(group,start_date);
    puts(payrollID);
    
    //date difference
    diff = dateDiff(start_date,end_date);
    printf("\n\n Date difference is %d days\n",diff);

    

}


long dateDiff(dateDetails s,dateDetails e){
  long num ;
  num =dayOfYear(e) - dayOfYear(s);
  if(s.year != e.year){
    if(s.year < e.year){
      num  += checkYear(s.year,e.year);
    }else{
      num  -= checkYear(e.year,s.year);
    }
  }
  return num ;
}

long checkYear(int xyr,int yyr){ 
  long jj,bb;
  bb=0;
  for(jj= xyr;jj<yyr;jj++){
    bb+=365;
    if(((((jj%400)==0)||((jj%100)!=0))
      &&((jj%4)==0))) bb+=1;
  }
  return(bb);
}

long dayOfYear(dateDetails date){ 
    long a,r[13];

    r[1] = 0; r[2] = 31; r[3] = 59;
    r[4] = 90; r[5] = 120; r[6] = 151;
    r[7] = 181; r[8] = 212; r[9] = 243;
    r[10]= 273; r[11]= 304; r[12]= 334;
    a=r[date.month]+date.day;
    if(((((date.year%400)==0)||((date.year%100)!=0))
        &&((date.year%4)==0))&&(date.month>2)) a+=1;
    return(a);
}


char * generatePayrollID(char group[],dateDetails date){
    char *payrollID = (char *) malloc(sizeof(char)*6);
    char smon[5],syear[5],mon[5] ="0";
    itoa(date.year,syear,10);
    itoa(date.month,smon,10);

    strncpy(payrollID, syear + 2,3);
    if(date.month < 10){
        strcat(mon,smon);
        strcat(payrollID,mon);
    }else{
        strcat(payrollID,smon);
    }
    strcat(payrollID,group);
    
    return payrollID;
}

dateDetails getDate(){
    unsigned int mon,day,year;
    dateDetails retval;

    scanf("%d/%d/%d",&mon,&day,&year);

    retval.day = day;
    retval.month = mon;
    retval.year = year;

    return retval;
}