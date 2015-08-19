#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>

struct date
{
  int year;
  int month; 
  int day;
}lo_date,hi_date,now_date;

int check_date_format(int year, int month, int day);
int falls_in_limit(int year, int month, int day);
int days_in_month(int year,int month);
struct date update ( struct date s,int new_offset);
int convert_date(int year, int month, int day);

#define DATE_LEN 20
#define ADVNCD_BOOK_STRT 7
#define ADVNCD_BOOK_END 14
#define START_YEAR 1999
#define END_YEAR 2016

int main()
{
	
  char date[DATE_LEN];
	int date_validate_flag=0, i, year, month, day;

  while(date_validate_flag==0)
  {
    i=0, year=0, month=0, day=0;
    printf("Enter a date in the yyyy/mm/dd format\n");
    scanf("%19s", date);

    while(date[i]!='/')
    {
      year=year*10+date[i]-'0';
      i++;
    }
    i=i+1;

    while(date[i]!='/')
    {
      month=month*10+date[i]-'0';
       i++;
    }
    i++;

    while(date[i]!='\0')
    {
      day=day*10+date[i]-'0';
      i++;
    }

   if( check_date_format(year, month, day)==1 && falls_in_limit(year, month, day)==1)
      date_validate_flag=1;
    
 }

  printf("The date has been accepted \n");
	return(0);
}

//-------------------------------------------------------------------------------

int check_date_format(int year, int month, int day)
{
	int dateflag=0, is_leap_year;
  
  is_leap_year= ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))?1:0;

	if(!(year>START_YEAR && year<END_YEAR))
	{
		printf("Invalid year\n");
		dateflag=1;
	}
  else
	 if(!(month>0&&month<13))
	 {
	   	printf("Invalid month\n");
		  dateflag=1;
	 }
    else
	   if(!(day>0&&day<32))
	   {
		    printf("Invalid day\n");
		    dateflag=1;
	   }
      else
        if( !((month==2) && ((is_leap_year==1)&&day<30)||day<29))
        {
          printf("Invalid day-month-year combination\n");
          dateflag=1;
        }	
        else
          if(!(((month == 4 || month == 6 || month == 9 || month == 11)&&day<31)||day<32))
          {
            printf("Invalid day-month combination\n");
            dateflag=1;
          }
    

      if(dateflag==1)
        return(0);
      else
        return(1);
}

//-------------------------------------------------------------------------------

int falls_in_limit(int uyear, int umonth, int uday)
{
  /*Used to get local time*/
  time_t t = time(NULL);                  //To get system time
  struct tm tm = *localtime(&t);    //To assign time to the tm struct
  int input_secs, lo_secs, hi_secs;

  /**/
  printf("Today: %d-%d-%d \n", tm.tm_year + 1900, tm.tm_mon + 1 , tm.tm_mday);
  now_date.year=tm.tm_year + 1900;
  now_date.month=tm.tm_mon + 1;
  now_date.day=tm.tm_mday;

  lo_date=update(now_date,ADVNCD_BOOK_STRT);
  printf("\n Booking allowed from: %d-%d-%d \n", lo_date.year, lo_date.month , lo_date.day);
  hi_date=update(now_date,ADVNCD_BOOK_END);
  printf("Booking allowed to: %d-%d-%d \n", hi_date.year, hi_date.month, hi_date.day);

  input_secs=convert_date(uyear,umonth,uday);
  lo_secs=convert_date(lo_date.year, lo_date.month , lo_date.day);
  hi_secs=convert_date(hi_date.year, hi_date.month, hi_date.day);

  if(lo_secs<input_secs && input_secs<hi_secs)
  {
    return(1);
  }
  else
  {
    printf("Does not fall within the specified booking range\n");
    return(0);
  }

}

//--------------------------------------------------------------------------------

int days_in_month(int year,int month)
{
  int numberOfDays,is_leap_year;

if (month == 4 || month == 6 || month == 9 || month == 11)  
  numberOfDays = 30;  
else if (month == 2)  
{  
  is_leap_year= ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))?1:0;  
  
  if (is_leap_year==1)  
    numberOfDays = 29;  
  else  
    numberOfDays = 28;  
}  
else  
  numberOfDays = 31;
  return(numberOfDays);
}

//-------------------------------------------------------------------------------

struct date update (struct date s,int new_offset)
{
    int offset=0;
    s.day += new_offset - offset;
   
    while (s.day > days_in_month (s.year, s.month)) 
    {
        s.day -= days_in_month (s.year, s.month);
        s.month++;
        if (s.month > 12) 
        {
            s.month = 1;
            s.year++;
        }
    }
    offset = new_offset;
    return s;
}
//------------------------------------------------------------------------------
int convert_date(int year,int month,int day)
{
   return((year*12+month)*days_in_month(year, month)+day);
} 
