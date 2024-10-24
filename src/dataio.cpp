#include "dataio.hpp"
#include <iostream>
#include <array>
#include <filesystem>
#include <fstream>
#include <pwd.h>
#include <unistd.h>

static std::string GetUserDataDir()
{
    passwd *pw = getpwuid(getuid());
    return pw->pw_dir + std::string("/.config/evt");
}

bool CreateFilesIfNotExists()
{
    namespace fs = std::filesystem;
    std::string dir = GetUserDataDir();

    if (!fs::exists(dir)) {
        std::cout << "$HOME/.config/evt directory not found. Creating one...\n";
        if (!fs::create_directory(dir)) {
            std::cerr << "ERROR: unable to create directory.\n";
            return false;
        }
    }

    static std::array files = { "/event.txt", "/todo.txt" };

    for (auto* f : files) {
        if (std::string file = dir + f; !fs::exists(file)) {
            std::cout << "$HOME/.config/evt" << f << " file not found. Creating one...\n";
            std::ofstream ofs(file);
            if (!ofs.is_open()) {
                std::cerr << "ERROR: unable to create file.\n";
                return false;
            }
            ofs.close();
        }
    }

    return true;
}

std::vector<EventRecord> ReadEventRecords()
{
    std::ifstream ifs(GetUserDataDir() + "/event.txt");
    std::vector<EventRecord> records;
    std::string line;

    while (getline(ifs, line) && !line.empty())
    {
        EventRecord& rec = records.emplace_back();
        rec.course = line == "/" ? "" : line;
        getline(ifs, line);
        rec.name = line;
        getline(ifs, line);
        rec.location = line;
        getline(ifs, line);
        char* ptr;
        rec.start_date = strtol(line.c_str(), &ptr, 10);
        rec.start_time = strtol(ptr, &ptr, 10);
        rec.duration = strtol(ptr, &ptr, 10);
    }

    ifs.close();
    return records;
}

void WriteEventRecords(std::vector<EventRecord> const& records)
{
    std::ofstream ofs(GetUserDataDir() + "/event.txt");

    for (const auto & rec : records) {
        ofs << (rec.course.empty() ? "/" : rec.course) << '\n' << rec.name << '\n' << rec.location << '\n';
        ofs << rec.start_date << ' ' << rec.start_time << ' ' << rec.duration << '\n';
    }

    ofs.close();
}

std::vector<TodoRecord> ReadTodoRecords()
{
    std::ifstream ifs(GetUserDataDir() + "/todo.txt");
    std::vector<TodoRecord> records;
    std::string line;

    while (getline(ifs, line) && !line.empty())
    {
        TodoRecord& rec = records.emplace_back();
        rec.course = line;
        getline(ifs, line);
        rec.name = line;
        getline(ifs, line);
        rec.due = std::stoi(line);
    }

    ifs.close();
    return records;
}

void WriteTodoRecords(std::vector<TodoRecord> const& records)
{
    std::ofstream ofs(GetUserDataDir() + "/todo.txt");

    for (const auto & rec : records) {
        ofs << rec.course << '\n' << rec.name << '\n' << rec.due << '\n';
    }

    ofs.close();
}

int AddEvent_Main()
{
    if (!CreateFilesIfNotExists()) return 1;
    auto records = ReadEventRecords();

    EventRecord& rec = records.emplace_back();
    printf("\n:: Enter course code    -> ");
    getline(std::cin, rec.course);
    if (!rec.course.empty() && rec.course.length() != 8) {
        std::cerr << "ERROR: Course code must be either 8 characters or empty.\n\n";
        return 1;
    }

    std::cout << ":: Enter event name     -> ";
    getline(std::cin, rec.name);
    std::cout << ":: Enter location       -> ";
    getline(std::cin, rec.location);
    std::string s;
    std::cout << ":: Enter start datetime -> ";
    std::cin >> s;
    int year, month, day, hour, minute;
    if (sscanf(s.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        std::cerr << "ERROR: Date format must be in YYYY-MM-DD format.\n\n";
        return 1;
    }

    rec.start_date = TO_DATE(year, month, day);
    std::cin >> s;
    if (sscanf(s.c_str(), "%d:%d", &hour, &minute) != 2) {
        std::cerr << "ERROR: Time format must be in hh:mm (24-hours) format.\n\n";
        return 1;
    }

    rec.start_time = TO_TIME(hour, minute, 0);
    std::cout << ":: Enter duration       -> ";
    std::cin >> s;
    if (sscanf(s.c_str(), "%d:%d", &hour, &minute) != 2) {
        std::cerr << "ERROR: Time format must be in hh:mm (24-hours) format.\n\n";
        return 1;
    }

    rec.duration = TO_TIME(hour, minute, 0);

    WriteEventRecords(records);
    std::cout << "\033[33mCreated event with ID = " << records.size() << ".\033[0m\n\n";

    return 0;
}

int RmEvent_Main(int id)
{
    std::cout << '\n';

    if (!id) {
        std::cout << ":: Enter ID -> ";
        std::cin >> id;
    }

    if (!CreateFilesIfNotExists()) return 1;
    auto records = ReadEventRecords();
    if (id < 1 || id > records.size()) {
        std::cerr << "ERROR: Not a valid ID.\n\n";
        return 1;
    }

    std::string str = records[id-1].course + " " + records[id-1].name;
    records.erase(records.begin() + id-1);

    WriteEventRecords(records);
    std::cout << "\033[33mEnded event " << id << ": " << str << "\033[0m\n\n";

    return 0;
}

int AddTodo_Main()
{
    if (!CreateFilesIfNotExists()) return 1;
    auto records = ReadTodoRecords();

    TodoRecord& rec = records.emplace_back();
    printf("\n:: Enter course code -> ");
    std::cin >> rec.course;
    if (rec.course.length() != 8) {
        std::cerr << "ERROR: Length of course code must be 8.\n\n";
        return 1;
    }
    std::cin.ignore();
    std::cout << ":: Enter todo name   -> ";
    getline(std::cin, rec.name);
    std::cout << ":: Enter due date    -> ";
    std::string date;
    std::cin >> date;
    int year, month, day;

    if (sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        std::cerr << "ERROR: Date format must be in YYYY-MM-DD format.\n\n";
        return 1;
    }

    Date today = GetTodayDate();
    rec.due = TO_DATE(year, month, day);
    if (!IsDateValid(rec.due) || rec.due < today) {
        std::cerr << "ERROR: Date is out of range.\n\n";
        return 1;
    }

    WriteTodoRecords(records);
    std::cout << "\033[33mCreated task with ID = " << records.size() << ".\033[0m\n\n";
    return 0;
}

int RmTodo_Main(int id)
{
    std::cout << '\n';

    if (!id) {
        std::cout << ":: Enter ID -> ";
        std::cin >> id;
    }

    if (!CreateFilesIfNotExists()) return 1;
    auto records = ReadTodoRecords();
    if (id < 1 || id > records.size()) {
        std::cerr << "ERROR: Not a valid ID.\n\n";
        return 1;
    }

    std::string str = records[id-1].course + " " + records[id-1].name;
    records.erase(records.begin() + id-1);

    WriteTodoRecords(records);
    std::cout << "\033[33mCompleted task " << id << ": " << str << "\033[0m\n\n";

    return 0;
}