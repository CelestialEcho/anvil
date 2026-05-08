#include <iostream>
#include <string>
#include "src/anvil.h"

static void print_usage()
{
    std::cerr <<
        "Usage: anvil <command> [args]\n"
        "\n"
        "Commands:\n"
        "  version                  Prints the current version of anvil\n"
        "  init                     Initializes the anvil project\n"
        "  include <path>           Add file or directory to build\n"
        "  exclude <path>           Remove file from build\n"
        "  list-files               List included files\n"
        "  clear                    Clear the list of included files\n"
        "  add-arg <arg>            Add compiler argument\n"
        "  add-arg <arg> [value]    Add compiler argument with value\n"
        "  remove-arg <flag>        Remove compiler flag\n"
        "  list-args                List currently applied flags\n"
        "  build                    Compile the project\n"
        "  start                    Run the compiled executable\n";
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        print_usage();
        return 1;
    }

    std::string command = argv[1];
    Anvil anvil;
    if (command == "version")
    {
        anvil.version();
    }
    else if (command == "init")
    {
        anvil.init();
    }
    else if (command == "include")
    {
        if (argc < 3) { std::cerr << "Error: 'include' requires a path\n"; return 1; }
        anvil.include_file(argv[2]);
    }
    else if (command == "exclude")
    {
        if (argc < 3) { std::cerr << "Error: 'exclude' requires a path\n"; return 1; }
        anvil.exclude_file(argv[2]);
    }
    else if (command == "list-files")
    {
        anvil.list_files();
    }
    else if (command == "clear")
    {
        anvil.clear();
    }
    else if (command == "add-arg")
    {
        if (argc < 3) { std::cerr << "Error: 'add-flag' requires a arg name\n"; return 1; }
        std::string value = (argc >= 4) ? argv[3] : "";
        anvil.add_arg(argv[2], value);
    }
    else if (command == "remove-arg")
    {
        if (argc < 3) { std::cerr << "Error: 'remove-flag' requires a arg name\n"; return 1; }
        anvil.remove_arg(argv[2]);
    }
    else if (command == "list-args")
    {
        anvil.list_args();
    }
    else if (command == "build")
    {
        return anvil.build();
    }
    // TODO: fix
    // else if (command == "start")
    // {
    //     return anvil.start();
    // }
    // else if (command == "list-flags")
    // {
    //     return anvil.start();
    // }
    else
    {
        std::cerr << "Error: unknown command '" << command << "'\n\n";
        print_usage();
        return 1;
    }

    return 0;
}
