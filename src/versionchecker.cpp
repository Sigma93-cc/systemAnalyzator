#include "versionchecker.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <version.h>

Versionchecker::systemInfo Versionchecker::getInfo() {
    std::cout << projectversion;
    return {readOSRelease(), std::string(projectversion)};
}

std::string Versionchecker::readOSRelease() {

    const auto filename = "/etc/os-release";
    std::stringstream out;
    if (!boost::filesystem::exists(filename)) {
         std::cout << "Error, file not exists" << std::endl;
         return {};
    }

    std::list<std::string> keys {"NAME=", "VERSION_ID="};
    auto it = keys.begin();
    if (auto file = std::fstream(filename, std::ios::in); file.is_open()) {
        for (std::string line; std::getline(file, line);) {
            if (it == keys.end())
                break;
            if (auto [res, value] = parseLine(line, *it); res) {
                out << value << " ";
                ++it;
            }
        }
    } else {
      std::cout << "Can't open file" << filename << std::endl;
      return {};
    }
    return out.str();
}

std::pair<bool, std::string> Versionchecker::parseLine(std::string_view line, std::string_view attr) {
    if (auto res = line.find(attr); res != line.npos && res == 0) {
        return {true, std::string(line.substr(attr.length()+1,line.length()-attr.length()-2))};
    }
    return {};
}
