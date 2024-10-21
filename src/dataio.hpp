#pragma once

#include <string>
#include "timedate.hpp"

struct TodoRecord
{
    std::string course;
    std::string name;
    Date due;
};

// Main function for add
int AddMain(int opt); // 0 = N/A, 1 = event, 2 = todo

// Main function for rm
int RemoveMain(int id); // 0 = N/A

// Main function for done
int DoneMain(int id); // 0 = N/A