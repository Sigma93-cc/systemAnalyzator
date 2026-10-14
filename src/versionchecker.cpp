#include "versionchecker.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <version.h>

Versionchecker::systemInfo Versionchecker::getInfo() {
    return {readOSRelease(), std::string(projectversion)};
}

std::string Versionchecker::readOSRelease() {

    const auto filename = "/etc/os-release";
    std::stringstream out;
    if (!boost::filesystem::exists(filename)) {
         std::cout << "Error, file not exists" << std::endl;
         return {};
    }

    std::string key="PRETTY_NAME=";
    if (auto file = std::fstream(filename, std::ios::in); file.is_open()) {
        for (std::string line; std::getline(file, line);) {
            if (auto [res, value] = parseLine(line, key); res) {
                return value;
            }
        }
    } else {
      std::cout << "Can't open file" << filename << std::endl;
      return {};
    }
    return {};
}

std::pair<bool, std::string> Versionchecker::parseLine(std::string_view line, std::string_view attr) {
    if (auto res = line.find(attr); res != line.npos && res == 0) {
        return {true, std::string(line.substr(attr.length(),line.length()-attr.length()))};
    }
    return {};
}
