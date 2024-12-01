#pragma once

#include <stdint.h>
#include <util.h>

struct RSDP_t {
        char Signature[8];
        uint8_t Checksum;
        char OEMID[6];
        uint8_t Revision;
        uint32_t *RsdtAddress;
} __attribute__ ((packed));

struct FACP {
    uint8_t Signature[4];
    uint32_t Length;
    uint8_t uneeded1[32];
    uint32_t *DSDT;
    uint8_t uneeded2[4];
    uint32_t *SMI_CMD;
    uint8_t ACPI_ENABLE;
    uint8_t ACPI_DISABLE;
    uint8_t uneeded3[10];
    uint32_t *PM1a_CNT_BLK;
    uint32_t *PM1b_CNT_BLK;
    uint8_t uneeded4[17];
    uint8_t PM1_CNT_LEN;
};

