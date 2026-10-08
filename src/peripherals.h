#ifndef ASHER_PERIPHERALS_H
#define ASHER_PERIPHERALS_H

#include <stdbool.h>
#include <stdint.h>
#include <unicorn/unicorn.h>

#include "device.h"

struct asher_peripheral {
	const char *name;
	uint32_t addr;
	uint32_t size;
	void *userdata;
	asher_device *device;
	void (*reset)(void *userdata);
	void (*tick)(uc_engine *uc, asher_peripheral *periph, uint64_t cycles);
	void (*destroy)(void *userdata);
};

double asher_peripheral_dvt1_rcc_get_freq(asher_peripheral *periph);
double asher_peripheral_h7d1_rcc_get_freq(asher_peripheral *periph);

bool asher_peripheral_dvt1_sysctl_nvic_return(uc_engine *uc, asher_peripheral *periph, uint32_t excReturn);
bool asher_peripheral_h7d1_sysctl_nvic_return(uc_engine *uc, asher_peripheral *periph, uint32_t excReturn);

bool asher_peripherals_dvt1_register(asher_device *device);
bool asher_peripherals_dvt1_unregister(asher_device *device);

bool asher_peripherals_h7d1_register(asher_device *device);
bool asher_peripherals_h7d1_unregister(asher_device *device);

#endif
