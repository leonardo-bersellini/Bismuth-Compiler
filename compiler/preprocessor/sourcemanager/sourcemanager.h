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

    void addIncludeDirectory(const std::string& path);

    std::optional<std::string> getFileContent(const std::filesystem::path& path);

    std::optional<std::string> resolvePath(const std::string& path, const std::string& includingFile);

private:
    std::vector<std::string> m_includeDirs;
};


#endif //SOURCE_MANAGER_H