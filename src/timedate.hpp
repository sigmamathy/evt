#pragma once

#include <string>

using Time = unsigned;
using Date = unsigned;

// Serialize time and date via byte shifting

#define HOUR(_0)    ((_0) >> 16)
#define MINUTE(_0)  ((_0) >> 8 & 0xFF)
#define SECOND(_0)  (int)((_0) & 0xFF)

#define TO_TIME(_0, _1, _2) Time(((_0) << 16) | ((_1) << 8) | (_2))

#define YEAR(_0)    ((_0) >> 16)
#define MONTH(_0)   ((_0) >> 8 & 0xFF)
#define DAY(_0)     (int)((_0) & 0xFF)

#define TO_DATE(_0, _1, _2) Date(((_0) << 16) | ((_1) << 8) | (_2))

Date GetTodayDate();

Time GetTimeNow();

int GetDayOfTheWeek(Date date);

bool IsLeapYear(int year);

int GetDaysInMonth(int month, bool leap_year);

int GetDaysSince1970(Date date);

Date GetDateFromDaysSince1970(int days);

std::string GetMonthString(int month);

std::string GetDayString(int day);

bool IsDateValid(Date date);