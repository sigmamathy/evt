#include <iostream>
#include <vector>
#include <string>

#include "calendar.hpp"
#include "timedate.hpp"

int main(int argc, char** argv)
{
    if (argc == 1) {
        CalendarMain();
        return 0;
    }

    std::vector<std::string> args(argc - 1);
    for (int i = 1; i < argc; ++i)
        args[i - 1] = argv[i];

    if (argv[0] == "list") {

    }


    return 0;
}