#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <md5.h>
#include <unicorn/unicorn.h>

#include "device.h"
#include "peripherals.h"
#include "timer.h"

struct asher_device {
	asher_device_type type;
	uc_engine *uc;
	
	asher_peripheral peripherals[255];
	uint8_t numPeripherals;
	
	uint8_t bootData[0x18000];
	uint8_t pdfwData[0xf0000];
	
	uint64_t lastTick;
};

static bool unmappedCb(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *userdata) {
	if (type == UC_MEM_READ_UNMAPPED || type == UC_MEM_FETCH_UNMAPPED) {
		printf("RD%02d 0x%08llx\n", size * 8, address);
	}
	else if (type == UC_MEM_WRITE_UNMAPPED) {
		printf("WR%02d 0x%08llx -> 0x%08llx\n", size * 8, value, address);
	}
	
	return false;
}

static void tickCb(uc_engine *uc, uint64_t address, uint32_t size, void *userdata) {
	asher_device *device = (asher_device *)userdata;
	
	uint64_t tick = asher_timer();
	double freq = asher_peripheral_dvt1_rcc_get_freq(asher_device_get_peripheral(device, "RCC"));
	
	uint64_t cycles = (uint64_t)(freq / 1000.0 * (tick - device->lastTick));
	
	for (uint8_t i = 0; i < device->numPeripherals; i++) {
		asher_peripheral *periph = &device->peripherals[i];
		if (periph->tick != NULL) {
			periph->tick(uc, periph, cycles);
		}
	}
	
	device->lastTick = tick;
}

asher_device *asher_device_create(asher_device_type deviceType) {
	asher_device *device = malloc(sizeof(asher_device));
	
	device->type = deviceType;
	device->numPeripherals = 0;
	
	if (deviceType == ASHER_DEVICE_SOFTWARE) {
		// todo
	}
	else if (deviceType == ASHER_DEVICE_DVT1) {
		uc_err err = uc_open(UC_ARCH_ARM, UC_MODE_THUMB, &device->uc);
		if (err) {
			printf("asher_device_create: uc_open failed: %s\n", uc_strerror(err));
			free(device);
			return NULL;
		}
		
		err = uc_ctl_set_cpu_model(device->uc, UC_CPU_ARM_CORTEX_M7);
		if (err) {
			printf("asher_device_create: uc_ctl_set_cpu_model failed: %s\n", uc_strerror(err));
			uc_close(device->uc);
			free(device);
			return NULL;
		}
		
		err = uc_mem_map(device->uc, 0x00000000, 0x00010000, UC_PROT_ALL);
		if (err) {
			printf("asher_device_create: uc_mem_map (DVT1 ITCM) failed: %s\n", uc_strerror(err));
			uc_close(device->uc);
			free(device);
			return NULL;
		}
		
		err = uc_mem_map(device->uc, 0x20000000, 0x00010000, UC_PROT_ALL);
		if (err) {
			printf("asher_device_create: uc_mem_map (DVT1 DTCM) failed: %s\n", uc_strerror(err));
			uc_close(device->uc);
			free(device);
			return NULL;
		}
		
		err = uc_mem_map(device->uc, 0x20010000, 0x00050000, UC_PROT_ALL);
		if (err) {
			printf("asher_device_create: uc_mem_map (DVT1 AHB SRAM) failed: %s\n", uc_strerror(err));
			uc_close(device->uc);
			free(device);
			return NULL;
		}
		
		uc_hook hh;
		uc_hook_add(device->uc, &hh, UC_HOOK_MEM_UNMAPPED, &unmappedCb, NULL, 0x00000000, 0xffffffff);
		uc_hook_add(device->uc, &hh, UC_HOOK_CODE, &tickCb, device, 0x00000000, 0xffffffff);
		
		asher_peripherals_dvt1_register(device);
	}
	else if (deviceType == ASHER_DEVICE_H7D1) {
		uc_err err = uc_open(UC_ARCH_ARM, UC_MODE_THUMB, &device->uc);
		if (err) {
			printf("asher_device_create: uc_open failed: %s\n", uc_strerror(err));
			free(device);
			return NULL;
		}
		
		err = uc_ctl_set_cpu_model(device->uc, UC_CPU_ARM_CORTEX_M7);
		if (err) {
			printf("asher_device_create: uc_ctl_set_cpu_model failed: %s\n", uc_strerror(err));
			uc_close(device->uc);
			free(device);
			return NULL;
		}
		
		err = uc_mem_map(device->uc, 0x00000000, 0x00010000, UC_PROT_ALL);
		if (err) {
			printf("asher_device_create: uc_mem_map (H7D1 ITCM) failed: %s\n", uc_strerror(err));
			uc_close(device->uc);
			free(device);
			return NULL;
		}
		
		err = uc_mem_map(device->uc, 0x20000000, 0x00010000, UC_PROT_ALL);
		if (err) {
			printf("asher_device_create: uc_mem_map (H7D1 DTCM) failed: %s\n", uc_strerror(err));
			uc_close(device->uc);
			free(device);
			return NULL;
		}
		
		err = uc_mem_map(device->uc, 0x24000000, 0x00100000, UC_PROT_ALL);
		if (err) {
			printf("asher_device_create: uc_mem_map (H7D1 AXI SRAM) failed: %s\n", uc_strerror(err));
			uc_close(device->uc);
			return false;
		}
		
		err = uc_mem_map(device->uc, 0x30000000, 0x00020000, UC_PROT_ALL);
		if (err) {
			printf("asher_device_create: uc_mem_map (H7D1 AHB SRAM) failed: %s\n", uc_strerror(err));
			uc_close(device->uc);
			free(device);
			return NULL;
		}
		
		uc_hook hh;
		uc_hook_add(device->uc, &hh, UC_HOOK_MEM_UNMAPPED, &unmappedCb, NULL, 0x00000000, 0xffffffff);
		uc_hook_add(device->uc, &hh, UC_HOOK_CODE, &tickCb, device, 0x00000000, 0xffffffff);
		
		asher_peripherals_h7d1_register(device);
	}
	
	return device;
}

