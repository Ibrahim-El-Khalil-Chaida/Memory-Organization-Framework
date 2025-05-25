#include "memory_organization.h"

int main(void) {
    bool force_boot = false;
    
    // e.g., read pin or flag
    if (force_boot) {
        Bootloader_Run();
    }

    uint32_t app_addr = BANK0_START_ADDR + 0x8000;
    __set_MSP(*(uint32_t *)app_addr);
    ((void(*)(void))(*(uint32_t *)(app_addr + 4)))();

    while (1);
}
