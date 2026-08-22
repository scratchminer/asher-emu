#include <stdio.h>

#include "peripheral.h"

typedef struct {
	uint32_t baseAddr;
} asher_dummy;

void *asher_peripheral_dummy_create(uint32_t baseAddr) {
	asher_dummy *dummy = malloc(sizeof(asher_dummy));
	
	dummy->baseAddr = baseAddr;
	return dummy;
}

uint64_t asher_peripheral_dummy_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dummy *dummy = (asher_dummy *)(((asher_peripheral *)periph)->userdata);
	
	printf("[debug] Stub %u-bit read at address 0x%08llx\n", size << 3, dummy->baseAddr + offset);
	
	return 0x00000000;
}

void asher_peripheral_dummy_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dummy *dummy = (asher_dummy *)(((asher_peripheral *)periph)->userdata);
	
	switch (size) {
		case 1:
			printf("[debug] Stub 8-bit write: 0x%02llx -> address 0x%08llx\n", value, dummy->baseAddr + offset);
			return;
		case 2:
			printf("[debug] Stub 16-bit write: 0x%04llx -> address 0x%08llx\n", value, dummy->baseAddr + offset);
			return;
		case 4:
		default:
			printf("[debug] Stub 32-bit write: 0x%08llx -> address 0x%08llx\n", value, dummy->baseAddr + offset);
			return;
	}
}

void asher_peripheral_dummy_destroy(void *userdata) {
	free(userdata);
}
