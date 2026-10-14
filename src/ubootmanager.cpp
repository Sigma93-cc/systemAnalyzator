#include "ubootmanager.h"
#include <iostream>
#include <cstring>

struct UbootEnvManagerPriv
{
    UbootEnvManagerPriv();
    ~UbootEnvManagerPriv();
    bool _open();
    void _close();
    struct uboot_ctx* ctx = nullptr;
};

UbootEnvManagerPriv::UbootEnvManagerPriv()
{
    if (libuboot_read_config_ext(&ctx, "/etc/fw_env.config") < 0) {
        std::cerr << "Error reading U-Boot config" << std::endl;
        ctx = nullptr;
        return;
    }
    std::cout << "U-Boot config readed sucessfully" << std::endl;
}

UbootEnvManagerPriv::~UbootEnvManagerPriv()
{
    if (!ctx)
        return;

    libuboot_exit(ctx);
}

UbootEnvManager::UbootEnvManager() : d (new UbootEnvManagerPriv()) {}
UbootEnvManager::~UbootEnvManager() {}

bool UbootEnvManagerPriv::_open()
{
    if (!ctx)
        return false;

    if (!libuboot_open(ctx))
        return true;

    return libuboot_load_file(ctx, "/etc/u-boot-initial-env") == 0;
}

void UbootEnvManagerPriv::_close()
{
    if (!ctx)
        return;

    libuboot_close(ctx);    
}

std::string UbootEnvManager::get(std::string_view attr)
{
    std::string out {};
    if (auto res = d->_open(); res) {
        if (auto val = libuboot_get_env(d->ctx, attr.data()); val) {
            out.assign(val);
            free(val);
        }
    }
    d->_close();
    return out;
}

bool UbootEnvManager::set(std::string_view attr, std::string_view val)
{
    auto ok = false;
    if (auto res = d->_open(); res) {
        ok = libuboot_set_env(d->ctx, attr.data(), val.data()) == 0;
        if (ok) {
            ok = libuboot_env_store(d->ctx) == 0;
        }
    }
    d->_close();
    return ok;
}
