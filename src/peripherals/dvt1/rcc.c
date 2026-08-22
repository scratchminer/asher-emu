#include "../peripheral.h"

typedef struct {
	uint32_t cr;
	uint32_t pllcfgr;
	uint32_t cfgr;
	uint16_t cir;
	uint32_t ahb1rstr;
	uint32_t ahb2rstr;
	uint32_t ahb3rstr;
	uint32_t apb1rstr;
	uint32_t apb2rstr;
	uint32_t ahb1enr;
	uint32_t ahb2enr;
	uint32_t ahb3enr;
	uint32_t apb1enr;
	uint32_t apb2enr;
	uint32_t ahb1lpenr;
	uint32_t ahb2lpenr;
	uint32_t ahb3lpenr;
	uint32_t apb1lpenr;
	uint32_t apb2lpenr;
	uint32_t bdcr;
	uint32_t dckcfgr1;
	uint32_t csr;
} asher_dvt1_rcc;

void *asher_peripheral_dvt1_rcc_create(uint32_t baseAddr) {
	asher_dvt1_rcc *rcc = malloc(sizeof(asher_dvt1_rcc));
	
	asher_peripheral_dvt1_rcc_reset(rcc);
	return rcc;
}

void asher_peripheral_dvt1_rcc_reset(void *userdata) {
	asher_dvt1_rcc *rcc = (asher_dvt1_rcc *)userdata;
	
	rcc->cr = 0x2a026083;
	rcc->pllcfgr = 0x24003010;
	rcc->cfgr = 0x00000000;
	rcc->cir = 0x0000;
	
	rcc->ahb1rstr = 0x00000000;
	rcc->ahb2rstr = 0x00000000;
	rcc->ahb3rstr = 0x00000000;
	rcc->apb1rstr = 0x00000000;
	rcc->apb2rstr = 0x00000000;
	
	rcc->ahb1enr = 0x00010000;
	rcc->ahb2enr = 0x00000000;
	rcc->ahb3enr = 0x00000000;
	rcc->apb1enr = 0x00000000;
	rcc->apb2enr = 0x00000000;
	
	rcc->ahb1lpenr = 0x7ef7b7ff;
	rcc->ahb2lpenr = 0x000000f1;
	rcc->ahb3lpenr = 0x00000003;
	rcc->apb1lpenr = 0xffffcbff;
	rcc->apb2lpenr = 0x04f77f33;
	
	rcc->bdcr = 0x00000000;
	rcc->csr = 0x0e000002;
	rcc->dckcfgr1 = 0x00000000;
}

uint64_t asher_peripheral_dvt1_rcc_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_rcc *rcc = (asher_dvt1_rcc *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00:
			return rcc->cr;
		case 0x04:
			return rcc->pllcfgr;
		case 0x08:
			return rcc->cfgr;
		case 0x0c:
			return rcc->cir;
		case 0x10:
			return rcc->ahb1rstr;
		case 0x14:
			return rcc->ahb2rstr;
		case 0x18:
			return rcc->ahb3rstr;
		case 0x20:
			return rcc->apb1rstr;
		case 0x24:
			return rcc->apb2rstr;
		case 0x30:
			return rcc->ahb1enr;
		case 0x34:
			return rcc->ahb2enr;
		case 0x38:
			return rcc->ahb3enr;
		case 0x40:
			return rcc->apb1enr;
		case 0x44:
			return rcc->apb2enr;
		case 0x50:
			return rcc->ahb1lpenr;
		case 0x54:
			return rcc->ahb2lpenr;
		case 0x58:
			return rcc->ahb3lpenr;
		case 0x60:
			return rcc->apb1lpenr;
		case 0x64:
			return rcc->apb2lpenr;
		case 0x70:
			return rcc->bdcr;
		case 0x74:
			return rcc->csr;
		case 0x8c:
			return rcc->dckcfgr1;
		default:
			return 0x00000000;
	}
}

void asher_peripheral_dvt1_rcc_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_rcc *rcc = (asher_dvt1_rcc *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00:
			rcc->cr = (rcc->cr & 0x2a02ff02) | (value & 0x150d00f9);
			return;
		case 0x04: {
			rcc->pllcfgr = value & 0x0f437fff;
			
			double freq = 16.0;
			freq = freq / (rcc->pllcfgr & 0x3f) * ((rcc->pllcfgr >> 6) & 0x1ff) / (((rcc->pllcfgr >> 15) & 0x6) + 2);
			printf("[debug] RCC PLL frequency set to %.2f MHz\n", freq);
			return;
		}
		case 0x08: {
			rcc->cfgr = (value & 0xfffffcf3) | ((value << 2) & 0x0000000c);
			
			char *mode = "";
			
			switch (value & 0x00000003) {
				case 0:
					mode = "HSI oscillator";
					break;
				case 1:
					mode = "HSE oscillator";
					break;
				case 2:
					mode = "PLL";
					break;
				default:
					break;
			}
			printf("[debug] RCC system clock source set to %s\n", mode);
			return;
		}
		case 0x0c:
			rcc->cir = ((rcc->cir & 0xff) & ~(value >> 16)) | (value & 0xff00);
			return;
		case 0x74: {
			if ((value & 0x01000000) != 0) {
				rcc->csr &= 0x01ffffff;
			}
			return;
		}
		default:
			return;
	}
	
	if (offset == 0x74) {
		
	}
}

void asher_peripheral_dvt1_rcc_destroy(void *userdata) {
	free(userdata);
}
