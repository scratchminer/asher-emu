#include <stdio.h>

#include "../peripheral.h"

typedef struct {
	uint32_t memrmp;
	uint32_t pmc;
	uint16_t exticr[4];
	uint32_t cmpcr;
} asher_dvt1_syscfg;

void *asher_peripheral_dvt1_syscfg_create(uint32_t baseAddr) {
	asher_dvt1_syscfg *syscfg = malloc(sizeof(asher_dvt1_syscfg));
	
	asher_peripheral_dvt1_syscfg_reset(syscfg);
	return syscfg;
}

void asher_peripheral_dvt1_syscfg_reset(void *userdata) {
	asher_dvt1_syscfg *syscfg = (asher_dvt1_syscfg *)userdata;
	
	syscfg->memrmp = 0x00000000;
	syscfg->pmc = 0x00000000;
	for (uint8_t i = 0; i < 4; i++) {
		syscfg->exticr[i] = 0x0000;
	}
	syscfg->cmpcr = 0x00000100;
}

uint64_t asher_peripheral_dvt1_syscfg_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_syscfg *syscfg = (asher_dvt1_syscfg *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00:
			return syscfg->memrmp;
		case 0x04:
			return syscfg->pmc;
		case 0x08:
		case 0x0c:
		case 0x10:
		case 0x14:
			return syscfg->exticr[(offset - 0x08) >> 3];
		case 0x18:
			return syscfg->cmpcr;
		default:
			return 0x00000000;
	}
}

void asher_peripheral_dvt1_syscfg_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_syscfg *syscfg = (asher_dvt1_syscfg *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00:
			syscfg->memrmp &= 0xfffff3ff;
			syscfg->memrmp |= value & 0x00000c00;
			return;
		case 0x04:
			syscfg->pmc &= 0xff78ffff;
			syscfg->pmc |= value & 0x00870000;
			return;
		case 0x08:
		case 0x0c:
		case 0x10:
		case 0x14:
			syscfg->exticr[(offset - 0x08) >> 2] = value & 0xffff;
			return;
		case 0x18:
			syscfg->cmpcr &= 0xfffffffe;
			syscfg->cmpcr |= value & 0x00000001;
			return;
		default:
			return;
	}
}

void asher_peripheral_dvt1_syscfg_destroy(void *userdata) {
	free(userdata);
}
