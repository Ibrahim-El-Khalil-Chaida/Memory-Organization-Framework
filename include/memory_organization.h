#ifndef MEMORY_ORGANIZATION_H
#define MEMORY_ORGANIZATION_H

#include <stdint.h>
#include <stdbool.h>

// Dual-bank mode toggle
#define DUAL_BANK_MODE    1

// Flash bank definitions
#define BANK0_START_ADDR  ((uint32_t)0x08000000)
#define BANK0_SIZE        ((uint32_t)0x00040000)
#define BANK1_START_ADDR  ((uint32_t)0x08040000)
#define BANK1_SIZE        ((uint32_t)0x00040000)

// MBR constants
#define MBR_SIGNATURE     0xAA55
#define MBR_SIZE          512

#pragma pack(push, 1)
typedef struct {
    uint8_t  jump_instruction[3];
    uint8_t  oem_name[8];
    uint8_t  partition_table[64];
    uint16_t signature;
} MasterBootRecord_t;
#pragma pack(pop)

// Initialize flash banks
bool Memory_Init(void);

// Load and verify MBR
bool MBR_Load(MasterBootRecord_t *mbr);

// Run bootloader
void Bootloader_Run(void);

#endif 