uc_engine *asher_device_get_engine(asher_device *device) {
	return device->uc;
}

asher_peripheral *asher_device_push_peripheral(asher_device *device, const char *name, uint32_t addr, uint32_t size, void *userdata, void (*reset)(void *userdata), void (*tick)(uc_engine *uc, asher_peripheral *periph, uint64_t cycles), void (*destroy)(void *userdata)) {
	if (device->numPeripherals == 255) {
		printf("asher_device_push_peripheral: peripheral stack full\n");
		return NULL;
	}
	
	device->peripherals[device->numPeripherals].name = name;
	device->peripherals[device->numPeripherals].addr = addr;
	device->peripherals[device->numPeripherals].size = size;
	device->peripherals[device->numPeripherals].userdata = userdata;
	device->peripherals[device->numPeripherals].device = device;
	device->peripherals[device->numPeripherals].reset = reset;
	device->peripherals[device->numPeripherals].tick = tick;
	device->peripherals[device->numPeripherals].destroy = destroy;
	
	return &device->peripherals[device->numPeripherals++];
}

asher_peripheral *asher_device_pop_peripheral(asher_device *device) {
	if (device->numPeripherals == 0) {
		return NULL;
	}
	
	return &device->peripherals[--device->numPeripherals];
}

asher_peripheral *asher_device_get_peripheral(asher_device *device, const char *name) {
	for (uint8_t i = 0; i < device->numPeripherals; i++) {
		asher_peripheral *periph = &device->peripherals[i];
		
		if (strcmp(periph->name, name) == 0) {
			return periph;
		}
	}
	
	return NULL;
}

