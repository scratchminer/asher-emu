#include <stdlib.h>

#include <unicorn/unicorn.h>

#include "device.h"
#include "peripherals.h"
#include "peripherals/peripheral.h"

#define ASHER_PERIPHERAL_REGISTER(name, addr, sz) do { \
	void *userdata = asher_peripheral_##name##_create(addr); \
	err = uc_mmio_map(uc, addr, sz, &asher_peripheral_##name##_read, userdata, &asher_peripheral_##name##_write, userdata); \
	if (err) { \
		printf("asher_peripherals_*_register: uc_mmio_map failed: %s\n", uc_strerror(err)); \
		uc_mem_unmap(uc, addr, sz); \
		asher_peripheral_##name##_destroy(userdata); \
		return false; \
	} \
	if (!asher_device_push_peripheral(device, userdata)) { \
		uc_mem_unmap(uc, addr, sz); \
		asher_peripheral_##name##_destroy(userdata); \
		return false; \
	} \
} while(0)

#define ASHER_PERIPHERAL_UNREGISTER(name, addr, sz) do { \
	void *userdata = asher_device_pop_peripheral(device); \
	if (userdata == NULL) { \
		return false; \
	} \
	uc_mem_unmap(uc, addr, sz); \
	asher_peripheral_##name##_destroy(userdata); \
} while(0)

bool asher_peripherals_dvt1_register(asher_device *device) {
	uc_engine *uc = asher_device_get_engine(device);
	uc_err err;
	
	ASHER_PERIPHERAL_REGISTER(dvt1_sysctl, 0xe000e000, 0x1000);
	
	ASHER_PERIPHERAL_REGISTER(dvt1_dma, 0x40026000, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_dma, 0x40026400, 0x400);
	
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40020000, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40020400, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40020800, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40020c00, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40021000, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40021400, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40021800, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40021c00, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40022000, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40022400, 0x400);
	ASHER_PERIPHERAL_REGISTER(dvt1_gpio, 0x40022800, 0x400);
	
	ASHER_PERIPHERAL_REGISTER(dvt1_rcc, 0x40023800, 0x400);
	
	return true;
}

bool asher_peripherals_dvt1_unregister(asher_device *device) {
	uc_engine *uc = asher_device_get_engine(device);
	
	ASHER_PERIPHERAL_UNREGISTER(dvt1_rcc, 0x40023800, 0x400);
	
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40022800, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40022400, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40022000, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40021c00, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40021800, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40021400, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40021000, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40020c00, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40020800, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40020400, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_gpio, 0x40020000, 0x400);
	
	ASHER_PERIPHERAL_UNREGISTER(dvt1_dma, 0x40026000, 0x400);
	ASHER_PERIPHERAL_UNREGISTER(dvt1_dma, 0x40026400, 0x400);
	
	ASHER_PERIPHERAL_UNREGISTER(dvt1_sysctl, 0xe000e000, 0x1000);
	
	return true;
}

bool asher_peripherals_h7d1_register(asher_device *device) {
	uc_engine *uc = asher_device_get_engine(device);
	uc_err err;
	
	// todo
	
	return true;
}

bool asher_peripherals_h7d1_unregister(asher_device *device) {
	uc_engine *uc = asher_device_get_engine(device);
	
	// todo
	
	return true;
}