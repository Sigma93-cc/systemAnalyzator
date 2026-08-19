#ifndef UBOOTMANAGER_H
#define UBOOTMANAGER_H

#include <cstdint>
#include <cstddef>
#include <libuboot.h>
#include <utility>

class Ubootmanager
{
public:
    using uboot_info = std::pair<std::uint32_t, bool>;
    static Ubootmanager& instance();
    bool mark_good();
    uboot_info info();
private:
    Ubootmanager();
    ~Ubootmanager();
    bool _open();
    void _close();
    struct uboot_ctx* ctx = nullptr;
};

#endif // UBOOTMANAGER_H