bool asher_device_load_boot(asher_device *device, const char *bootPath) {
	if (device->type == ASHER_DEVICE_SOFTWARE) {
		return false;
	}
	
	FILE *boot = fopen(bootPath, "r");
	
	if (boot == NULL) {
		printf("asher_device_load_boot: fopen failed: %s\n", strerror(errno));
		return false;
	}
	
	char buf[5];
	buf[4] = '\0';
	const char *errStr = "";
	
	if (fread(buf, sizeof(char), 4, boot) < 4 * sizeof(char)) {
		if (feof(boot)) {
			errStr = "EOF reached";
		}
		else if (ferror(boot)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_boot: fread (magic number) failed: %s\n", errStr);
		return false;
	}
	if (strcmp(buf, "PDBL") != 0) {
		printf("asher_device_load_boot: invalid file\n");
		return false;
	}
	
	if (fread(buf, sizeof(char), 4, boot) < 4 * sizeof(char)) {
		if (feof(boot)) {
			errStr = "EOF reached";
		}
		else if (ferror(boot)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_boot: fread (device version) failed: %s\n", errStr);
		return false;
	}
	
	if (device->type == ASHER_DEVICE_DVT1) {
		if (strcmp(buf, "dvt1") != 0) {
			printf("asher_device_load_boot: wrong device version (expected 'dvt1')\n");
			return false;
		}
	}
	else if (device->type == ASHER_DEVICE_H7D1) {
		if (strcmp(buf, "h7d1") != 0) {
			printf("asher_device_load_boot: wrong device version (expected 'h7d1')\n");
			return false;
		}
	}
	
	uint32_t fileLength;
	
	if (fread(&fileLength, sizeof(uint32_t), 1, boot) < 1) {
		if (feof(boot)) {
			errStr = "EOF reached";
		}
		else if (ferror(boot)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_boot: fread (file length) failed: %s\n", errStr);
		return false;
	}
	
	uint32_t stamp;
	
	if (fread(&stamp, sizeof(uint32_t), 1, boot) < 1) {
		if (feof(boot)) {
			errStr = "EOF reached";
		}
		else if (ferror(boot)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_boot: fread (build timestamp) failed: %s\n", errStr);
		return false;
	}
	
	uint8_t commit[8];
	
	if (fread(commit, sizeof(uint8_t), 8, boot) < 8) {
		if (feof(boot)) {
			errStr = "EOF reached";
		}
		else if (ferror(boot)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_boot: fread (Git commit hash) failed: %s\n", errStr);
		return false;
	}
	
	uint8_t md5Half[8];
	
	if (fread(md5Half, sizeof(uint8_t), 8, boot) < 8) {
		if (feof(boot)) {
			errStr = "EOF reached";
		}
		else if (ferror(boot)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_boot: fread (MD5 hash) failed: %s\n", errStr);
		return false;
	}
	
	if (fread(device->bootData, sizeof(uint8_t), fileLength, boot) < fileLength) {
		if (feof(boot)) {
			errStr = "EOF reached";
		}
		else if (ferror(boot)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_boot: fread (bootloader data) failed: %s\n", errStr);
		return false;
	}
	
	MD5Context md5Ctx;
	
	md5_Init(&md5Ctx);
	md5_Update(&md5Ctx, device->bootData, fileLength * sizeof(uint8_t));
	md5_Finalize(&md5Ctx);
	
	if (memcmp(md5Half, md5Ctx.digest, 8) != 0) {
		printf("asher_device_load_boot: MD5 hash mismatch\n");
		return false;
	}
	
	time_t timer = (time_t)stamp;
	char timeStr[256];
	
	strftime(timeStr, 256, "%c", gmtime(&timer));
	printf("Bootloader build timestamp (UTC): %s\n", timeStr);
	printf("Bootloader Git commit hash: %02x%02x%02x%02x%02x%02x%02x%02x\n", commit[0], commit[1], commit[2], commit[3], commit[4], commit[5], commit[6], commit[7]);
	
	uc_err err;
	
	if (device->type == ASHER_DEVICE_DVT1) {
		err = uc_mem_map_ptr(device->uc, 0x08000000, 0x00010000, UC_PROT_ALL, device->bootData);
		
		if (err) {
			printf("asher_device_load_boot: uc_mem_map_ptr failed: %s\n", uc_strerror(err));
			return false;
		}
	}
	else if (device->type == ASHER_DEVICE_H7D1) {
		err = uc_mem_map_ptr(device->uc, 0x08000000, 0x00018000, UC_PROT_ALL, device->bootData);
		
		if (err) {
			printf("asher_device_load_boot: uc_mem_map_ptr failed: %s\n", uc_strerror(err));
			return false;
		}
	}
	
	return true;
}

bool asher_device_load_pdfw(asher_device *device, const char *pdfwPath) {
	if (device->type == ASHER_DEVICE_SOFTWARE) {
		return false;
	}
	
	FILE *pdfw = fopen(pdfwPath, "r");
	
	if (pdfw == NULL) {
		printf("asher_device_load_pdfw: fopen failed: %s\n", strerror(errno));
		return false;
	}
	
	char buf[5];
	buf[4] = '\0';
	const char *errStr = "";
	
	if (fread(buf, sizeof(char), 4, pdfw) < 4) {
		if (feof(pdfw)) {
			errStr = "EOF reached";
		}
		else if (ferror(pdfw)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_pdfw: fread (magic number) failed: %s\n", errStr);
		return false;
	}
	if (strcmp(buf, "PDFW") != 0) {
		printf("asher_device_load_pdfw: invalid file\n");
		return false;
	}
	
	if (fread(buf, sizeof(char), 4, pdfw) < 4) {
		if (feof(pdfw)) {
			errStr = "EOF reached";
		}
		else if (ferror(pdfw)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_boot: fread (device version) failed: %s\n", errStr);
		return false;
	}
	
	if (device->type == ASHER_DEVICE_DVT1) {
		if (strcmp(buf, "dvt1") != 0) {
			printf("asher_device_load_pdfw: wrong device version (expected 'dvt1')\n");
			return false;
		}
	}
	else if (device->type == ASHER_DEVICE_H7D1) {
		if (strcmp(buf, "h7d1") != 0) {
			printf("asher_device_load_pdfw: wrong device version (expected 'h7d1')\n");
			return false;
		}
	}
	
	uint32_t fileLength;
	
	if (fread(&fileLength, sizeof(uint32_t), 1, pdfw) < 1) {
		if (feof(pdfw)) {
			errStr = "EOF reached";
		}
		else if (ferror(pdfw)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_pdfw: fread (file length) failed: %s\n", errStr);
		return false;
	}
	
	uint32_t stamp;
	
	if (fread(&stamp, sizeof(uint32_t), 1, pdfw) < 1) {
		if (feof(pdfw)) {
			errStr = "EOF reached";
		}
		else if (ferror(pdfw)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_pdfw: fread (build timestamp) failed: %s\n", errStr);
		return false;
	}
	
	uint8_t commit[8];
	
	if (fread(commit, sizeof(uint8_t), 8, pdfw) < 8) {
		if (feof(pdfw)) {
			errStr = "EOF reached";
		}
		else if (ferror(pdfw)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_pdfw: fread (Git commit hash) failed: %s\n", errStr);
		return false;
	}
	
	uint8_t md5Half[8];
	
	if (fread(md5Half, sizeof(uint8_t), 8, pdfw) < 8) {
		if (feof(pdfw)) {
			errStr = "EOF reached";
		}
		else if (ferror(pdfw)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_pdfw: fread (MD5 hash) failed: %s\n", errStr);
		return false;
	}
	
	if (fread(device->pdfwData, sizeof(uint8_t), fileLength, pdfw) < fileLength) {
		if (feof(pdfw)) {
			errStr = "EOF reached";
		}
		else if (ferror(pdfw)) {
			errStr = strerror(errno);
		}
		
		printf("asher_device_load_pdfw: fread (firmware data) failed: %s\n", errStr);
		return false;
	}
	
	MD5Context md5Ctx;
	
	md5_Init(&md5Ctx);
	md5_Update(&md5Ctx, device->pdfwData, fileLength * sizeof(uint8_t));
	md5_Finalize(&md5Ctx);
	
	if (memcmp(md5Half, md5Ctx.digest, 8) != 0) {
		printf("asher_device_load_pdfw: MD5 hash mismatch\n");
		return false;
	}
	
	time_t timer = (time_t)stamp;
	char timeStr[256];
	
	strftime(timeStr, 256, "%c", gmtime(&timer));
	printf("Firmware build timestamp (UTC): %s\n", timeStr);
	printf("Firmware Git commit hash: %02x%02x%02x%02x%02x%02x%02x%02x\n", commit[0], commit[1], commit[2], commit[3], commit[4], commit[5], commit[6], commit[7]);
	
	uc_err err;
	
	if (device->type == ASHER_DEVICE_DVT1) {
		err = uc_mem_map_ptr(device->uc, 0x08010000, 0x000f0000, UC_PROT_ALL, device->pdfwData);
		
		if (err) {
			printf("asher_device_load_pdfw: uc_mem_map_ptr failed: %s\n", uc_strerror(err));
			return false;
		}
	}
	else if (device->type == ASHER_DEVICE_H7D1) {
		// todo
		
		return false;
	}
	
	return true;
}

void asher_device_reset(asher_device *device) {
	if (device->type == ASHER_DEVICE_DVT1) {
		for (uint8_t i = 0; i < device->numPeripherals; i++) {
			asher_peripheral *periph = &device->peripherals[i];
			periph->reset(periph->userdata);
		}
		
		uint32_t *vectorTablePtr = (uint32_t *)(device->bootData);
		
		uc_reg_write(device->uc, UC_ARM_REG_SP, vectorTablePtr++);
		uc_reg_write(device->uc, UC_ARM_REG_PC, vectorTablePtr);
	}
	else if (device->type == ASHER_DEVICE_H7D1) {
		for (uint8_t i = 0; i < device->numPeripherals; i++) {
			asher_peripheral *periph = &device->peripherals[i];
			periph->reset(periph->userdata);
		}
		
		uint32_t *vectorTablePtr = (uint32_t *)(device->bootData);
		
		uc_reg_write(device->uc, UC_ARM_REG_SP, vectorTablePtr++);
		uc_reg_write(device->uc, UC_ARM_REG_PC, vectorTablePtr);
	}
}

bool asher_device_step(asher_device *device) {
	uint32_t pc;
	uc_reg_read(device->uc, UC_ARM_REG_PC, &pc);
	
	device->lastTick = asher_timer();
	uc_err err = uc_emu_start(device->uc, pc | 1, 0xffffffffUL, 10000000UL, 1);
	
	if (err) {
		printf("asher_device_step: uc_emu_start failed: %s\n", uc_strerror(err));
		return false;
	}
	
	return true;
}

bool asher_device_run(asher_device *device) {
	uint32_t pc;
	uc_reg_read(device->uc, UC_ARM_REG_PC, &pc);
	
	device->lastTick = asher_timer();
	
	for (;;) {
		uc_reg_read(device->uc, UC_ARM_REG_PC, &pc);
		uc_err err = uc_emu_start(device->uc, pc | 1, 0xffffffffUL, 0, 0);
		
		if (err) {
			uc_reg_read(device->uc, UC_ARM_REG_PC, &pc);
			
			if (err == UC_ERR_EXCEPTION && (pc & 0xf0000000) == 0xf0000000) {
				if (device->type == ASHER_DEVICE_DVT1) {
					if (asher_peripheral_dvt1_sysctl_nvic_return(device->uc, asher_device_get_peripheral(device, "SCB"), pc)) {
						continue;
					}
				}
				else if (device->type == ASHER_DEVICE_H7D1) {
					/*if (asher_peripheral_h7d1_sysctl_nvic_return(device->uc, asher_device_get_peripheral(device, "SCB"), pc)) {
						continue;
					}*/
				}
			}
			
			printf("asher_device_run: uc_emu_start failed: %s\n", uc_strerror(err));
			return false;
		}
		else {
			break;
		}
	}
	
	return true;
}

void asher_device_destroy(asher_device *device) {
	uc_close(device->uc);
	free(device);
}
