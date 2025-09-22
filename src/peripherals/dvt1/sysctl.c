#include <string.h>

#if defined(__APPLE__)
#include <mach/mach_time.h>
#else
#include <time.h>
#ifdef CLOCK_MONOTONIC
#define CLOCKID CLOCK_MONOTONIC
#else
#define CLOCKID CLOCK_REALTIME
#endif
#endif

#include "../peripheral.h"

typedef struct {
	struct {
		uint32_t csr;
		uint32_t rvr;
		uint32_t cvr;
	} systick;
	struct {
		uint32_t icsr;
		uint32_t vtor;
		uint32_t shpr1;
		uint32_t shpr2;
		uint32_t shpr3;
		uint32_t shcsr;
		uint32_t cfsr;
		uint32_t hfsr;
		uint32_t mmfar;
		uint32_t bfar;
		uint32_t csselr;
		uint32_t cpacr;
	} scb;
	struct {
		uint32_t ipr[25];
	} nvic;
	struct {
		uint32_t ctrl;
		uint32_t rnr;
		uint32_t rbar[8];
		uint32_t rasr[8];
	} mpu;
} asher_dvt1_sysctl;

// from https://www.roxlu.com/2014/047/high-resolution-timer-function-in-c-c--/
static uint64_t asher_peripheral_dvt1_sysctl_counter(void) {
	static uint64_t is_init = 0;
#if defined(__APPLE__)
	static mach_timebase_info_data_t info;
	if (is_init == 0) {
		mach_timebase_info(&info);
		is_init = 1;
	}
	uint64_t now;
	now = mach_absolute_time();
	now *= info.numer;
	now /= info.denom;
	return now;
#else
	static struct timespec linux_rate;
	if (is_init == 0) {
		clock_getres(CLOCKID, &linux_rate);
		is_init = 1;
	}
	uint64_t now;
	struct timespec spec;
	clock_gettime(CLOCKID, &spec);
	now = spec.tv_sec * 1.0e9 + spec.tv_nsec;
	return now;
#endif
}

void *asher_peripheral_dvt1_sysctl_create(uint32_t baseAddr) {
	asher_dvt1_sysctl *sysctl = malloc(sizeof(asher_dvt1_sysctl));
	
	sysctl->systick.csr = 0x00000000;
	
	sysctl->scb.icsr = 0x00000000;
	sysctl->scb.vtor = 0x08000000;
	sysctl->scb.shpr1 = 0x00000000;
	sysctl->scb.shpr2 = 0x00000000;
	sysctl->scb.shpr3 = 0x00000000;
	sysctl->scb.shcsr = 0x00000000;
	sysctl->scb.cfsr = 0x00000000;
	sysctl->scb.hfsr = 0x00000000;
	sysctl->scb.mmfar = 0x00000000;
	sysctl->scb.bfar = 0x00000000;
	sysctl->scb.csselr = 0x00000000;
	
	memset(sysctl->nvic.ipr, 0, 25 * sizeof(uint32_t));
	
	sysctl->mpu.ctrl = 0x00000000;
	sysctl->mpu.rnr = 0x00000000;
	
	return sysctl;
}

