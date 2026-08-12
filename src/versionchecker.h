#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <string>

class Versionchecker
{
public:
    using systemInfo = std::pair<std::string,std::string>;
    static systemInfo getInfo();
private:
    static std::string readOSRelease();
    static std::pair<bool, std::string> parseLine(std::string_view line, std::string_view attr);
};

#endif // VERSIONCHECKER_H
