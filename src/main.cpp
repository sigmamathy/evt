#include <iostream>
#include <vector>
#include <string>
#include "timedate.hpp"

int main(int argc, char** argv)
{
    std::vector<std::string> args(argc - 1);
    for (int i = 1; i < argc; ++i)
        args[i - 1] = argv[i];



    Date date = GetTodayDate();
    Date date2 = TO_DATE(2024, 10, 25);

    std::cout << GetDaysSince1970(date2) - GetDaysSince1970(date) << '\n';


    return 0;
}