uint64_t asher_peripheral_dvt1_sysctl_read(uc_engine *uc, uint64_t offset, unsigned size, void *userdata) {
	asher_dvt1_sysctl *sysctl = (asher_dvt1_sysctl *)userdata;
	
	if (offset == 0x004) {
		return 0x00000003;
	}
	else if (offset >= 0x010 && offset < 0x020) {
		// SysTick
		switch (offset - 0x010) {
			case 0x0:
				return sysctl->systick.csr;
			case 0x4:
				return sysctl->systick.rvr;
			case 0x8:
				return sysctl->systick.cvr;
			case 0xc:
				return 0x0000493e;
			default:
				return 0x00000000;
		}
	}
	else if (offset >= 0x400 && offset < 0x464) {
		// NVIC
		return sysctl->nvic.ipr[offset - 0x400];
	}
	else if (offset >= 0xd00 && offset < 0xd90) {
		// SCB
		switch (offset & 0xff) {
			case 0x00:
				return 0x411fc270;
			case 0x04:
				return sysctl->scb.icsr;
			case 0x08:
				return sysctl->scb.vtor;
			case 0x0c:
				return 0xfa050000;
			case 0x18:
				return sysctl->scb.shpr1;
			case 0x1c:
				return sysctl->scb.shpr2;
			case 0x20:
				return sysctl->scb.shpr3;
			case 0x24:
				return sysctl->scb.shcsr;
			case 0x28:
				return sysctl->scb.cfsr;
			case 0x2c:
				return sysctl->scb.hfsr;
			case 0x34:
				return sysctl->scb.mmfar;
			case 0x38:
				return sysctl->scb.bfar;
			case 0x40:
				return 0x00000030;
			case 0x44:
				return 0x00000200;
			case 0x48:
				return 0x00100000;
			case 0x50:
				return 0x00100030;
			case 0x58:
				return 0x01000000;
			case 0x60:
				return 0x01101110;
			case 0x64:
				return 0x02112000;
			case 0x68:
				return 0x20232231;
			case 0x6c:
				return 0x01111131;
			case 0x70:
				return 0x01310132;
			case 0x78:
				return 0x00000003;
			case 0x7c:
				return 0x8303c003;
			case 0x80:
				return ((sysctl->scb.csselr & 1) == 0) ? 0xf003e019 : 0xf007e009;
			case 0x84:
				return sysctl->scb.csselr;
			case 0x88:
				return sysctl->scb.cpacr;
			default:
				return 0x00000000;
		}
	}
	else if (offset >= 0xd90 && offset < 0xdec) {
		// MPU
		switch (offset & 0xff) {
			case 0x90:
				return 0x00000800;
			case 0x94:
				return sysctl->mpu.ctrl;
			case 0x98:
				return sysctl->mpu.rnr;
			case 0x9c:
			case 0xa4:
			case 0xbc:
			case 0xb4:
				return sysctl->mpu.rbar[sysctl->mpu.rnr];
			case 0xa0:
			case 0xa8:
			case 0xb0:
			case 0xb8:
				return sysctl->mpu.rasr[sysctl->mpu.rnr];
			default:
				return 0x00000000;
		}
	}
	else if (offset == 0xf90 || offset == 0xf94) {
		return 0x00000039;
	}
	else if (offset == 0xf98) {
		return 0x00000003;
	}
	else if (offset >= 0xfd0) {
		switch (offset & 0xff) {
			case 0xd0:
				return 0x00000004;
			case 0xdc:
				return 0xfa050000;
			case 0xe0:
				return 0x0000000c;
			case 0xe4:
				return 0x000000b0;
			case 0xe8:
				return 0x0000000b;
			case 0xf0:
				return 0x0000000d;
			case 0xf4:
				return 0x000000e0;
			case 0xf8:
				return 0x00000005;
			case 0xfc:
				return 0x000000b1;
			default:
				return 0x00000000;
		}
	}
	
	return 0x00000000;
}

void asher_peripheral_dvt1_sysctl_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *userdata) {
	asher_dvt1_sysctl *sysctl = (asher_dvt1_sysctl *)userdata;
	
	if (offset >= 0x010 && offset < 0x020) {
		// SysTick
		switch (offset - 0x010) {
			case 0x0:
				sysctl->systick.csr = value;
				return;
			case 0x4:
				sysctl->systick.rvr = value;
				return;
			case 0x8:
				sysctl->systick.cvr = value;
				return;
			default:
				return;
		}
	}
	else if (offset >= 0x400 && offset < 0x464) {
		// NVIC
		sysctl->nvic.ipr[offset - 0x400] = value;
	}
	else if (offset >= 0xd00 && offset < 0xd90) {
		// SCB
		switch (offset & 0xff) {
			case 0x04:
				sysctl->scb.icsr = value & 0xff000000;
				return;
			case 0x08:
				sysctl->scb.vtor = value & 0xffffff80;
				return;
			case 0x18:
				sysctl->scb.shpr1 = value;
				return;
			case 0x1c:
				sysctl->scb.shpr2 = value;
				return;
			case 0x20:
				sysctl->scb.shpr3 = value;
				return;
			case 0x24:
				sysctl->scb.shcsr = value & 0x0007fd8b;
				return;
			case 0x28:
				sysctl->scb.cfsr = value;
				return;
			case 0x2c:
				sysctl->scb.hfsr = value & 0x030fbfbb;
				return;
			case 0x34:
				sysctl->scb.mmfar = value;
				return;
			case 0x38:
				sysctl->scb.bfar = value;
				return;
			case 0x84:
				sysctl->scb.csselr = value & 0x00000001;
				return;
			case 0x88:
				sysctl->scb.cpacr = value & 0x00f0ffff;
				return;
			default:
				return;
		}
	}
	else if (offset >= 0xd90 && offset < 0xdec) {
		// MPU
		switch (offset & 0xff) {
			case 0x94:
				sysctl->mpu.ctrl = value & 0x00000007;
				return;
			case 0x98:
				sysctl->mpu.rnr = value & 0x00000007;
				return;
			case 0x9c:
			case 0xa4:
			case 0xbc:
			case 0xb4:
				sysctl->mpu.rbar[sysctl->mpu.rnr] = value;
				return;
			case 0xa0:
			case 0xa8:
			case 0xb0:
			case 0xb8:
				sysctl->mpu.rasr[sysctl->mpu.rnr] = value;
				return;
			default:
				return;
		}
	}
}

void asher_peripheral_dvt1_sysctl_destroy(void *userdata) {
	free(userdata);
}