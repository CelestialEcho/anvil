#pragma once

#include <string>
#include <vector>
#include "config/config.h"

class Anvil
{
public:
    Anvil();

    void version() const;
    void init() const;
    void include_file(const std::string& path);
    void exclude_file(const std::string& item);
    void list_files() const;
    void clear();
    void add_arg(const std::string& flag, const std::string& value = "");
    void remove_arg(const std::string& flag);
    void list_args() const;
    int build() const;
    int start() const;

private:
    Config config;
    std::vector<std::string> recursive_find(const std::string& path) const;
};
