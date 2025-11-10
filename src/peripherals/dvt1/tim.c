#include <stdio.h>

#include "../peripheral.h"

typedef struct {
	uint8_t id;
	
	uint16_t cr1;
	uint16_t cr2;
	uint32_t smcr;
	uint16_t dier;
	uint16_t sr;
	uint16_t egr;
	uint32_t ccmr1;
	uint32_t ccmr2;
	uint16_t ccer;
	uint32_t cnt;
	uint32_t psc;
	uint32_t arr;
	uint16_t rcr;
	uint32_t ccr1;
	uint32_t ccr2;
	uint32_t ccr3;
	uint32_t ccr4;
	uint32_t bdtr;
	uint16_t dcr;
	uint16_t dmar;
	uint16_t or;
	uint32_t ccmr3;
	uint32_t ccr5;
	uint32_t ccr6;
} asher_dvt1_tim;

void *asher_peripheral_dvt1_tim_create(uint32_t baseAddr) {
	asher_dvt1_tim *tim = malloc(sizeof(asher_dvt1_tim));
	
	tim->cr1 = 0x0000;
	tim->cr2 = 0x0000;
	tim->smcr = 0x00000000;
	tim->dier = 0x0000;
	tim->sr = 0x0000;
	tim->egr = 0x0000;
	tim->ccmr1 = 0x00000000;
	tim->ccmr2 = 0x00000000;
	tim->ccer = 0x0000;
	tim->cnt = 0x00000000;
	tim->psc = 0x00000000;
	tim->rcr = 0x0000;
	tim->ccr1 = 0x00000000;
	tim->ccr2 = 0x00000000;
	tim->ccr3 = 0x00000000;
	tim->ccr4 = 0x00000000;
	tim->bdtr = 0x00000000;
	tim->dcr = 0x0000;
	tim->dmar = 0x0000;
	tim->or = 0x0000;
	tim->ccmr3 = 0x00000000;
	tim->ccr5 = 0x00000000;
	tim->ccr6 = 0x00000000;
	
	switch (baseAddr) {
		case 0x40000000:
			tim->arr = 0xffffffff;
			tim->id = 2;
			break;
		case 0x40000400:
			tim->arr = 0x0000ffff;
			tim->id = 3;
			break;
		case 0x40000800:
			tim->arr = 0x0000ffff;
			tim->id = 4;
			break;
		case 0x40000c00:
			tim->arr = 0xffffffff;
			tim->id = 5;
			break;
		case 0x40001000:
			tim->arr = 0x0000ffff;
			tim->id = 6;
			break;
		case 0x40001400:
			tim->arr = 0x0000ffff;
			tim->id = 7;
			break;
		case 0x40001800:
			tim->arr = 0x0000ffff;
			tim->id = 12;
			break;
		case 0x40001c00:
			tim->arr = 0x0000ffff;
			tim->id = 13;
			break;
		case 0x40002000:
			tim->arr = 0x0000ffff;
			tim->id = 14;
			break;
		case 0x40010000:
			tim->arr = 0x0000ffff;
			tim->id = 1;
			break;
		case 0x40010400:
			tim->arr = 0x0000ffff;
			tim->id = 8;
			break;
		case 0x40014400:
			tim->arr = 0x0000ffff;
			tim->id = 10;
			break;
		case 0x40014800:
			tim->arr = 0x0000ffff;
			tim->id = 11;
			break;
		default:
			break;
	}
	
	return tim;
}

uint64_t asher_peripheral_dvt1_tim_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_tim *tim = (asher_dvt1_tim *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00:
			return tim->cr1;
		case 0x04:
			return tim->cr2;
		case 0x08:
			return tim->smcr;
		case 0x0c:
			return tim->dier;
		case 0x10:
			return tim->sr;
		case 0x14:
			return tim->egr;
		case 0x18:
			return tim->ccmr1;
		case 0x1c:
			return tim->ccmr2;
		case 0x20:
			return tim->ccer;
		case 0x24: {
			if (((tim->cr1 >> 11) & 0x0001) != 0) {
				return tim->cnt | ((tim->sr & 0x0001) << 31);
			}
			else {
				return tim->cnt;
			}
		}
		case 0x28:
			return tim->psc;
		case 0x2c:
			return tim->arr;
		case 0x30:
			return tim->rcr;
		case 0x34:
			return tim->ccr1;
		case 0x38:
			return tim->ccr2;
		case 0x3c:
			return tim->ccr3;
		case 0x40:
			return tim->ccr4;
		case 0x48:
			return tim->dcr;
		case 0x4c:
			return tim->dmar;
		case 0x50:
			return tim->or;
		case 0x54:
			return tim->ccmr3;
		case 0x58:
			return tim->ccr5;
		case 0x5c:
			return tim->ccr6;
		default:
			return 0x00000000;
	}
}

