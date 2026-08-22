#ifndef ASHER_PERIPHERALS_H
#define ASHER_PERIPHERALS_H

#include <stdbool.h>
#include <unicorn/unicorn.h>

#include "device.h"

struct asher_peripheral {
	const char *name;
	uint32_t addr;
	uint32_t size;
	void *userdata;
	asher_device *device;
	void (*reset)(void *userdata);
	void (*destroy)(void *userdata);
};

bool asher_peripherals_dvt1_register(asher_device *device);
bool asher_peripherals_dvt1_unregister(asher_device *device);

bool asher_peripherals_h7d1_register(asher_device *device);
bool asher_peripherals_h7d1_unregister(asher_device *device);

#endif
