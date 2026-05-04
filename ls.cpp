#include <iostream>
#include <print>
#include <span>
#include <filesystem>

namespace fs = std::filesystem;
static constexpr std::string_view dot { "." };

struct PrintOptions {
    fs::path path_to_print { fs::current_path() };
    bool user_path { false };
    bool show_hidden { false };
};

PrintOptions create_print_options(std::span<char*> arguments) {
    PrintOptions print_options;
    size_t i { 1 };
    while(i < arguments.size()) {
        std::string argument { arguments[i++] };
        if(argument == "-a")
            print_options.show_hidden = true;
        else if(!fs::exists(argument)) {
            std::cerr << "not a valid path!\n";
            continue;
        } else {
            std::filesystem::path user_path { argument };
            print_options.path_to_print = user_path;
        }
    }
    return print_options;
}

class Printer {
    private:
        PrintOptions m_options;
    public:
        Printer(PrintOptions print_options)
            : m_options(print_options)
        {}

        void print() {
            std::println("print function is called");
            for(const fs::directory_entry& entry : fs::directory_iterator(m_options.path_to_print)) {
                if(entry.is_directory()) {
                    if(!m_options.show_hidden && entry.path().filename().string().starts_with(dot))
                        continue;
                    std::println("Directory: {}", entry.path().filename().string());
                } else {
                    if(!m_options.show_hidden && entry.path().filename().string().starts_with(dot))
                        continue;
                    std::println("File: {}", entry.path().filename().string());
                }
            }
        }
};

int main(int argc, char** argv) {
    PrintOptions print_options { create_print_options(std::span<char*>(argv, argc)) };
    std::println("directory: {}", print_options.path_to_print.string());
    Printer printer { print_options };
    printer.print();
    return 0;
}
