#include "../peripheral.h"

typedef struct {
	uint32_t csr;
} asher_dvt1_rcc;

void *asher_peripheral_dvt1_rcc_create(uint32_t baseAddr) {
	asher_dvt1_rcc *rcc = malloc(sizeof(asher_dvt1_rcc));
	
	// power-on reset, pin reset, low-speed internal oscillator ready
	rcc->csr = 0x0e000002;
	
	return rcc;
}

uint64_t asher_peripheral_dvt1_rcc_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_rcc *rcc = (asher_dvt1_rcc *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00:
			return 0x02026083;
		case 0x74:
			return rcc->csr;
		default:
			return 0x00000000;
	}
}

void asher_peripheral_dvt1_rcc_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_rcc *rcc = (asher_dvt1_rcc *)(((asher_peripheral *)periph)->userdata);
	
	if (offset == 0x74) {
		if ((value & 0x01000000) != 0) {
			rcc->csr &= 0x01ffffff;
		}
	}
}

void asher_peripheral_dvt1_rcc_destroy(void *userdata) {
	free(userdata);
}