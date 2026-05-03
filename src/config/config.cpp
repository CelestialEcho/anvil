#include "config.h"
#include "../../include/nlohmann/json.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

std::string Config::config_path() const
{
    return project_name + ".json";
}

Config::Config(const std::string& name) : project_name(name)
{
    std::string path = config_path();
    std::ifstream f(path);
    if (f.good())
    {
        load(path);
    }
    else
    {
        update();
    }
}

void Config::load(const std::string& path)
{
    std::ifstream f(path);
    if (!f.is_open())
    {
        throw std::runtime_error("Cannot open config: " + path);
    }

    json data = json::parse(f);

    includes.clear();
    for (auto& item : data["includes"])
    {
        includes.push_back(item.get<std::string>());
    }

    flags.clear();
    for (auto& entry : data["flags"])
    {
        auto key = entry["flag"].get<std::string>();
        auto& val = entry["value"];
        if (val.is_null())
        {
            flags.emplace_back(key, std::nullopt);
        }
        else
        {
            flags.emplace_back(key, val.get<std::string>());
        }
    }
}

void Config::update() const
{
    json j;
    j["BUILDER_INFO"] = "Anvil CLI C/C++ builder, check out more on github.";
    j["ANVIL_VERSION"] = InternalConfig::CURRENT_VERSION;
    j["PROJECT_NAME"] = project_name;
    j["includes"] = includes;

    j["flags"] = json::array();
    for (auto& [key, val] : flags)
    {
        json entry;
        entry["flag"] = key;
        entry["value"] = val.has_value() ? json(*val) : json(nullptr);
        j["flags"].push_back(entry);
    }

    std::ofstream f(config_path());
    if (!f.is_open())
    {
        throw std::runtime_error("Cannot write config: " + config_path());
    }

    f << j.dump(4) << "\n";
}
