#pragma once
#include <stdint.h>


typedef struct ACPISDTHeader ACPISDTHeader;

typedef struct RSDP RSDP;

typedef struct ACPI_Table ACPI_Table;

typedef struct FADT FADT;
typedef struct RSDT RSDT;
typedef struct Config_space Config_space ;
typedef struct GenericAddressStructure GenericAddressStructure;


 struct RSDP{
	char 		Signature[8];
	uint8_t 	Checksum;
	char 		OEMID[6];
	uint8_t 	Revision;
	uint32_t 	RsdtAddress;
} __attribute__ ((packed));

/**
packed when size  does not add up to multiple of 32bits
*/

struct  ACPISDTHeader{
	char Signature[4];

	//Total size of the table, inclusive of the header.
	uint32_t Length;

	uint8_t Revision;
	uint8_t Checksum;
	char OEMID[6];

	char OEMTableID[8];

	uint32_t OEMRevision;
	uint32_t CreatorID;
	uint32_t CreatorRevision;
};
struct Config_space{
	uint8_t base_addr;
	uint8_t bits;
};
struct ACPI_Table{
	ACPISDTHeader header;
	char reserved[8];
	Config_space Configuration_space_structs[];
};

struct RSDT {
  struct ACPISDTHeader h;
  uint32_t PointerToOtherSDT[]; //(h.Length - sizeof(h)) / 4
 
};

struct GenericAddressStructure
{
  uint8_t AddressSpace;
  uint8_t BitWidth;
  uint8_t BitOffset;
  uint8_t AccessSize;
  uint64_t Address;
};

struct FADT
{
    struct   ACPISDTHeader h;
    uint32_t FirmwareCtrl;
    uint32_t Dsdt;

    // field used in ACPI 1.0; no longer in use, for compatibility only
    uint8_t  Reserved;

    uint8_t  PreferredPowerManagementProfile;
    uint16_t SCI_Interrupt;
    uint32_t SMI_CommandPort;
    uint8_t  AcpiEnable;
    uint8_t  AcpiDisable;
    uint8_t  S4BIOS_REQ;
    uint8_t  PSTATE_Control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t  PM1EventLength;
    uint8_t  PM1ControlLength;
    uint8_t  PM2ControlLength;
    uint8_t  PMTimerLength;
    uint8_t  GPE0Length;
    uint8_t  GPE1Length;
    uint8_t  GPE1Base;
    uint8_t  CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t  DutyOffset;
    uint8_t  DutyWidth;
    uint8_t  DayAlarm;
    uint8_t  MonthAlarm;
    uint8_t  Century;

    // reserved in ACPI 1.0; used since ACPI 2.0+
    uint16_t BootArchitectureFlags;

    uint8_t  Reserved2;
    uint32_t Flags;

    // 12 byte structure; see below for details
    GenericAddressStructure ResetReg;

    uint8_t  ResetValue;
    uint8_t  Reserved3[3];
  
    // 64bit pointers - Available on ACPI 2.0+
    uint64_t                X_FirmwareControl;
    uint64_t                X_Dsdt;

    GenericAddressStructure X_PM1aEventBlock;
    GenericAddressStructure X_PM1bEventBlock;
    GenericAddressStructure X_PM1aControlBlock;
    GenericAddressStructure X_PM1bControlBlock;
    GenericAddressStructure X_PM2ControlBlock;
    GenericAddressStructure X_PMTimerBlock;
    GenericAddressStructure X_GPE0Block;
    GenericAddressStructure X_GPE1Block;
};


void rsdt_print_acpi_tabled(void * ptr);

void rsdt_print(void * ptr);


void *rsdt_findFACP(void *RootSDT);

void rsdt_print_FACP_address_spaces( void * FADT);

RSDP * rsdt_find_rsdp32();

ACPI_Table * rsdt_find_ecm32(RSDP * ptr);
