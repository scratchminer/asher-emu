#include <stdio.h>

#include "../peripheral.h"

typedef struct {
	uint32_t dummy;
} asher_dvt1_iwdg;

void *asher_peripheral_dvt1_iwdg_create(uint32_t baseAddr) {
	asher_dvt1_iwdg *iwdg = malloc(sizeof(asher_dvt1_iwdg));
	
	asher_peripheral_dvt1_iwdg_reset(iwdg);
	return iwdg;
}

void asher_peripheral_dvt1_iwdg_reset(void *userdata) {
	return;
}

uint64_t asher_peripheral_dvt1_iwdg_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_iwdg *iwdg = (asher_dvt1_iwdg *)(((asher_peripheral *)periph)->userdata);
	(void)iwdg;
	
	return 0x00000000;
}

void asher_peripheral_dvt1_iwdg_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_iwdg *iwdg = (asher_dvt1_iwdg *)(((asher_peripheral *)periph)->userdata);
	
	(void)iwdg;
}

void asher_peripheral_dvt1_iwdg_destroy(void *userdata) {
	free(userdata);
}
