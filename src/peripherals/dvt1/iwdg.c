#include <stdio.h>

#include "../peripheral.h"

typedef struct {
	uint32_t kr;
	uint32_t pr;
	uint32_t rlr;
	uint32_t winr;
	uint32_t counter;
} asher_dvt1_iwdg;

void *asher_peripheral_dvt1_iwdg_create(uint32_t baseAddr) {
	asher_dvt1_iwdg *iwdg = malloc(sizeof(asher_dvt1_iwdg));
	
	asher_peripheral_dvt1_iwdg_reset(iwdg);
	return iwdg;
}

void asher_peripheral_dvt1_iwdg_reset(void *userdata) {
	asher_dvt1_iwdg *iwdg = (asher_dvt1_iwdg *)userdata;
	
	iwdg->kr = 0x00000000;
	iwdg->pr = 0x00000000;
	iwdg->rlr = 0x00000fff;
	iwdg->winr = 0x00000fff;
	iwdg->counter = 0x00000fff;
	return;
}

uint64_t asher_peripheral_dvt1_iwdg_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_iwdg *iwdg = (asher_dvt1_iwdg *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00:
			return 0x00000000;
		case 0x04:
			return iwdg->pr;
		case 0x08:
			return iwdg->rlr;
		case 0x0c:
			return 0x00000000;
		case 0x10:
			return iwdg->winr;
		default:
			return 0x00000000;
	}
}

void asher_peripheral_dvt1_iwdg_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_iwdg *iwdg = (asher_dvt1_iwdg *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00: {
			if ((value & 0x0000ffff) == 0x0000aaaa) {
				iwdg->counter &= 0xffff0000;
				iwdg->counter |= iwdg->rlr << 8;
			}
			else if ((value & 0x0000ffff) == 0x0000cccc) {
				iwdg->counter |= 0x80000000;
			}
			
			iwdg->kr = value & 0x0000ffff;
			return;
		}
		case 0x04: {
			if (iwdg->kr == 0x00005555) {
				iwdg->pr = value & 0x00000007;
			}
			return;
		}
		case 0x08: {
			if (iwdg->kr == 0x00005555) {
				iwdg->rlr = value & 0x00000fff;
			}
			return;
		}
		case 0x0c:
			return;
		case 0x10: {
			if (iwdg->kr == 0x00005555) {
				iwdg->winr = value & 0x00000fff;
			}
		}
		default:
			return;
	}
}

void asher_peripheral_dvt1_iwdg_destroy(void *userdata) {
	free(userdata);
}

void asher_peripheral_dvt1_iwdg_tick(void *periph, uint32_t cycles) {
	asher_dvt1_iwdg *iwdg = (asher_dvt1_iwdg *)(((asher_peripheral *)periph)->userdata);
	
	if ((iwdg->counter & 0x80000000) == 0x80000000) {
		uint32_t prescaler = (iwdg->counter & 0x000000ff) + cycles;
		uint32_t divider = ((4 << iwdg->pr) - 1) & 0xff;
		
		while (prescaler >= divider) {
			if (iwdg->counter < 0x00000100) {
				// todo: set RCC_CSR.IWDGRSTF
				asher_device_reset(((asher_peripheral *)periph)->device);
				return;
			}
			
			iwdg->counter -= 0x00000100;
			prescaler -= divider;
		}
	}
}
