#pragma once

#include <libuboot.h>
#include <string_view>
#include <memory>

struct UbootEnvManagerPriv;
class UbootEnvManager
{
public:
    UbootEnvManager();
    ~UbootEnvManager();
    std::string get(std::string_view attr);
    bool set(std::string_view attr, std::string_view val);
private:
    std::unique_ptr<UbootEnvManagerPriv> d;
};


