#ifndef ASHER_DEVICE_H
#define ASHER_DEVICE_H

#include <stdbool.h>
#include <stdint.h>

#include <unicorn/unicorn.h>

typedef enum {
	ASHER_DEVICE_SOFTWARE = 0,
	ASHER_DEVICE_DVT1 = 1,
	ASHER_DEVICE_H7D1 = 2
} asher_device_type;

typedef struct asher_device asher_device;
typedef struct asher_peripheral asher_peripheral;

asher_device *asher_device_create(asher_device_type deviceType);

uc_engine *asher_device_get_engine(asher_device *device);
asher_peripheral *asher_device_push_peripheral(asher_device *device, const char *name, uint32_t addr, uint32_t size, void *userdata, void (*reset)(void *userdata), void (*destroy)(void *userdata));
asher_peripheral *asher_device_pop_peripheral(asher_device *device);
asher_peripheral *asher_device_get_peripheral(asher_device *device, const char *name);

bool asher_device_load_boot(asher_device *device, const char *bootPath);
bool asher_device_load_pdfw(asher_device *device, const char *pdfwPath);

// Equivalent to pressing the soft-reset button inside the crank dock -- resets the emulated MCU if there is one
void asher_device_reset(asher_device *device);

bool asher_device_step(asher_device *device);
bool asher_device_run(asher_device *device);

void asher_device_destroy(asher_device *device);

#endif
