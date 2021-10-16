/**********************************************************************/
/*                                                                    */
/* This program manages a company's employee bonus program. A company */
/* enters in its name, number of employees, the year in which the     */
/* bonuses are given, the yearly bonus amount per employee, and each  */
/* employees number of service years. The year hired and the bonus    */
/* amount is calculated for each employee and then it prints the      */
/* employee bonus information.                                        */
/*                                                                    */
/**********************************************************************/

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <new>
using namespace std;

#define MAX_NAME_LENGTH      80      // The maximum length for a name
#define COMPANY_ALLOC_ERROR  1       // Can't allocate company info memory
#define NAME_ALLOC_ERROR     2       // Can't allocate company name memory
#define EMPLOYEE_ALLOC_ERROR 3       // Can't allocate employee db memory

// The company information
struct company_info
{
   char  *p_company_name;
   int   bonus_year,
         employees;
   float bonus_amount;
};

// A company's employee bonus record
class bonus_record
{
   int   employee_id,
         service_years,
         year_hired;
   float bonus_amount;
public:
   // Sets the information for the employee bonus record
   void  set_id            (int   id ) {employee_id   = id;}
   void  set_service_years (int   s_y) {service_years = s_y;}
   void  set_year_hired    (int   y_h) {year_hired    = y_h;}
   void  set_bonus_amount  (float b_a) {bonus_amount  = b_a;}

   // Gets the information for the employee bonus record
   int   get_id            (         ) {return employee_id;}
   int   get_service_years (         ) {return service_years;}
   int   get_year_hired    (         ) {return year_hired;}
   float get_bonus_amount  (         ) {return bonus_amount;}

   // Deletes the employee bonus record
         ~bonus_record     (         ) {cout << "\n\nDestructor "
                                             << "executing ...";}
};

void print_instructions();
company_info *get_company_info();
bonus_record *get_employee_record(company_info company_info);
void print_employee_info(const char *sort_order,
                         bonus_record *p_employee_database, int employees);
void sort_employee_records(bonus_record *p_employee_database, int employees);
void fatal_error(const char *p_function_name, const char *p_type,
                 int error_number);

int main()
{
   company_info *p_company_info;
   bonus_record *p_employee_database = NULL;

   print_instructions();

   p_company_info = get_company_info();
   cout << "\n\nCompany name:        "  << p_company_info->p_company_name;
   cout <<   "\nYear of the bonuses: "  << p_company_info->bonus_year;
   cout <<   "\nNumber of employees: "  << p_company_info->employees;
   cout <<   "\nBonus per year:      $" << p_company_info->bonus_amount;

   p_employee_database = get_employee_record(*p_company_info);
   print_employee_info  ("IN UNSORTED ORDER", p_employee_database,
                         p_company_info->employees);

   sort_employee_records(p_employee_database,
                         p_company_info->employees);
   print_employee_info  ("SORTED BY YEAR HIRED", p_employee_database,
                         p_company_info->employees);

   delete p_company_info;
   delete [] p_employee_database;
   //delete [] p_company_information->p_company_name;

   cout << "\n\nThanks for processing employee bonuses today ;)";
   cout << "\n\n\n\n\n\n\n\n";
   return 0;
}

void print_instructions()
{
   cout << "\n========================================================";
   cout << "\nThis program asks for information about your company and";
   cout << "\nabout each employee. It then calculates the bonus amount";
   cout << "\nowed each employee based on the number of service years.";
   return;
}

