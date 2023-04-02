#include <cstdio>
#include <filesystem>
#include <regex>
#include <vector>

#include <fmt/core.h>

using namespace std;
namespace fs = std::filesystem;

string exec(const string &cmd) {
    shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        return "ERROR";
    }
    char buffer[128];
    std::string result;
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL) {
            result += buffer;
        }
    }
    return result;
}

inline string exec_clang_tidy(const string &clang_format_bin,
                              const string &compile_commands_dir,
                              const string &file_name) {
    return exec(clang_format_bin + +" --format-style=file -p " + compile_commands_dir + " " + file_name);
}

void collect_files(const string &dir, vector<string> &files) {
    regex pattern(R"(.*\.cpp)");
    for (const auto &entry : fs::recursive_directory_iterator(dir)) {
        string file_name = entry.path().string();
        if (regex_match(file_name.c_str(), pattern)) {
            fmt::print("checking {}\n", file_name);
            files.emplace_back(std::move(file_name));
        }
    }
}

int main(int argc, char **argv) {
    string clang_format_bin = argv[1];
    string compile_commands_dir = argv[2];

    fs::path current_path = fs::current_path();
    fs::path src_dir = current_path / "../src";
    fs::path test_src_dir = current_path / "../test";

    vector<string> files;
    collect_files(src_dir, files);
    collect_files(test_src_dir, files);

    vector<string> results;
    for (const auto &file_name : files) {
        results.emplace_back(exec_clang_tidy(clang_format_bin, compile_commands_dir, file_name));
    }
    for (const auto &result : results) {
        fmt::print("{}\n", result);
    }
}
