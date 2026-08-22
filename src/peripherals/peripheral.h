#ifndef ASHER_PERIPHERALS_PERIPHERAL_H
#define ASHER_PERIPHERALS_PERIPHERAL_H

#include <stdbool.h>
#include <stdint.h>

#include <unicorn/unicorn.h>

#include "../peripherals.h"

#define ASHER_DEFINE_PERIPHERAL(name) \
	void *asher_peripheral_##name##_create(uint32_t baseAddr); \
	void asher_peripheral_##name##_reset(void *userdata); \
	uint64_t asher_peripheral_##name##_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph); \
	void asher_peripheral_##name##_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph); \
	void asher_peripheral_##name##_destroy(void *userdata)

ASHER_DEFINE_PERIPHERAL(dummy);

ASHER_DEFINE_PERIPHERAL(dvt1_dma);
ASHER_DEFINE_PERIPHERAL(dvt1_flash);
ASHER_DEFINE_PERIPHERAL(dvt1_gpio);
ASHER_DEFINE_PERIPHERAL(dvt1_pwr);
ASHER_DEFINE_PERIPHERAL(dvt1_rcc);
ASHER_DEFINE_PERIPHERAL(dvt1_syscfg);
ASHER_DEFINE_PERIPHERAL(dvt1_sysctl);
ASHER_DEFINE_PERIPHERAL(dvt1_tim);
// todo

#undef ASHER_DEFINE_PERIPHERAL

void asher_peripheral_dvt1_sysctl_nvic_set_pending(uc_engine *uc, asher_peripheral *periph, uint8_t exceptionNum, bool pending);
void asher_peripheral_h7d1_sysctl_nvic_set_pending(uc_engine *uc, asher_peripheral *periph, uint8_t exceptionNum, bool pending);

#endif
