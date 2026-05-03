#pragma once

#include <string>
#include <vector>
#include <optional>


namespace InternalConfig
{
    inline constexpr const char* CURRENT_VERSION = "1.0.0";
    inline const std::vector<std::string> INCLUDE_FILE_EXTENSIONS = {"cpp", "c", "cc", "cxx", "cp"};
    inline constexpr const char* COMPILER = "g++";
}

class Config
{
public:
    std::string project_name;
    std::vector<std::string> includes;
    std::vector<std::pair<std::string, std::optional<std::string>>> flags;

    explicit Config(const std::string& project_name);
    void update() const;

private:
    std::string config_path() const;
    void load(const std::string& path);
};
