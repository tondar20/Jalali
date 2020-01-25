/*
  Jalali.h - Library for convert Gregorian to Persian calendar.
  Created by Tondar, January 21, 2020.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Jalali.h"

const PROGMEM long JALALI_ZERO = 1947954;
const PROGMEM int LOWER_PERSIAN_YEAR = 1;
const PROGMEM int UPPER_PERSIAN_YEAR = 2324;
const PROGMEM long gregorian_CALENDAR_EPOCH = 1721060;

const PROGMEM int LOWER_GREGORIAN_YEAR = 623;
const PROGMEM int UPPER_GREGORIAN_YEAR = 2945;
int gmonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int jmonths[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};

long jalali_jd0(int jalali_year)
{
    const PROGMEM int breaks[] = {6, 394, 720, 786, 1145, 1635, 1701, 1866, 2328};
    const PROGMEM int deltas[] = {1249, 952, 891, 930, 866, 869, 844, 848, 852};
    long rval;
    long l = jalali_year;

    for (int i = 0; i < 9; i++)
        if (jalali_year < breaks[i])
        {
            rval = JALALI_ZERO + l * 365 + (deltas[i] + l * 303) / 1250;
            return (rval);
        }
}

long gregorian_jd0(int gregorian_year)
{
    long rval;
    int y;

    long l = gregorian_year;
    rval = l * 365 + l / 4;
    rval += -l / 100 + l / 400;

    if (Jalali::is_year_leap(gregorian_year, GREGORIAN))
        rval--;

    rval += gregorian_CALENDAR_EPOCH + 1;
    return rval;
}

long date_to_days(int year, int month, int day, int calendar)
{
    long days;
    if (calendar == JALALI)
    {
        days = jalali_jd0(year) + 1;
        for (int i = 0; i < month - 1; i++)
            days += jmonths[i];
    }
    else
    {
        days = gregorian_jd0(year);
        for (int i = 0; i < month - 1; i++)
            days += gmonths[i];
    }
    days += day - 1;
    return days;
}

void days_to_date(long jd, int &year, int &month, int &day, int calendar)
{
    long days0;
    long days1;
    int m;

    if (calendar == JALALI)
        year = (jd - JALALI_ZERO) / 365;
    else
        year = (jd - gregorian_CALENDAR_EPOCH) / 365;

    do
    {
        if (calendar == JALALI)
        {
            days0 = jalali_jd0(year) + 1;
            days1 = jalali_jd0(year + 1) + 1;
            jmonths[11] = days1 - days0 - 336;
        }
        else
        {
            days0 = gregorian_jd0(year);
            days1 = days0;
            for (int i = 0; i < 12; i++)
                days1 += gmonths[i];
        }

        if (days0 > jd)
            year--;
        if (days1 <= jd)
            year++;
    } while (days0 > jd || days1 <= jd);

    for (int i = 0; i < 12; i++)
    {
        day = jd - days0;
        if (calendar == JALALI)
            m = jmonths[i];
        else
            m = gmonths[i];
        if (day < m)
        {
            month = i + 1;
            day++;
            return;
        }
        days0 += m;
    }
}

bool Jalali::is_year_leap(int year, int calendar)
{
    if (calendar == JALALI)
    {
        if (year > UPPER_PERSIAN_YEAR || year < LOWER_PERSIAN_YEAR)
            return false;
        long days0;
        long days1;
        days0 = jalali_jd0(year) + 1;
        days1 = jalali_jd0(year + 1) + 1;
        if ((days1 - days0) == 366)
        {
            jmonths[11] = 30;
            return true;
        }
        jmonths[11] = 29;
        return false;
    }
    else
    {
        if (year > UPPER_GREGORIAN_YEAR || year < LOWER_GREGORIAN_YEAR)
            return false;
        if ((year % 4) == 0)
            if ((year % 100) != 0 || (year % 400) == 0)
            {
                gmonths[1] = 29;
                return true;
            }
        gmonths[1] = 28;
        return false;
    }
}

bool Jalali::is_date_valid(int year, int month, int day, int calendar)
{
    if (month > 12 || month < 1)
        return false;
    if (day > 31 || day < 1)
        return false;

    int mnt;
    month--;

    if (calendar == JALALI)
    {
        if (year > UPPER_PERSIAN_YEAR || year < LOWER_PERSIAN_YEAR)
            return false;
        if (month == 11)
            is_year_leap(year, calendar);
        mnt = jmonths[month];
    }
    else
    {
        if (year > UPPER_GREGORIAN_YEAR || year < LOWER_GREGORIAN_YEAR)
            return false;
        if (month == 1)
            is_year_leap(year, calendar);
        mnt = gmonths[month];
    }

    if (day > mnt)
        return false;
    return true;
}

void Jalali::convert_date(int year, int month, int day, int &cyear, int &cmonth, int &cday, int calendar)
{
    if (!is_date_valid(year, month, day, calendar))
    {
        cday = cmonth = cyear = 0;
        return;
    }

    long jd = date_to_days(year, month, day, calendar);
    days_to_date(jd, cyear, cmonth, cday, !calendar);
}

int Jalali::day_of_week(int year, int month, int day, int calendar)
{
    if (!is_date_valid(year, month, day, calendar))
        return 0;

    long l = date_to_days(year, month, day, calendar);
    l %= 7;
    l += 3;
    if (l > 7)
        l -= 7;
    return l;
}
