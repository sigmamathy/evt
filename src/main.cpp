#include <iostream>
#include <vector>
#include <string>

#include "calendar.hpp"
#include "dataio.hpp"
#include "list.hpp"

int main(int argc, char** argv)
{
    if (argc == 1) {
        CalendarMain();
        return 0;
    }

    std::vector<std::string> args(argc - 1);
    for (int i = 1; i < argc; ++i) args[i - 1] = argv[i];

    if (args[0] == "e") {
        return ListEvents_Main();
    }

    if (args[0] == "t") {
        return ListTodos_Main();
    }

    if (args[0] == "+e") {
        return AddEvent_Main();
    }

    if (args[0] == "-e") {
        int opt = 0;
        if (args.size() > 1)
            opt = std::stoi(args[1]);
        return RmEvent_Main(opt);
    }

    if (args[0] == "+t") {
        return AddTodo_Main();
    }

    if (args[0] == "-t") {
        int opt = 0;
        if (args.size() > 1)
            opt = std::stoi(args[1]);
        return RmTodo_Main(opt);
    }

    return 0;
}
