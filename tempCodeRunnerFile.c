printf("\n==========================================");
    printf("\n           VIEW SPECIFIC EMPLOYEE	        ");
    printf("\n==========================================");
    printf("\nLast Name:              \t%c", empTable[index].employee.name.LName);
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
    printf("\n==========================================");
    printf("\n\nPress any key to exit...");
    getch();