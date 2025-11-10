#include <stdio.h>

#include "../peripheral.h"

typedef struct {
	uint32_t cr1;
	uint32_t csr1;
	uint32_t cr2;
	uint32_t csr2;
} asher_dvt1_pwr;

void *asher_peripheral_dvt1_pwr_create(uint32_t baseAddr) {
	asher_dvt1_pwr *pwr = malloc(sizeof(asher_dvt1_pwr));
	
	pwr->cr1 = 0x0000c000;
	pwr->csr1 = 0x00000008;
	pwr->cr2 = 0x00000000;
	pwr->csr2 = 0x00000000;
	
	return pwr;
}

uint64_t asher_peripheral_dvt1_pwr_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_pwr *pwr = (asher_dvt1_pwr *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00:
			return pwr->cr1;
		case 0x04:
			return pwr->csr1 & 0x0003820f;
		case 0x08:
			return pwr->cr2;
		case 0x0c:
			return pwr->csr2;
		default:
			return 0x00000000;
	}
}

void asher_peripheral_dvt1_pwr_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_pwr *pwr = (asher_dvt1_pwr *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00: {
			if ((value & 0x00000008) != 0) {
				pwr->csr1 &= 0xfffffffd;
			}
			
			pwr->cr1 = value & 0x000feff3;
			return;
		}
		case 0x04: {
			if ((value & 0x000c0000) == 0x000c0000) {
				pwr->csr1 &= 0xfff3ffff;
			}
			
			pwr->csr1 |= value & 0x00000300;
			return;
		}
		case 0x08:
			pwr->cr2 = value & 0x00003f3f;
		case 0x0c:
			pwr->csr2 &= 0xffffc0ff;
			pwr->csr2 |= value & 0x00003f00;
		default:
			return;
	}
}

void asher_peripheral_dvt1_pwr_destroy(void *userdata) {
	free(userdata);
}