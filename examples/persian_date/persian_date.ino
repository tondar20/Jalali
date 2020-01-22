/*
 date ranges:
 from Persian 1-1-1 to 2324-12-29
 from Greforian = 623-1-1 to 2945-12-31
*/

#include <Jalali.h>

Jalali jal;

String day_name(int wd)
{
    switch (wd)
    {
    case 1:
        return "sat";
    case 2:
        return "sun";
    case 3:
        return "mon";
    case 4:
        return "tue";
    case 5:
        return "wed";
    case 6:
        return "thu";
    case 7:
        return "fri";
    }
}

void setup()
{
    Serial.begin(115200);

    bool b;
    int year, month, day, week_day;
    char buf[30];

    b = jal.is_date_valid(2021, 4, 31, GREGORIAN);
    Serial.print("gregorian 2021-4-31 is ");
    Serial.println(b ? "valid" : "invalid");

    b = jal.is_date_valid(1398, 10, 30, JALALI);
    Serial.print("jalali 1398-10-30 is ");
    Serial.println(b ? "valid" : "invalid");

    Serial.println();

    b = jal.is_year_leap(2021, GREGORIAN);
    Serial.print("gregorian 2021 is ");
    Serial.println(b ? "leap" : "not leap");

    b = jal.is_year_leap(1399, JALALI);
    Serial.print("jalali 1399 is ");
    Serial.println(b ? "leap" : "not leap");

    Serial.println();

    jal.convert_date(2021, 3, 20, year, month, day, GREGORIAN);
    sprintf(buf, "2021-3-20 to jalali: %04d-%02d-%02d", year, month, day);
    Serial.println(buf);

    jal.convert_date(1398, 12, 10, year, month, day, JALALI);
    sprintf(buf, "1398-12-10 to gregorian: %04d-%02d-%02d", year, month, day);
    Serial.println(buf);

    Serial.println();
    week_day = jal.day_of_week(1398, 3, 30, JALALI);
    Serial.print("1398-3-30 is ");
    Serial.println(day_name(week_day));

    week_day = jal.day_of_week(2020, 1, 22, GREGORIAN);
    Serial.print("2020-1-22 is ");
    Serial.println(day_name(week_day));
}

void loop()
{
}
