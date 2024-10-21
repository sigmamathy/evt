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

int AddMain(int opt)
{
    if (!opt) {
        printf("\033[31mEnter type [1: event, 2: todo]\033[0m >> ");
        std::cin >> opt;
        if (opt != 1 && opt != 2) {
            std::cerr << "ERROR: Not a valid option.\n";
            return 1;
        }
    }

    if (opt == 2)
    {
        if (!CreateFilesIfNotExists()) return 1;
        auto records = ReadTodoRecords();

        TodoRecord& rec = records.emplace_back();
        printf("\033[31mEnter course code\033[0m >> ");
        std::cin >> rec.course;
        if (rec.course.length() != 8) {
            std::cerr << "ERROR: Length of course code must be 8.\n";
            return 1;
        }
        std::cin.ignore();
        std::cout << "\033[31mEnter todo name\033[0m   >> ";
        getline(std::cin, rec.name);
        std::cout << "\033[31mEnter due date\033[0m    >> ";
        std::string date;
        std::cin >> date;
        int year, month, day;

        if (sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
            std::cout << "ERROR: Date format must be in YYYY-MM-DD format.\n";
            return 1;
        }

        Date today = GetTodayDate();
        rec.due = TO_DATE(year, month, day);
        if (!IsDateValid(rec.due) || rec.due < today) {
            std::cerr << "ERROR: Date is out of range.\n";
            return 1;
        }

        WriteTodoRecords(records);
        std::cout << "\033[33mCreated task with ID = " << records.size() << ".\033[0m\n";
    }

    return 0;
}

int RemoveMain(int id)
{
    // ...
    return 0;
}

int DoneMain(int id)
{
    if (!id) {
        std::cout << "\033[31mEnter ID\033[0m >> ";
        std::cin >> id;
    }

    if (!CreateFilesIfNotExists()) return 1;
    auto records = ReadTodoRecords();
    if (id < 1 || id > records.size()) {
        std::cerr << "ERROR: Not a valid ID.\n";
        return 1;
    }

    std::string str = records[id-1].course + " " + records[id-1].name;
    records.erase(records.begin() + id-1);

    WriteTodoRecords(records);
    std::cout << "\033[33mCompleted task " << id << ": " << str << "\033[0m\n";

    return 0;
}