#include <stdbool.h>
#include <stdlib.h>

#include <unicorn/unicorn.h>

#include "device.h"
#include "peripherals.h"
#include "peripherals/peripheral.h"

#define ASHER_PERIPHERAL_REGISTER(name, id, addr, sz) do { \
	void *userdata = asher_peripheral_##id##_create(addr); \
	asher_peripheral *periph = asher_device_push_peripheral(device, name, addr, sz, userdata, &asher_peripheral_##id##_destroy); \
	if (periph == NULL) { \
		asher_peripheral_##id##_destroy(userdata); \
		return false; \
	} \
	err = uc_mmio_map(uc, addr, sz, &asher_peripheral_##id##_read, periph, &asher_peripheral_##id##_write, periph); \
	if (err) { \
		printf("asher_peripherals_*_register: uc_mmio_map failed: %s\n", uc_strerror(err)); \
		asher_peripheral_##id##_destroy(userdata); \
		asher_device_pop_peripheral(device); \
		return false; \
	} \
} while(0)

bool asher_peripherals_dvt1_register(asher_device *device) {
	uc_engine *uc = asher_device_get_engine(device);
	uc_err err;
	
	ASHER_PERIPHERAL_REGISTER("SCB", dvt1_sysctl, 0xe000e000, 0x1000);
	
	ASHER_PERIPHERAL_REGISTER("DMA1", dvt1_dma, 0x40026000, 0x400);
	ASHER_PERIPHERAL_REGISTER("DMA2", dvt1_dma, 0x40026400, 0x400);
	
	ASHER_PERIPHERAL_REGISTER("RCC", dvt1_rcc, 0x40023800, 0x400);
	
	ASHER_PERIPHERAL_REGISTER("GPIOA", dvt1_gpio, 0x40020000, 0x400);
	ASHER_PERIPHERAL_REGISTER("GPIOB", dvt1_gpio, 0x40020400, 0x400);
	ASHER_PERIPHERAL_REGISTER("GPIOC", dvt1_gpio, 0x40020800, 0x400);
	ASHER_PERIPHERAL_REGISTER("GPIOD", dvt1_gpio, 0x40020c00, 0x400);
	ASHER_PERIPHERAL_REGISTER("GPIOE", dvt1_gpio, 0x40021000, 0x400);
	ASHER_PERIPHERAL_REGISTER("GPIOF", dvt1_gpio, 0x40021400, 0x400);
	ASHER_PERIPHERAL_REGISTER("GPIOG", dvt1_gpio, 0x40021800, 0x400);
	ASHER_PERIPHERAL_REGISTER("GPIOH", dvt1_gpio, 0x40021c00, 0x400);
	ASHER_PERIPHERAL_REGISTER("GPIOI", dvt1_gpio, 0x40022000, 0x400);
	ASHER_PERIPHERAL_REGISTER("GPIOJ", dvt1_gpio, 0x40022400, 0x400);
	ASHER_PERIPHERAL_REGISTER("GPIOK", dvt1_gpio, 0x40022800, 0x400);
	
	// BKPSRAM
	uc_mem_map(uc, 0x40024000, 0x400, UC_PROT_READ | UC_PROT_WRITE);
	
	ASHER_PERIPHERAL_REGISTER("TIM11", dvt1_tim, 0x40014800, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM10", dvt1_tim, 0x40014400, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM9", dvt1_tim, 0x40014000, 0x400);
	
	ASHER_PERIPHERAL_REGISTER("SYSCFG", dvt1_syscfg, 0x40013800, 0x400);
	
	ASHER_PERIPHERAL_REGISTER("TIM8", dvt1_tim, 0x40010400, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM1", dvt1_tim, 0x40010000, 0x400);
	
	ASHER_PERIPHERAL_REGISTER("PWR", dvt1_pwr, 0x40007000, 0x400);
	
	ASHER_PERIPHERAL_REGISTER("TIM14", dvt1_tim, 0x40002000, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM13", dvt1_tim, 0x40001c00, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM12", dvt1_tim, 0x40001800, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM7", dvt1_tim, 0x40001400, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM6", dvt1_tim, 0x40001000, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM5", dvt1_tim, 0x40000c00, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM4", dvt1_tim, 0x40000800, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM3", dvt1_tim, 0x40000400, 0x400);
	ASHER_PERIPHERAL_REGISTER("TIM2", dvt1_tim, 0x40000000, 0x400);
	
	return true;
}

bool asher_peripherals_dvt1_unregister(asher_device *device) {
	uc_engine *uc = asher_device_get_engine(device);
	
	for (;;) {
		asher_peripheral *periph = asher_device_pop_peripheral(device);
		if (periph == NULL) {
			return true;
		}
		uc_mem_unmap(uc, periph->addr, periph->size);
		periph->destroy(periph->userdata);
	}
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