void asher_peripheral_dvt1_tim_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_tim *tim = (asher_dvt1_tim *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00: {
			uint16_t mask;
			if (tim->id >= 9 && tim->id <= 14) {
				mask = 0x0b8f;
			}
			else if (tim->id == 6 || tim->id == 7) {
				mask = 0x088f;
			}
			else {
				mask = 0x0bff;
			}
			
			uint16_t changedValues = tim->cr1 ^ (value & mask);
			uint8_t newValue;
			
			if ((changedValues & 0x0060) != 0) {
				uint8_t newValue = (value >> 5) & 0x00000003;
				char *mode = "";
				
				if (newValue == 0) {
					mode = "edge-aligned";
				}
				else if (newValue == 1) {
					mode = "center-aligned with compare during count-down";
				}
				else if (newValue == 2) {
					mode = "center-aligned with compare during count-up";
				}
				else if (newValue == 3) {
					mode = "center-aligned with compare during count-up and count-down";
				}
				
				printf("[debug] TIM%d compare mode set to %s\n", tim->id, mode);
			}
			if ((changedValues & 0x0010) != 0) {
				uint8_t newValue = (value >> 4) & 0x00000001;
				printf("[debug] TIM%d count direction set to %s\n", tim->id, (newValue == 0) ? "up" : "down");
			}
			if ((changedValues & 0x0008) != 0) {
				uint8_t newValue = (value >> 3) & 0x00000001;
				printf("[debug] TIM%d count mode set to %s\n", tim->id, (newValue == 0) ? "continuous" : "one-pulse");
			}
			if ((changedValues & 0x0004) != 0) {
				uint8_t newValue = (value >> 2) & 0x00000001;
				printf("[debug] TIM%d update request mode set to %s\n", tim->id, (newValue == 0) ? "external" : "internal");
			}
			if ((changedValues & 0x0001) != 0) {
				uint8_t newValue = (value >> 1) & 0x00000001;
				printf("[debug] TIM%d update event %s\n", tim->id, (newValue == 0) ? "enabled" : "disabled");
			}
			
			tim->cr1 ^= changedValues;
			return;
		}
		case 0x04:
			// todo: tim->cr2
			return;
		case 0x08:
			// todo: tim->smcr
			return;
		case 0x0c:
			// todo: tim->dier
			return;
		case 0x10:
			// todo: tim->sr
			return;
		case 0x14:
			// todo: tim->egr
			return;
		case 0x18:
			// todo: tim->ccmr1
			return;
		case 0x1c:
			// todo: tim->ccmr2
			return;
		case 0x20:
			// todo: tim->ccer
			return;
		case 0x24: {
			value &= 0xffff;
			
			if (tim->cnt != value) {
				printf("[debug] TIM%d counter value set to 0x%04llx\n", tim->id, value);
			}
			
			tim->cnt = value;
			return;
		}
		case 0x28: {
			value &= 0xffff;
			
			if (tim->psc != value) {
				printf("[debug] TIM%d prescaler value set to 0x%04llx\n", tim->id, value);
			}
			
			tim->psc = value;
			return;
		}
		case 0x2c: {
			uint32_t mask = 0x0000ffff;
			
			if (tim->id == 2 || tim->id == 5) {
				mask = 0xffffffff;
			}
			
			value &= mask;
			
			if (tim->arr != value) {
				printf("[debug] TIM%d auto-reload register set to 0x%04llx\n", tim->id, value);
			}
			
			tim->arr = value;
			return;
		}
		case 0x30:
			tim->rcr = value & 0xffff;
			return;
		case 0x34:
			return;
		case 0x38:
			return;
		case 0x3c:
			return;
		case 0x40:
			return;
		case 0x48:
			return;
		case 0x4c:
			return;
		case 0x50:
			return;
		case 0x54:
			return;
		case 0x58:
			return;
		case 0x5c:
			return;
		default:
			return;
	}
}

void asher_peripheral_dvt1_tim_destroy(void *userdata) {
	free(userdata);
}