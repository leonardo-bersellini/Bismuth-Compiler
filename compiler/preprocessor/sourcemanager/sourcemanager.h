#ifndef SOURCE_MANAGER_H
#define SOURCE_MANAGER_H

#include <filesystem>
#include <optional>
#include <vector>
#include <string>

class SourceManager
{
public:
    SourceManager() = default;
    ~SourceManager() = default;

    std::optional<std::string> getFileContent(const std::filesystem::path& path);

    std::optional<std::string> resolvePath(const std::string& path);

private:
};


#endif //SOURCE_MANAGER_H