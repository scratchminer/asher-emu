#include "../peripheral.h"

typedef struct {
	char id;
	
	uint32_t moder;
	uint32_t otyper;
	uint32_t ospeedr;
	uint32_t pupdr;
	uint32_t odr;
	uint32_t afrl;
	uint32_t afrh;
} asher_dvt1_gpio;

void *asher_peripheral_dvt1_gpio_create(uint32_t baseAddr) {
	asher_dvt1_gpio *gpio = malloc(sizeof(asher_dvt1_gpio));
	
	gpio->moder = 0x00000000;
	gpio->otyper = 0x00000000;
	gpio->ospeedr = 0x00000000;
	gpio->pupdr = 0x00000000;
	gpio->odr = 0x00000000;
	gpio->afrl = 0x00000000;
	gpio->afrh = 0x00000000;
	
	switch (baseAddr) {
		case 0x40020000:
			gpio->id = 'A';
			gpio->moder = 0xa8000000;
			gpio->ospeedr = 0x0c000000;
			gpio->pupdr = 0x64000000;
			break;
		case 0x40020400:
			gpio->id = 'B';
			gpio->moder = 0x00000280;
			gpio->ospeedr = 0x000000c0;
			gpio->pupdr = 0x00000100;
			break;
		case 0x40020800:
			gpio->id = 'C';
			break;
		case 0x40020c00:
			gpio->id = 'D';
			break;
		case 0x40021000:
			gpio->id = 'E';
			break;
		case 0x40021400:
			gpio->id = 'F';
			break;
		case 0x40021800:
			gpio->id = 'G';
			break;
		case 0x40021c00:
			gpio->id = 'H';
			break;
		case 0x40022000:
			gpio->id = 'I';
			break;
		case 0x40022400:
			gpio->id = 'J';
			break;
		case 0x40022800:
			gpio->id = 'K';
			break;
	}
	
	return gpio;
}

uint64_t asher_peripheral_dvt1_gpio_read(uc_engine *uc, uint64_t offset, unsigned size, void *userdata) {
	asher_dvt1_gpio *gpio = (asher_dvt1_gpio *)userdata;
	
	switch (offset) {
		case 0x00:
			return gpio->moder;
		case 0x04:
			return gpio->otyper;
		case 0x08:
			return gpio->ospeedr;
		case 0x0c:
			return gpio->pupdr;
		case 0x14:
			return gpio->odr;
		case 0x20:
			return gpio->afrl;
		case 0x24:
			return gpio->afrh;
		case 0x10:
			// input data register
		default:
			return 0x00000000;
	}
}

void asher_peripheral_dvt1_gpio_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *userdata) {
	asher_dvt1_gpio *gpio = (asher_dvt1_gpio *)userdata;
	
	switch (offset) {
		case 0x00: {
			for (uint8_t i = 0; i < 16; i++) {
				uint64_t newValue = (value >> (i * 2)) & 0x00000003;
				uint64_t oldValue = (gpio->moder >> (i * 2)) & 0x00000003;
				
				if (newValue != oldValue) {
					char *mode = "";
					
					if (newValue == 0) {
						mode = "input";
					}
					else if (newValue == 1) {
						mode = "output";
					}
					else if (newValue == 2) {
						mode = "alternate function";
					}
					else if (newValue == 3) {
						mode = "analog";
					}
					
					printf("[debug] P%c%d mode set to %s\n", gpio->id, i, mode);
				}
			}
			
			gpio->moder = value;
			return;
		}
		case 0x04:{
			for (uint8_t i = 0; i < 16; i++) {
				uint64_t newValue = (value >> i) & 0x00000001;
				uint64_t oldValue = (gpio->otyper >> i) & 0x00000001;
				
				if (newValue != oldValue) {
					char *mode = "";
					
					if (newValue == 0) {
						mode = "push-pull";
					}
					else if (newValue == 1) {
						mode = "open-drain";
					}
					
					printf("[debug] P%c%d output type set to %s\n", gpio->id, i, mode);
				}
			}
			
			gpio->otyper = value;
			return;
		}
		case 0x08: {
			for (uint8_t i = 0; i < 16; i++) {
				uint64_t newValue = (value >> (i * 2)) & 0x00000003;
				uint64_t oldValue = (gpio->ospeedr >> (i * 2)) & 0x00000003;
				
				if (newValue != oldValue) {
					printf("[debug] P%c%d output speed set to %llu\n", gpio->id, i, newValue);
				}
			}
			
			gpio->ospeedr = value;
			return;
		}
		case 0x0c:
			gpio->pupdr = value;
			return;
		case 0x14:
			gpio->odr = value;
			return;
		case 0x18: {
			for (uint8_t i = 0; i < 16; i++) {
				if (((value >> i) & 0x00000001) != 0) {
					gpio->odr |= (1 << i);
				}
				else if (((value >> (16 + i)) & 0x00000001) != 0) {
					gpio->odr &= ~(1 << i);
				}
			}
			
			return;
		}
		case 0x20: {
			for (uint8_t i = 0; i < 8; i++) {
				uint64_t newValue = (value >> (i * 4)) & 0x0000000f;
				uint64_t oldValue = (gpio->afrl >> (i * 4)) & 0x0000000f;
				
				if (newValue != oldValue) {
					printf("[debug] P%c%d alternate function set to %llu\n", gpio->id, i, newValue);
				}
			}
			
			gpio->afrl = value;
			return;
		}
		case 0x24: {
			for (uint8_t i = 0; i < 8; i++) {
				uint64_t newValue = (value >> (i * 4)) & 0x0000000f;
				uint64_t oldValue = (gpio->afrh >> (i * 4)) & 0x0000000f;
				
				if (newValue != oldValue) {
					printf("[debug] P%c%d alternate function set to %llu\n", gpio->id, i + 8, newValue);
				}
			}
			
			gpio->afrh = value;
			return;
		}
		default:
			return;
	}
}

void asher_peripheral_dvt1_gpio_destroy(void *userdata) {
	free(userdata);
}