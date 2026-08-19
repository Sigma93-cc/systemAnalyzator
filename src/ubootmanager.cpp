#include "ubootmanager.h"
#include <cstddef>
#include <iostream>
#include <cstring>

Ubootmanager::Ubootmanager()
{    
    if (libuboot_read_config_ext(&ctx, "/etc/fw_env.config") < 0) {
        std::cerr << "Error reading U-Boot config" << std::endl;
        ctx = nullptr;
        return;
    }
}

Ubootmanager::~Ubootmanager()
{
    if (!ctx)
        return;

    libuboot_exit(ctx);
}

Ubootmanager& Ubootmanager::instance()
{
    static Ubootmanager mng;
    return mng;
}

bool Ubootmanager::_open()
{
    if (!ctx)
        return false;

    if (!libuboot_open(ctx))
        return true;
    return false;
}

void Ubootmanager::_close()
{
    if (!ctx)
        return;

    libuboot_close(ctx);    
}

bool Ubootmanager::mark_good()
{
    if (auto res = _open(); res) {
        libuboot_set_env(ctx,"upgrade_available", "0");
        libuboot_env_store(ctx);
        _close();
        return true;
    }
    return false;
}

Ubootmanager::uboot_info Ubootmanager::info()
{
    uboot_info out{0, false};

    if(auto res = _open(); res) {
        if (auto up_available = libuboot_get_env(ctx, "upgrade_available");up_available) {
            out.second = std::stoi(up_available);
            free(up_available);
        }
        if (auto bootpart = libuboot_get_env(ctx, "bootpart");bootpart) {
            out.first = std::stoi(bootpart);
            free(bootpart);
        }
        _close();
    }
    return out;
}
