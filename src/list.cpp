#include "list.hpp"
#include "timedate.hpp"
#include "dataio.hpp"
#include <iostream>

int ListEvents_Main()
{
    if (!CreateFilesIfNotExists()) return 1;
    auto records = ReadEventRecords();

    size_t l_name = 4, l_loc = 8;
    for (auto& rec : records) {
        l_name = std::max(l_name, rec.name.size());
        l_loc = std::max(l_loc, rec.location.size());
    }

    printf("\n\033[47m\033[30mID    Course      Name");
    for (int i = 0; i < l_name - 4; ++i) putchar(' ');
    printf("    Location");
    for (int i = 0; i < l_loc - 8; ++i) putchar(' ');
    printf("    Start                End              \033[0m\n");

    for (int i = 0; i < records.size(); ++i)
    {
        EventRecord& rec = records[i];
        printf("%2d    %8s    %s", i+1, rec.course.c_str(), rec.name.c_str());
        for (int j = 0; j < l_name - rec.name.size(); ++j) putchar(' ');
        printf("    %s", rec.location.c_str());
        for (int j = 0; j < l_loc - rec.location.size(); ++j) putchar(' ');

        int h = HOUR(rec.start_time), m = MINUTE(rec.start_time);

        printf("    %04d-%02d-%02d  %02d:%02d    ",
            YEAR(rec.start_date), MONTH(rec.start_date), DAY(rec.start_date), h, m);

        h += HOUR(rec.duration), m += MINUTE(rec.duration);
        h += m / 60;
        m %= 60;

        if (h < 24) {
            printf("→         ");
        } else {
            int d = h / 24;
            h %= 24;
            d += GetDaysSince1970(rec.start_date);
            Date nd = GetDateFromDaysSince1970(d);
            printf("%04d-%02d-%02d", YEAR(nd), MONTH(nd), DAY(nd));
        }

        printf("  %02d:%02d\n", h, m);

    }

    putchar('\n');

    return 0;
}

int ListTodos_Main()
{
    if (!CreateFilesIfNotExists()) return 1;
    auto records = ReadTodoRecords();

    int longest_name = 0;
    for (auto& rec : records)
        longest_name = std::max(longest_name, static_cast<int>(rec.name.size()));

    std::cout << "╭────┬──────────┬─────";
    for (int i = 0; i < longest_name - 4; ++i) std::cout << "─";
    std::cout << "─┬────────────╮\n";
    std::cout << "│ ID │ Course   │ Name";
    for (int i = 0; i < longest_name - 4; ++i) std::cout << " ";
    std::cout << " │ Due        │\n";
    std::cout << "├────┼──────────┼─────";
    for (int i = 0; i < longest_name - 4; ++i) std::cout << "─";
    std::cout << "─┼────────────┤\n";

    for (int i = 0; i < records.size(); ++i)
    {
        auto& rec = records[i];
        std::cout << "│ " << (i < 9 ? " " : "") << i + 1;
        std::cout << " │ " << rec.course << " │ " << rec.name;
        for (int j = 0; j < std::max(longest_name, 4) - rec.name.size(); ++j) std::cout << " ";
        printf(" │ %d-%02d-%02d │\n", YEAR(rec.due), MONTH(rec.due), DAY(rec.due));
    }

    std::cout << "╰────┴──────────┴─────";
    for (int i = 0; i < longest_name - 4; ++i) std::cout << "─";
    std::cout << "─┴────────────╯\n";
    return 0;
}
