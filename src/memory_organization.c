#include "memory_organization.h"
#include <string.h>
#include <stm32f4xx.h> // Adjust to your MCU header

bool Memory_Init(void) {
#if (DUAL_BANK_MODE)
    // Example: enable dual-bank mode
    FLASH->CR2 |= FLASH_CR2_DUALBANK;
#else
    // Single-bank default
#endif
    return true;
}

bool MBR_Load(MasterBootRecord_t *mbr) {
    if (!mbr) return false;
    memcpy(mbr, (void *)BANK0_START_ADDR, sizeof(MasterBootRecord_t));
    return (mbr->signature == MBR_SIGNATURE);
}

void Bootloader_Run(void) {
    MasterBootRecord_t mbr;
    if (!Memory_Init() || !MBR_Load(&mbr)) {
        while (1); // Error lock
    }

    uint32_t app_addr = BANK0_START_ADDR + 0x8000;
#if (DUAL_BANK_MODE)
    // Optional bank swap logic
#endif

    __set_MSP(*(uint32_t *)app_addr);
    ((void(*)(void))(*(uint32_t *)(app_addr + 4)))();
}
