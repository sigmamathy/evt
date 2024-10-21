#include "timedate.hpp"
#include <ctime>

Date GetTodayDate()
{
    time_t t = time(nullptr);
    tm tm = *localtime(&t);

    return TO_DATE(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

int GetDayOfTheWeek(Date date)
{
    int m = MONTH(date), y = YEAR(date);

    if (m < 3) {
        m += 12;
        y--;
    }

    int k = y % 100, j = y / 100;
    int h = (DAY(date) + 13*(m+1)/5 + k + k/4 + j/4 + 5*j) % 7; // WTF? is this magic or something?

    return (h - 2) % 7 + 1;

}

bool IsLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int GetDaysInMonth(int month, bool leap_year)
{
    int const days[] = {31, leap_year ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return days[month - 1];
}

int GetDaysSince1970(Date date)
{
    int days = DAY(date);
    int year = YEAR(date);

    for (int y = 1970; y < year; ++y)
        days += IsLeapYear(y) ? 366 : 365;

    for (int m = 1; m < MONTH(date); m++)
        days += GetDaysInMonth(m, IsLeapYear(year));

    return days;
}