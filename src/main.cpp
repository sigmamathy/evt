#include <iostream>
#include <vector>
#include <string>

#include "calendar.hpp"
#include "dataio.hpp"

int main(int argc, char** argv)
{
    if (argc == 1) {
        CalendarMain();
        return 0;
    }

    std::vector<std::string> args(argc - 1);
    for (int i = 1; i < argc; ++i)
        args[i - 1] = argv[i];

    if (args[0] == "add") {
        int opt = 0;
        if (args.size() > 1) {
            if (args[1] == "event") opt = 1;
            else if (args[1] == "todo") opt = 2;
            else {
                std::cerr << "ERROR: Not a valid option.\n";
                return 1;
            }
        }
        return AddMain(opt);
    }

    if (args[0] == "done") {
        int opt = 0;
        if (args.size() > 1)
            opt = std::stoi(args[1]);
        return DoneMain(opt);
    }


    return 0;
}