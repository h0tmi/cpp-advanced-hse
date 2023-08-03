#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

class FileManager {
public:
    FileManager() = default;

    static std::string read(const std::string& filename);
};
