#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <string>

#include "timedate.hpp"

#define CURSOR_UP(_)        "\033[" #_ "A"
#define CURSOR_DOWN(_)      "\033[" #_ "B"
#define CURSOR_LEFT(_)      "\033[" #_ "D"
#define CURSOR_RIGHT(_)     "\033[" #_ "C"
#define HIDE_CURSOR         "\033[?25l"
#define SHOW_CURSOR         "\033[?25h"

#define DELETE_LINE         "\r\033[K"

struct NonCanonicalModeTerminal // RAII purpose.
{
    termios old;

    NonCanonicalModeTerminal() : old{}
    {
        tcgetattr(STDIN_FILENO, &old);
        termios settings = old;
        settings.c_lflag &= ~(ICANON | ECHO);
        settings.c_cc[VTIME] = 0;
        settings.c_cc[VMIN] = 1;
        tcsetattr(STDIN_FILENO, TCSANOW, &settings);
#ifdef NDEBUG
        printf("\033[?25l"); // hide cursor
#endif
    }

    ~NonCanonicalModeTerminal()
    {
#ifdef NDEBUG
        printf("\033[?25h"); // show cursor
#endif
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
    }

    [[nodiscard]] unsigned char GetKey() const
    {
        unsigned char key;
        read(STDIN_FILENO, &key, 1);
        return key;
    }
};

static int cursor_x, cursor_y;

static void MoveCursorToPos(int x, int y)
{
    if (cursor_x < x) printf("\033[%dC", x - cursor_x); // right
    else if (cursor_x > x) printf("\033[%dD", cursor_x - x); // left
    if (cursor_y < y) printf("\033[%dB", y - cursor_y); // down
    else if (cursor_y > y) printf("\033[%dA", cursor_y - y); // up

    cursor_x = x;
    cursor_y = y;
}

static int year, month, day;
static int first_day_of_month;
static int days_of_month;

static void WriteCalendar()
{
    MoveCursorToPos(0, 8);
    printf(DELETE_LINE "\n" DELETE_LINE);
    ++cursor_y;
    MoveCursorToPos(0, 0);

    printf("╭──────────────────────╮\n");
    std::string str = GetMonthString(month) + " " + std::to_string(year);
    unsigned space = (20 - str.length()) / 2;
    printf("│ ");
    for (int i = 0; i < space; ++i) putchar(' ');
    printf("\033[33m%s\033[0m", str.c_str());
    for (int i = 0; i < 20 - str.length() - space; ++i) putchar(' ');
    printf(" │\n");

    printf("│ Mo Tu We Th Fr Sa \033[31mSu\033[0m │\n│ ");
    for (int i = 1; i < first_day_of_month; ++i) printf("   ");
    int week_day = first_day_of_month - 1; // range: [0, 6]

    putchar(' ');
    if (day == 1) printf("\033[44m");
    putchar('1');
    if (day == 1) printf("\033[0m");
    putchar(' ');
    week_day = (week_day + 1) % 7;

    for (int i = 2; i <= days_of_month; ++i)
    {
        if (!week_day) printf("│\n│ ");
        if (i < 10) putchar(' ');
        if (day == i) printf("\033[1m\033[44m");
        printf("%d", i);
        if (day == i) printf("\033[0m");
        putchar(' ');
        week_day = (week_day + 1) % 7;
    }

    for (int i = 0; week_day && i < 7 - week_day; ++i)
        printf("   ");
    printf("│\n");

    printf("╰──────────────────────╯\n");

    cursor_x = 0;
    cursor_y = (days_of_month + first_day_of_month - 2) / 7 + 5;
}

static void MoveCursorToSelectedDay()
{
    int d = day + first_day_of_month - 2;
    int r = d / 7, c = d % 7;
    MoveCursorToPos(2 + 3 * c, 3 + r);
}

static void NextMonth(int next, int d) // either -1 or 1
{
    month += next;
    if (!month) {
        month = 12;
        --year;
    } else if (month == 13) {
        month = 1;
        ++year;
    }

    first_day_of_month = GetDayOfTheWeek(TO_DATE(year, month, 1));
    days_of_month = GetDaysInMonth(month, IsLeapYear(year));
    day = d ? (d > days_of_month ? days_of_month : d) : (next == -1 ? days_of_month : 1);

    WriteCalendar();
    MoveCursorToSelectedDay();
}

void CalendarMain() // [5] Hello World  (13:30 - 14:15)
{
    NonCanonicalModeTerminal term;

    Date today = GetTodayDate();
    year = YEAR(today), month = MONTH(today), day = DAY(today);
    first_day_of_month = GetDayOfTheWeek(TO_DATE(year, month, 1));
    days_of_month = GetDaysInMonth(month, IsLeapYear(year));
    cursor_x = 0, cursor_y = 0;

    WriteCalendar();
    MoveCursorToSelectedDay();
    fflush(stdout);

    for (;;)
    {
        auto key = term.GetKey();

        if (key == 'q')
            break;

        switch (key)
        {
            case 'C': // right
            {
                if (day == days_of_month) {
                    NextMonth(1, 0);
                    break;
                }
                printf("%2d", day);
                ++day;
                cursor_x += 2;
                MoveCursorToSelectedDay();
                if (day < 10) putchar(' ');
                printf("\033[44m%d\033[0m\033[2D", day);
                break;
            }

            case 'D': // left
            {
                if (day == 1) {
                    NextMonth(-1, 0);
                    break;
                }
                printf("%2d", day);
                --day;
                cursor_x += 2;
                MoveCursorToSelectedDay();
                if (day < 10) putchar(' ');
                printf("\033[44m%d\033[0m\033[2D", day);
                break;
            }

            case 'A': // up
            {
                if (day <= 7) break;
                printf("%2d", day);
                day -= 7;
                printf("\033[1A\033[2D");
                if (day < 10) putchar(' ');
                printf("\033[44m%d\033[0m\033[2D", day);
                --cursor_y;
                break;
            }

            case 'B': // down
            {
                if (day > days_of_month - 7) break;
                printf("%2d", day);
                day += 7;
                printf("\033[1B\033[2D");
                if (day < 10) putchar(' ');
                printf("\033[44m%d\033[0m\033[2D", day);
                ++cursor_y;
                break;
            }

            case '<':
            {
                NextMonth(-1, day);
                break;
            }

            case '>':
            {
                NextMonth(1, day);
                break;
            }
        }

        fflush(stdout);
    }

    MoveCursorToPos(0, (days_of_month + first_day_of_month - 2) / 7 + 5);
    fflush(stdout);
}