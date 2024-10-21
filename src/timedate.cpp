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

    h -= 2;
    if (h < 0) h += 7;
    return h + 1;
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

std::string GetMonthString(int month)
{
    static char const* str[] = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
    };

    return str[month - 1];
}

std::string GetDayString(int day)
{
    static char const* str[] = {
        "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"
    };

    return str[day - 1];
}