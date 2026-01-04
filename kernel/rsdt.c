
#include "rsdt.h"
#include "display.h"
#include "mem.h"
 
void rsdt_print(void * ptr){
	RSDP *t = ptr;


	print_cstring("RSDT:\n");
 
	print_cstring_len(t->Signature,8);
	print_cstring("\n");
	print_cstring_len(t->OEMID, 6);
	print_cstring("\n");


	print_cstring("Checksum: ");
	vga_print_hex(t->Checksum);
	print_cstring("\n");


	print_cstring("Revision: ");
	vga_print_hex(t->Revision);
	print_cstring("\n");

	print_cstring("RsdtAddress: ");
	vga_print_hex(t->RsdtAddress);
	print_cstring("\n");


	ACPISDTHeader * h = (ACPISDTHeader*)t->RsdtAddress;

	print_cstring("ACPISDTHeader signature: ");
	print_cstring_len(h->Signature, 4);
	print_cstring("\n");

	print_cstring("ACPISDTHeader OEMID: ");
	print_cstring_len(h->OEMID, 6);
	print_cstring("\n");

	print_cstring("ACPISDTHeader Len: ");
	vga_print_hex(h->Length);
	print_cstring("\n");

	print_cstring("ACPISDTHeader Size: ");
	vga_print_hex(sizeof(ACPISDTHeader));
	print_cstring("\n");

 
	
}


void *rsdt_findFACP(void *RootSDT)
{
	//should be another header somewhere below below 4 GB;
    RSDT *rsdt = (RSDT *) RootSDT;
    int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;

    for (int i = 0; i < entries; i++)
    {
        ACPISDTHeader *h = (ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
        if (mem_eq32(h->Signature, "FACP", 4))
            return (void *) h;
    }

    // No FACP found
    return nullptr;
}

void rsdt_print_FACP_address_spaces( void * ptr){
	FADT *fadt = (FADT *) ptr;
	print_cstring("rsdt_print_FACP_address_spaces\n");
	print_cstring("ResetReg:");
	vga_print_hex((uint32_t)fadt->ResetReg.AddressSpace);
	print_cstring("\n");
 
	print_cstring("X_PM1aEventBlock:");
	vga_print_hex((uint32_t)fadt->X_PM1aEventBlock.AddressSpace);
	print_cstring("\n");

	print_cstring("X_PM1bEventBlock:");
	vga_print_hex((uint32_t)fadt->X_PM1bEventBlock.AddressSpace);
	print_cstring("\n");

	print_cstring("X_PM1aControlBlock:");
	vga_print_hex((uint32_t)fadt->X_PM1aControlBlock.AddressSpace);
	print_cstring("\n");

	print_cstring("X_PM1bControlBlock:");
	vga_print_hex((uint32_t)fadt->X_PM1bControlBlock.AddressSpace);
	print_cstring("\n");

	print_cstring("X_PM2ControlBlock:");
	vga_print_hex((uint32_t)fadt->X_PM2ControlBlock.AddressSpace);
	print_cstring("\n");

	print_cstring("X_PMTimerBlock:");
	vga_print_hex((uint32_t)fadt->X_PMTimerBlock.AddressSpace);
	print_cstring("\n");

	print_cstring("X_GPE0Block:");
	vga_print_hex((uint32_t)fadt->X_GPE0Block.AddressSpace);
	print_cstring("\n");

	print_cstring("X_GPE1Block:");
	vga_print_hex((uint32_t)fadt->X_GPE1Block.AddressSpace);
	print_cstring("\n");

 
}

void rsdt_print_acpi_tabled(void * ptr){
	ACPI_Table * tab = (ACPI_Table * ) ptr;

	print_cstring("\nACPI_Table\n");
	print_cstring("Signature:");
	print_cstring_len(tab->header.Signature, 4);
	print_cstring("\n");

	
}

RSDP * rsdt_find_rsdp32(){

	char pat[] = "RSD PTR \n";
	
	uint32_t len = 8;
	uint32_t res = 	mem_search32_end((void *) 0x00080000, (void *) 	0x000FFFFF, pat, len);

	if(res == 0xffffffff)
	{
		return nullptr;
	}

	
	return (RSDP *)res;
}

ACPI_Table * rsdt_find_ecm32(RSDP * ptr){

	 

	RSDT * r = (RSDT * )ptr->RsdtAddress;

	int entries = (r->h.Length - sizeof(r->h)) / 4;

    for (int i = 0; i < entries; i++)
    {
        ACPISDTHeader *h = (ACPISDTHeader *) r->PointerToOtherSDT[i];
		print_cstring_len(h->Signature,4);
		print_cstring("\n");
        if (mem_eq32(h->Signature, "MCFG", 4))
            return (void *) h;
    }


	return nullptr;
}

 