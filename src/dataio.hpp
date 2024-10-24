#pragma once

#include <string>
#include <vector>
#include "timedate.hpp"

struct EventRecord
{
    std::string course;
    std::string name, location;
    Date start_date;
    Time start_time, duration;
};

struct TodoRecord
{
    std::string course;
    std::string name;
    Date due;
};

bool CreateFilesIfNotExists();

std::vector<EventRecord> ReadEventRecords();

std::vector<TodoRecord> ReadTodoRecords();

// Main function for add event
int AddEvent_Main();

// Main function for remove event
int RmEvent_Main(int id); // 0 = N/A

// Main function for add todo
int AddTodo_Main();

// Main function for done
int RmTodo_Main(int id); // 0 = N/A