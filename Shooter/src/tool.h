#ifndef TOOL_H
#define TOOL_H

#include <filesystem>
#include <string>
// Convert relative path to absolute path
inline auto getPath(const char* path) -> std::string
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    return (currentPath / path).string();
}

#endif