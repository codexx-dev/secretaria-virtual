#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

// // ----------------------------------------------------------------------------
// // Utilidades
// // ----------------------------------------------------------------------------

bool write_byte(const fs::path& path, std::string_view& data);
bool write_line(const fs::path& path, std::string_view& content, int line = -1);

bool erase(const fs::path& path);

bool read_bytes(const fs::path& path, std::vector<char>& out);
bool read_line(const fs::path& path, std::string& out, int line);