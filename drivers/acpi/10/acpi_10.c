#include <stdint.h>
#include <util.h>
#include <stdio.h>
#include <io.h>

#include "acpi_10.h"

uint32_t *SMI_CMD;       
uint8_t ACPI_ENABLE;    
uint8_t ACPI_DISABLE;  
uint32_t *PM1a_CNT;   
uint32_t *PM1b_CNT;  
uint16_t SLP_TYPa;  
uint16_t SLP_TYPb; 
uint16_t SLP_EN;  
uint16_t SCI_EN; 
uint8_t PM1_CNT_LEN;     

extern inline void outw(unsigned short port, unsigned short value);

uint32_t *acpiCheckRSDPtr(uint32_t *ptr) 
{
        char *sig = "RSD PTR ";
	struct RSDP_t *rsdp = (struct RSDP_t *) ptr;
       	uint8_t *bptr;
	uint8_t check = 0;
	uint16_t i;

	if (__memcmp(sig, rsdp, 8) == 0) {
                bptr = (uint8_t *) ptr; 

		for (i = 0; i < sizeof(struct RSDP_t); i++)
		{
			check += *bptr;
			bptr++;
		}

		// found valid rsdp 
		if (check == 0) {
			if (rsdp->Revision == 0) {
				__printf("[acpi] ACPI version 1.0 \n");
                        }
			else {
				__printf("[acpi] unknown ACPI version \n");
                        }

			return (unsigned int *) rsdp->RsdtAddress;
		}
        }
        return NULL;
}

uint32_t *acpiGetRSDPtr(void)
{
	uint32_t *addr;
	uint32_t *rsdp;

	// search below the 1mb mark for RSDP signature
	for (addr = (unsigned int *) 0x000E0000; (int) addr < 0x00100000; addr += 0x10/sizeof(addr))
	{
		rsdp = acpiCheckRSDPtr(addr);
		if (rsdp != NULL) {
			return rsdp;
                }
	}


	// at address 0x40:0x0E is the RM segment of the ebda
	int ebda = *((short *) 0x40E);	// get pointer
	ebda = ebda*0x10 &0x000FFFFF;	// transform segment into linear address

	// search Extended BIOS Data Area for the RSDP signature
	for (addr = (unsigned int *) ebda; (int) addr<ebda+1024; addr+= 0x10/sizeof(addr))
	{
		rsdp = acpiCheckRSDPtr(addr);
		if (rsdp != NULL) {
			return rsdp;
                }
	}

	return NULL;
}

// checks for a given header and validates checksum
int acpiCheckHeader(uint32_t *ptr, char *sig)
{
	if (__memcmp(ptr, sig, 4) == 0)
	{
		char *checkPtr = (char *) ptr;
		int len = *(ptr + 1);
		char check = 0;
		while (0 < len--)
		{
			check += *checkPtr;
			checkPtr++;
		}
		if (check == 0) {
			return 0;
                }
	}
	return -1;
}

int acpiEnable(void)
{
	// check if acpi is enabled
	if ((inw((unsigned int) PM1a_CNT) &SCI_EN) == 0) {
		// check if acpi can be enabled
		if (SMI_CMD != 0 && ACPI_ENABLE != 0)
		{
			outb((unsigned int) SMI_CMD, ACPI_ENABLE); // send acpi enable command
			// give 3 seconds time to enable acpi
			int i;
			for (i=0; i<300; i++)
			{
				if ((inw((unsigned int) PM1a_CNT) &SCI_EN) == 1) {
					break;
                                }
				sleep(10);
			}

			if (PM1b_CNT != 0) {
				for (; i<300; i++ )
				{
					if ((inw((unsigned int) PM1b_CNT) &SCI_EN) == 1) {
						break;
                                        }
					sleep(10);
				}
                        }
			if (i<300) {
				__printf("[acpi] enabled acpi.\n");
				return 0;
			} else {
				__printf("[acpi] couldn't enable acpi.\n");
				return -1;
			}
		} else {
		        __printf("[acpi] no known way to enable acpi.\n");
			return -1;
		}
	} else {
		return 0;
	}
}

int initAcpi(void)
{
	unsigned int *ptr = acpiGetRSDPtr();

	// check if address is correct  (if acpi is available on this pc)
	if (ptr != NULL && acpiCheckHeader(ptr, "RSDT") == 0)
	{
		// the RSDT contains an unknown number of pointers to acpi tables
		int entrys = *(ptr + 1);
		entrys = (entrys-36) /4;
		ptr += 36/4;	// skip header information

		while (0<entrys--)
		{
			// check if the desired table is reached
			if (acpiCheckHeader((unsigned int *) *ptr, "FACP") == 0)
			{
				entrys = -2;
				struct FACP *facp = (struct FACP *) *ptr;
				if (acpiCheckHeader((unsigned int *) facp->DSDT, "DSDT") == 0)
				{
					// search the \_S5 package in the DSDT
					char *S5Addr = (char *) facp->DSDT +36; // skip header
					int dsdtLength = *(facp->DSDT+1) -36;
					while (0 < dsdtLength--)
					{
						if (__memcmp(S5Addr, "_S5_", 4) == 0)
							break;
						S5Addr++;
					}
					// check if \_S5 was found
					if (dsdtLength > 0)
					{
						// check for valid AML structure
						if ( ( *(S5Addr-1) == 0x08 || ( *(S5Addr-2) == 0x08 && *(S5Addr-1) == '\\') ) && *(S5Addr+4) == 0x12 )
						{
							S5Addr += 5;
							S5Addr += ((*S5Addr &0xC0)>>6) +2;	// calculate PkgLength size

							if (*S5Addr == 0x0A)
								S5Addr++;	// skip byteprefix
							SLP_TYPa = *(S5Addr)<<10;
							S5Addr++;

							if (*S5Addr == 0x0A)
								S5Addr++;	// skip byteprefix
							SLP_TYPb = *(S5Addr)<<10;

							SMI_CMD = facp->SMI_CMD;

							ACPI_ENABLE = facp->ACPI_ENABLE;
							ACPI_DISABLE = facp->ACPI_DISABLE;

							PM1a_CNT = facp->PM1a_CNT_BLK;
							PM1b_CNT = facp->PM1b_CNT_BLK;
							
							PM1_CNT_LEN = facp->PM1_CNT_LEN;

							SLP_EN = 1<<13;
							SCI_EN = 1;

							return 0;
						} else {
							__printf("[acpi] \\_S5 parse error.\n");
						}
					} else {
						__printf("[acpi] \\_S5 not present.\n");
					}
				} else {
				        __printf("[acpi] DSDT invalid.\n");
				}
			}
			ptr++;
		}
		__printf("[acpi] no valid FACP present.\n");
	} else {
		__printf("[acpi] warning: no acpi.\n");
	}

	return -1;
}



void acpiPowerOff(void)
{
	// SCI_EN is set to 1 if acpi shutdown is possible
	if (SCI_EN == 0)
		return;

	acpiEnable();

	// send the shutdown command
	outw((unsigned int) PM1a_CNT, SLP_TYPa | SLP_EN );
	if ( PM1b_CNT != 0 )
		outw((unsigned int) PM1b_CNT, SLP_TYPb | SLP_EN );

	__printf("[acpi] error: poweroff failed.\n");
}
