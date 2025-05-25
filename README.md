# Memory Organization Framework

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)  [![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)  [![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()

**Master Single- & Dual-Bank Flash Memory Management with Bootloader & MBR**

This repository provides a professional-grade, modular C framework for embedded microcontrollers to safely manage flash memory layouts—supporting both single- and dual-bank configurations—alongside a robust bootloader and Master Boot Record (MBR) handling. It’s designed for reliability, portability, and ease of integration in safety- or security-critical systems (e.g., automotive ECUs, IoT gateways, industrial controllers).

---

## Table of Contents

1. [Overview](#overview)  
2. [Key Features](#key-features)  
3. [Architecture & Design](#architecture--design)  
4. [Getting Started](#getting-started)  
   - [Prerequisites](#prerequisites)  
   - [Clone & Build](#clone--build)  
   - [Flashing & Deployment](#flashing--deployment)  
5. [Configuration](#configuration)  
6. [Operational Flow](#operational-flow)  
7. [Extensibility & Security](#extensibility--security)  
8. [Contributing](#contributing)  

---

## Overview

Embedded systems often require over-the-air updates, rollback mechanisms, and guaranteed safe boot to prevent bricking in the field. The **Memory Organization Framework** addresses these needs by:

- Abstracting flash-bank management for **single** or **dual** A/B partitions.  
- Implementing a **packed MBR** loader (512-byte sector) with signature validation.  
- Providing a **bootloader** to select, swap, and launch application images securely.  
- Maintaining **modularity** for rapid adaptation across MCU families and toolchains.

This framework ensures your device always boots a valid image and facilitates seamless firmware updates.

---

## Key Features

- **Configurable Bank Modes** – Toggle between single-bank (linear) and dual-bank (A/B) layouts via a single macro (`DUAL_BANK_MODE`).  
- **Master Boot Record (MBR)** – Packed struct matching on-flash layout, signature-validated, extensible to CRC or cryptographic checks.  
- **Robust Bootloader** – Reads MBR, parses partition table, supports bank swap triggers (GPIO/pin or flag), sets MSP, and jumps to application.  
- **Fail-Safe A/B Swapping** – Prevents bricking by falling back to the last known good image if validation fails.  
- **Portable Design** – Hardware abstraction layer isolates MCU-specific registers; easily retarget to STM32, NXP, Microchip, etc.  
- **Clear Documentation** – Comprehensive README, inline code comments, and header docs for quick onboarding.

---

## Architecture & Design

```text
[ Reset ISR ]
      ↓
[ Bootloader_Run() ]
      ↓
[ Memory_Init() ] → Configure flash controller & bank registers
      ↓
[ MBR_Load() ] → Read 512 B MBR sector, validate 0xAA55 signature
      ↓
[ Parse Partition Table ] → Determine active image; dual-bank swap logic
      ↓
[ __set_MSP() & Jump ] → Start application reset handler
```

- **Memory_Init:** Unlocks flash, sets bank-swap bits if dual-bank.  
- **MBR_Load:** Reads sector into a packed C struct; signature check.  
- **Partition Parsing:** Four 16-byte entries define image offsets and flags.  
- **Application Jump:** Sets Main Stack Pointer and calls the reset handler of the chosen image.

---

## Getting Started

### Prerequisites

- ARM Cortex-M toolchain (GNU Arm Embedded Toolchain or vendor SDK)  
- GNU Make or CMake  
- Flashing utility (OpenOCD, ST-LINK Utility, J-Link, etc.)

### Clone & Build

```bash
# Clone the repository
git clone https://github.com/your-username/memory-organization.git
cd memory-organization

# Build with Make
git submodule update --init --recursive
make all
# or, with CMake
# mkdir build && cd build
# cmake .. && make
```

### Flashing & Deployment

1. **Bootloader** → Flash to start of Bank 0.  
2. **Applications** → Compile with base address offset (e.g., `0x08008000`) and flash to Bank 0 or Bank 1.  
3. **Swap Test** → Trigger swap via configured GPIO or MBR flag; reset device and verify new image boots.  

---

## Configuration

All configurable parameters reside in `include/config.h` (or `src/config.h`):

- **Bank Addresses:** `BANK0_START_ADDR`, `BANK0_SIZE`, `BANK1_START_ADDR`, `BANK1_SIZE`.  
- **Dual-Bank Mode:** Enable/disable with `#define DUAL_BANK_MODE 1`.  
- **MBR Size & Signature:** Adjust `MBR_SIZE` and `MBR_SIGNATURE` if deviating from standard.  
- **Partition Offsets:** Modify application base offsets for custom layouts.

---

## Operational Flow

1. **Power-On Reset** triggers `Reset_Handler`.  
2. Bootloader decides: **Force Bootloader** (e.g., pin hold) or **Direct Jump**.  
3. **Memory_Init:** Setup flash banks.  
4. **MBR_Load:** Validate Master Boot Record.  
5. **Partition Parsing & Swap:** Choose proper image bank; optionally swap A/B.  
6. **Jump:** Set MSP and branch to application.

---

## Extensibility & Security

- **CRC/Hash Verification:** Integrate CRC32 or SHA-256 checks before jumping.  
- **Secure Boot:** Plug in asymmetric signature checks (e.g., ECDSA) to validate image authenticity.  
- **FOTA Integration:** Combine with network stacks (LwIP, MbedTLS) for over-the-air updates.  
- **Test Harness:** Simulate flash faults and power failures to validate robustness.

---

## Contributing

We welcome contributions! To get started:

1. Fork the repo & sync submodules.  
2. Create a feature branch: `git checkout -b feature/your-feature`.  
3. Commit changes with clear messages.  
4. Push to your fork & open a Pull Request.  
5. We’ll review, provide feedback, and merge!

Please adhere to the existing coding style and include unit tests where applicable.

---

Thank you for using the Memory Organization Framework! For questions or support, please open an issue.
