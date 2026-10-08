#include <stdio.h>

#include "../peripheral.h"

typedef struct {
	uint32_t keyr;
	uint32_t optkeyr;
	uint32_t cr;
	uint32_t optcr;
	uint32_t optcr1;
} asher_dvt1_flash;

void *asher_peripheral_dvt1_flash_create(uint32_t baseAddr) {
	asher_dvt1_flash *flash = malloc(sizeof(asher_dvt1_flash));
	
	asher_peripheral_dvt1_flash_reset(flash);	
	return flash;
}

void asher_peripheral_dvt1_flash_reset(void *userdata) {
	asher_dvt1_flash *flash = (asher_dvt1_flash *)userdata;
	
	flash->keyr = 0x00000000;
	flash->optkeyr = 0x00000000;
	flash->cr = 0x80000000;
	flash->optcr = 0xc0ff55fd;
	flash->optcr1 = 0x00402000;
}

uint64_t asher_peripheral_dvt1_flash_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_flash *flash = (asher_dvt1_flash *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x0c:
			return 0x00000200;
		case 0x10:
			return flash->cr;
		case 0x14:
			return flash->optcr;
		case 0x18:
			return flash->optcr1;
		default:
			return 0x00000000;
	}
}

void asher_peripheral_dvt1_flash_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_flash *flash = (asher_dvt1_flash *)(((asher_peripheral *)periph)->userdata);
	
	switch (offset) {
		case 0x00: {
			if ((value & 0x00000100) != 0) {
				printf("[debug] FLASH prefetch enabled\n");
			}
			else {
				printf("[debug] FLASH prefetch disabled\n");
			}
			if ((value & 0x00000200) != 0) {
				printf("[debug] FLASH ART Accelerator enabled\n");
			}
			else {
				printf("[debug] FLASH ART Accelerator disabled\n");
			}
			
			printf("[debug] FLASH latency set to %llu-wait-state\n", value & 0x0000000f);
			
			return;
		}
		case 0x04: {
			if (value == 0xcdef89ab && flash->keyr == 0x45670123) {
				flash->cr &= 0x7fffffff;
			}
			
			flash->keyr = value;
			return;
		}
		case 0x08: {
			if (value == 0x4c5d6e7f && flash->optkeyr == 0x08192a3b) {
				flash->optcr &= 0xfffffffe;
			}
			
			flash->optkeyr = value;
			return;
		}
		case 0x10:
			flash->cr |= (value & 0x80000000);
			return;
		case 0x14:
			flash->optcr |= (value & 0x00000001);
			return;
		case 0x18:
			flash->optcr1 = value;
			return;
		default:
			return;
	}
}

void asher_peripheral_dvt1_flash_tick(uc_engine *uc, asher_peripheral *periph, uint64_t cycles) {
	return;
}

void asher_peripheral_dvt1_flash_destroy(void *userdata) {
	free(userdata);
}