company_info *get_company_info()
{
   company_info *p_company_information = NULL;
   char         company_name[MAX_NAME_LENGTH + 1];

   // Allocates memory for the company information and aborts if no
   // memory is available
   try
   {
      p_company_information = new company_info;
   }
   catch(bad_alloc xa)
   {
      fatal_error("get_company_info()", "company information",
                  COMPANY_ALLOC_ERROR);
   }

   // Allocates memory for the company name and aborts if no memory
   // is available
   cout << "\n\n\nEnter the name of your company here (no spaces): ";
   cin  >> company_name;
   try
   {
      p_company_information->p_company_name =
         new char[strlen(company_name) + 1];
   }
   catch (bad_alloc xa)
   {
      fatal_error("get_company_info()", "company information",
                  COMPANY_ALLOC_ERROR);
   }
   strcpy(p_company_information->p_company_name, company_name);
   // strcpy_s(p_company_information->p_company_name,
   //          strlen(company_name) + 1, company_name);


   // Loops processing employee bonus records until all employees are recorded
   do
   {
      cout << "Enter your number of employees (1 or more): ";
      cin  >> p_company_information->employees;
   }
   while(p_company_information->employees < 1);

   cout << "Enter the year in which the bonuses are given (YYYY): ";
   cin  >> p_company_information->bonus_year;

   cout << "Give the yearly bonus amount per employee (in dollars): ";
   cin  >> p_company_information->bonus_amount;

   return p_company_information;
}

bonus_record *get_employee_record(company_info company_information)
{
   int          employee_id = 1,
                service_years;
   bonus_record *p_employee_start = NULL,
                *p_employee;

   // Allocates memory for the employee database and aborts if no memory
   // is available
   try
   {
     p_employee_start = new bonus_record[company_information.employees];
   }
   catch (bad_alloc xa)
   {
      fatal_error("get_employee_record()", "employee database",
                  EMPLOYEE_ALLOC_ERROR);
   }

   p_employee = p_employee_start;

   // Loops processing employee database records until every employee's
   // record is complete
   do
   {
      // Loops processing employee service years until all employees
      // service years have been entered
      do
      {
         cout << "\n\n\nEnter the number of service years of "
              << "employee #" << employee_id << ".";
         cout << "\nEnter 0 (zero) if this employee does not exist: ";
         cin  >> service_years;
         if (service_years < 0)
         {
            cout << "\nThe service years must be 0 or greater.";
            cout << "\nPlease reenter the number of service years.";
         }
      }
      while (service_years < 0);

      // Checks to see if the user enters a valid value
      if    (service_years > 0)
      {
         p_employee->set_id           (employee_id);
         p_employee->set_service_years(service_years);
         p_employee->set_year_hired   (company_information.bonus_year -
                                       service_years);
         p_employee->set_bonus_amount (p_employee->get_service_years() *
                                       company_information.bonus_amount);
         p_employee += 1;
      }
      employee_id += 1;
   }
   while ((p_employee - p_employee_start) < company_information.employees);

   return p_employee_start;
}

void print_employee_info(const char         *sort_order,
                               bonus_record *p_employee_start,
                               int          employees)
{
  bonus_record *p_employee;

   cout << "\n\nHere is the employee database, "<< sort_order << ":";
   cout <<   "\n====================================================";
   cout <<   "\nEmployee Id   Service Years   Year Hired   Bonus Amt";
   cout <<   "\n-----------   -------------   ----------   ---------";

   for(p_employee = p_employee_start;
      (p_employee - p_employee_start) < employees; p_employee++)
   {
      cout << "\n     "        << p_employee->get_id           ()
           << "              " << p_employee->get_service_years()
           << "            "   << p_employee->get_year_hired   ()
           << "        $"      << p_employee->get_bonus_amount ();
   }

   return;
}

void sort_employee_records(bonus_record *p_employee_start,
                           int employees)
{
   int          count_sort;
   bonus_record temp_employee,
                *p_employee;

   for (count_sort = 1; count_sort <= employees - 1; count_sort++)
   {
      for (p_employee = p_employee_start;
          (p_employee - p_employee_start) < employees - count_sort; p_employee++)
      {
         if (p_employee->get_year_hired() > (p_employee + 1)->get_year_hired())
         {
            temp_employee     = *p_employee;
            *p_employee       = *(p_employee + 1);
            *(p_employee + 1) = temp_employee;
         }
      }
   }

   return;
}

void fatal_error(const char *p_function_name, const char *p_type,
                 int error_number)
{
   cout << "\nError #"             << error_number
        << " occured in function " << p_function_name << ".";
   cout << "\nUnable to allocate memory "
        << "for the "              << p_type          << ".";
   cout << "\nThe program is aborting.";
   exit(error_number);

   return;
}
