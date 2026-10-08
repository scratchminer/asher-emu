#include <stdio.h>

#include "../peripheral.h"

typedef struct {
	uint32_t cr;
	uint32_t apb1_fz;
	uint32_t apb2_fz;
} asher_dvt1_dbgmcu;

void *asher_peripheral_dvt1_dbgmcu_create(uint32_t baseAddr) {
	asher_dvt1_dbgmcu *dbgmcu = malloc(sizeof(asher_dvt1_dbgmcu));
	
	asher_peripheral_dvt1_dbgmcu_reset(dbgmcu);
	return dbgmcu;
}

void asher_peripheral_dvt1_dbgmcu_reset(void *userdata) {
	asher_dvt1_dbgmcu *dbgmcu = (asher_dvt1_dbgmcu *)userdata;
	
	dbgmcu->cr = 0x00000000;
	dbgmcu->apb1_fz = 0x00000000;
	dbgmcu->apb2_fz = 0x00000000;
}

uint64_t asher_peripheral_dvt1_dbgmcu_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_dbgmcu *dbgmcu = (asher_dvt1_dbgmcu *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00:
			return 0x10010449;
		case 0x04:
			return dbgmcu->cr;
		case 0x08:
			return dbgmcu->apb1_fz;
		case 0x0c:
			return dbgmcu->apb2_fz;
		default:
			return 0x00000000;
	}
}

void asher_peripheral_dvt1_dbgmcu_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_dbgmcu *dbgmcu = (asher_dvt1_dbgmcu *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x04:
			dbgmcu->cr = value & 0x000000e7;
			return;
		case 0x08:
			dbgmcu->apb1_fz = value & 0x07e01fff;
			return;
		case 0x0c:
			dbgmcu->apb2_fz = value & 0x00070003;
		default:
			return;
	}
}

void asher_peripheral_dvt1_dbgmcu_tick(uc_engine *uc, asher_peripheral *periph, uint64_t cycles) {
	return;
}

void asher_peripheral_dvt1_dbgmcu_destroy(void *userdata) {
	free(userdata);
}
