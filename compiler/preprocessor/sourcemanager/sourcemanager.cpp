#include "sourcemanager.h"

#include <filesystem>
#include <vector>
#include <string>
#include <optional>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;


std::optional<std::string> SourceManager::getFileContent(const fs::path& path)
{
    std::ifstream in(path);

    if(!in) return std::nullopt;

    std::stringstream buffer;
    buffer << in.rdbuf();

    return buffer.str();
}

std::optional<std::string> SourceManager::resolvePath(const std::string& path)
{
    if(std::filesystem::exists(path)) {
        return std::filesystem::canonical(path).string();
    } else {
        return std::nullopt;
    }
}