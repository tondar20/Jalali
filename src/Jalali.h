/*
  Jalali.h - Library for convert Gregorian to Persian calendar.
  Created by Tondar, January 21, 2020.
  Released into the public domain.
*/

#ifndef Jalali_h
#define Jalali_h

#define JALALI 0x0
#define GREGORIAN 0x1

#include "Arduino.h"

/// Convert dates between Gregorian <--> Jalali (Persian)
class Jalali
{
public:
  // check if the year is leap
  // year: int
  // calendar: GREGORIAN | JALALI
  // returns 0 if year is out of range
  static bool is_year_leap(int year, int calendar);
  // check if the date is valid
  // year, month, day: int
  // calendar: GREGORIAN | JALALI
  static bool is_date_valid(int year, int month, int day, int calendar);
  // convert the date
  // year, month, day: int
  // calendar (source date): GREGORIAN | JALALI
  void convert_date(int year, int month, int day, int &cyear, int &cmonth, int &cday, int calendar);
  // get weekday name
  // year, month, day: int
  // calendar: GREGORIAN | JALALI
  // returns 1-7 for sat-fri, 0 for invalid date
  int day_of_week(int year, int month, int day, int calendar);

private:
};

#endif
