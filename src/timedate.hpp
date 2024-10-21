#pragma once

#include <string>

using Date = unsigned;

// Serialize date via byte shifting

#define YEAR(_0)    ((_0) >> 16)
#define MONTH(_0)   ((_0) >> 8 & 0xFF)
#define DAY(_0)     (int)((_0) & 0xFF)

#define TO_DATE(_0, _1, _2) Date(((_0) << 16) | ((_1) << 8) | (_2))

Date GetTodayDate();

int GetDayOfTheWeek(Date date);

bool IsLeapYear(int year);

int GetDaysInMonth(int month, bool leap_year);

int GetDaysSince1970(Date date);

std::string GetMonthString(int month);

std::string GetDayString(int day);

bool IsDateValid(Date date);