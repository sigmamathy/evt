#include "list.hpp"
#include "timedate.hpp"
#include "dataio.hpp"
#include <iostream>

int ListMain()
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
