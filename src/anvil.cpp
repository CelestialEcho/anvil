#include "anvil.h"
#include "config/config.h"

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

static std::string cwd_name()
{
    return fs::current_path().filename().string();
}

Anvil::Anvil() : config(cwd_name()) {}

std::vector<std::string> Anvil::recursive_find(const std::string& path) const
{
    std::vector<std::string> result;
    fs::path base = fs::path(path);

    for (auto& entry : fs::recursive_directory_iterator(path))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }
        std::string ext = entry.path().extension().string();
        if (!ext.empty())
        {
            ext = ext.substr(1);
        }
        for (auto& allowed : InternalConfig::INCLUDE_FILE_EXTENSIONS)
        {
            if (ext == allowed)
            {
                result.push_back(fs::relative(entry.path(), base).string());
                break;
            }
        }
    }
    return result;
}

void Anvil::version() const
{
    std::cout << "Anvil " << InternalConfig::CURRENT_VERSION << std::endl;
}

void Anvil::init() const
{
    this->config.update();
}

void Anvil::include_file(const std::string& path)
{
    if (path == ".")
    {
        auto files = recursive_find(".");
        this->config.includes.insert(this->config.includes.end(), files.begin(), files.end());
    }
    else if (fs::is_directory(path))
    {
        auto files = recursive_find(path);
        this->config.includes.insert(this->config.includes.end(), files.begin(), files.end());
    }
    else if (fs::is_regular_file(path))
    {
        this->config.includes.push_back(fs::relative(path).string());
    }
    else
    {
        std::cerr << "Warning: '" << path << "' is not a valid file or directory\n";
        return;
    }
    config.update();
}

void Anvil::exclude_file(const std::string& item)
{
    std::string normalized = fs::path(item).lexically_normal().string();

    auto it = std::find_if(this->config.includes.begin(), this->config.includes.end(), [&](const std::string& f)
    {
        return fs::path(f).lexically_normal().string() == normalized;
    });

    if (it == this->config.includes.end())
    {
        std::cerr << "Warning: '" << item << "' not in includes\n";
        return;
    }

    this->config.includes.erase(it);
    this->config.update();
}

void Anvil::list_files() const
{
    std::cout << "Included files: " << std::endl;
    for (const std::string& file : this->config.includes)
    {
        std::cout << file << std::endl;
    }
    return;
}

void Anvil::clear()
{
    this->config.includes.clear();
    this->config.update();
}

void Anvil::add_arg(const std::string& flag, const std::string& value)
{
    auto it = std::find_if(this->config.flags.begin(), this->config.flags.end(), [&](const auto& pair) { return pair.first == flag; });

    if (it != this->config.flags.end())
    {
        std::cerr << "Warning: '" << flag << "' already exists, overwriting\n";
        it->second = value.empty() ? std::nullopt : std::optional<std::string>(value);
    }
    else
    {
        this->config.flags.emplace_back(flag,
            value.empty() ? std::nullopt : std::optional<std::string>(value));
    }
    this->config.update();
}

void Anvil::remove_arg(const std::string& flag)
{
    auto it = std::find_if(this->config.flags.begin(), this->config.flags.end(), [&](const auto& pair) { return pair.first == flag; });

    if (it == this->config.flags.end())
    {
        std::cerr << "Warning: '" << flag << "' not in flags\n";
        return;
    }
    this->config.flags.erase(it);
    this->config.update();
}

void Anvil::list_args() const
{
    std::cout << "Currently applied flags: " << std::endl;
    for (const auto& [key, value] : this->config.flags)
    {
        if (value.has_value())
        {
            std::cout << key << " " << *value << std::endl;
        }
        else
        {
            std::cout << key << std::endl;
        }
    }
}

int Anvil::build() const
{
    if (this->config.includes.empty())
    {
        std::cerr << "Error: no source files included\n";
        return 1;
    }

    std::ostringstream include_flags, compile_flags, link_dirs, libraries, output;

    for (auto& [flag, val] : this->config.flags)
    {
        std::ostringstream part;
        part << flag;
        if (val.has_value() && !val->empty())
        {
            part << " " << *val;
        }

        if (flag.rfind("-I", 0) == 0)
        {
            include_flags << " " << part.str();
        }
        else if (flag.rfind("-L", 0) == 0)
        {
            link_dirs << " " << part.str();
        }
        else if (flag.rfind("-l", 0) == 0)
        {
            libraries << " " << part.str();
        }
        else if (flag == "-o")
        {
            output << " " << part.str();
        }
        else
        {
            compile_flags << " " << part.str();
        }
    }

    std::ostringstream cmd;
    cmd << InternalConfig::COMPILER;

    for (auto& src : this->config.includes)
    {
        cmd << " " << src;
    }

    cmd << include_flags.str() << compile_flags.str() << link_dirs.str() << libraries.str() << output.str();

    std::string command = cmd.str();
    std::cout << "Building command preview: \n" << command << "\n";
    return std::system(command.c_str());
}

int Anvil::start() const
{
    std::string exe = this->config.project_name + ".exe";

    auto it = std::find_if(this->config.flags.begin(), this->config.flags.end(), [](const auto& pair) { return pair.first == "-o"; });

    if (it != this->config.flags.end() && it->second.has_value())
    {
        exe = *it->second;
    }

    if (!fs::exists(exe))
    {
        std::cerr << "Error: executable '" << exe << "' not found. Run 'anvil build' first.\n";
        return 1;
    }

    std::string command = "./" + exe;
    std::cout << "Running: " << command << "\n";
    return std::system(command.c_str());
}
