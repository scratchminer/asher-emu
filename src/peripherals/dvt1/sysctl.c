#include <string.h>

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
		uint32_t aircr;
		uint32_t shpr[4];
		uint32_t shcsr;
		uint32_t cfsr;
		uint32_t hfsr;
		uint32_t mmfar;
		uint32_t bfar;
		uint32_t csselr;
		uint32_t cpacr;
		uint32_t fpccr;
		uint32_t fpcar;
		uint32_t fpdscr;
	} scb;
	struct {
		uint32_t iser[3];
		uint32_t ispr[3];
		uint32_t iabr[3];
		uint32_t ipr[25];
	} nvic;
	struct {
		uint32_t ctrl;
		uint32_t rnr;
		uint32_t rbar[8];
		uint32_t rasr[8];
	} mpu;
} asher_dvt1_sysctl;

void *asher_peripheral_dvt1_sysctl_create(uint32_t baseAddr) {
	asher_dvt1_sysctl *sysctl = malloc(sizeof(asher_dvt1_sysctl));
	
	asher_peripheral_dvt1_sysctl_reset(sysctl);
	return sysctl;
}

void asher_peripheral_dvt1_sysctl_reset(void *userdata) {
	asher_dvt1_sysctl *sysctl = (asher_dvt1_sysctl *)userdata;
	
	sysctl->systick.csr = 0x00000000;
	
	sysctl->scb.icsr = 0x00000000;
	sysctl->scb.aircr = 0xfa050000;
	sysctl->scb.vtor = 0x08000000;
	sysctl->scb.shcsr = 0x00000000;
	sysctl->scb.cfsr = 0x00000000;
	sysctl->scb.hfsr = 0x00000000;
	sysctl->scb.mmfar = 0x00000000;
	sysctl->scb.bfar = 0x00000000;
	sysctl->scb.csselr = 0x00000000;
	sysctl->scb.fpdscr = 0x00000000;
	memset(sysctl->scb.shpr, 0, 4 * sizeof(uint32_t));
	
	memset(sysctl->nvic.iser, 0, 3 * sizeof(uint32_t));
	memset(sysctl->nvic.ispr, 0, 3 * sizeof(uint32_t));
	memset(sysctl->nvic.iabr, 0, 3 * sizeof(uint32_t));
	memset(sysctl->nvic.ipr, 0, 25 * sizeof(uint32_t));
	
	sysctl->mpu.ctrl = 0x00000000;
	sysctl->mpu.rnr = 0x00000000;
}

uint64_t asher_peripheral_dvt1_sysctl_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_sysctl *sysctl = (asher_dvt1_sysctl *)(((asher_peripheral *)periph)->userdata);
	
	if (offset == 0x004) {
		return 0x00000003;
	}
	else if (offset >= 0x010 && offset < 0x020) {
		// SysTick
		switch (offset - 0x010) {
			case 0x0: {
				uint32_t temp = sysctl->systick.csr;
				sysctl->systick.csr &= 0xfffeffff;
				return temp;
			}
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
	else if (offset >= 0x100 && offset < 0x464) {
		// NVIC
		if (offset >= 0x100 && offset < 0x10c) {
			return sysctl->nvic.iser[(offset - 0x100) >> 2];
		}
		else if (offset >= 0x180 && offset < 0x18c) {
			return sysctl->nvic.iser[(offset - 0x180) >> 2];
		}
		if (offset >= 0x200 && offset < 0x20c) {
			return sysctl->nvic.ispr[(offset - 0x200) >> 2];
		}
		else if (offset >= 0x280 && offset < 0x28c) {
			return sysctl->nvic.ispr[(offset - 0x280) >> 2];
		}
		else if (offset >= 0x300 && offset < 0x30c) {
			return sysctl->nvic.iabr[(offset - 0x300) >> 2];
		}
		else if (offset >= 0x400) {
			return sysctl->nvic.ipr[(offset - 0x400) >> 2];
		}
		
		return 0x00000000;
	}
	else if (offset >= 0xd00 && offset < 0xd90) {
		// SCB
		switch (offset & 0xff) {
			case 0x00:
				return 0x411fc270;
			case 0x04: {
				uint32_t ipsr;
				uc_reg_read(uc, UC_ARM_REG_IPSR, &ipsr);
				return sysctl->scb.icsr | (ipsr & 0x1ff);
			}
			case 0x08:
				return sysctl->scb.vtor;
			case 0x0c:
				return sysctl->scb.aircr;
			case 0x18:
				return sysctl->scb.shpr[1];
			case 0x1c:
				return sysctl->scb.shpr[2];
			case 0x20:
				return sysctl->scb.shpr[3];
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
	else if (offset >= 0xf34 && offset < 0xf48) {
		// SCB FPU
		switch (offset & 0xff) {
			case 0x34:
				return sysctl->scb.fpccr;
			case 0x38:
				return sysctl->scb.fpcar;
			case 0x3c:
				return sysctl->scb.fpdscr;
			case 0x40:
				return 0x10110021;
			case 0x44:
				return 0x11000011;
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

void asher_peripheral_dvt1_sysctl_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_sysctl *sysctl = (asher_dvt1_sysctl *)(((asher_peripheral *)periph)->userdata);
	
	if (offset >= 0x010 && offset < 0x020) {
		// SysTick
		switch (offset - 0x010) {
			case 0x0:
				sysctl->systick.csr = value & 0x00010007;
				return;
			case 0x4:
				sysctl->systick.rvr = value & 0x00ffffff;
				return;
			case 0x8:
				sysctl->systick.cvr = 0x00000000;
				sysctl->systick.csr &= 0xfffeffff;
				return;
			default:
				return;
		}
	}
	else if (offset >= 0x100 && offset < 0x464) {
		// NVIC
		if (offset >= 0x100 && offset < 0x10c) {
			if (offset == 0x10c) {
				value &= 0x00000003;
			}
			
			sysctl->nvic.iser[(offset - 0x100) >> 2] |= value;
			return;
		}
		else if (offset >= 0x180 && offset < 0x18c) {
			if (offset == 0x18c) {
				value &= 0x00000003;
			}
			
			sysctl->nvic.iser[(offset - 0x180) >> 2] &= 0xffffffff ^ value;
			return;
		}
		if (offset >= 0x200 && offset < 0x20c) {
			if (offset == 0x20c) {
				value &= 0x00000003;
			}
			
			uint16_t n = (offset - 0x200) >> 2;
			
			for (uint8_t m = 0; m < 32; m++) {
				if (((value >> m) & 0x00000001) == 0x00000001 && ((sysctl->nvic.ispr[n] >> m) & 0x00000001) == 0x00000000) {
					asher_peripheral_dvt1_sysctl_nvic_set_pending(uc, periph, ((n + 1) << 5) | m, true);
				}
			}
			return;
		}
		else if (offset >= 0x280 && offset < 0x28c) {
			if (offset == 0x20c) {
				value &= 0x00000003;
			}
			
			uint16_t n = (offset - 0x200) >> 2;
			
			for (uint8_t m = 0; m < 32; m++) {
				if (((value >> m) & 0x00000001) == 0x00000001 && ((sysctl->nvic.ispr[n] >> m) & 0x00000001) == 0x00000001) {
					asher_peripheral_dvt1_sysctl_nvic_set_pending(uc, periph, ((n + 1) << 5) | m, false);
				}
			}
			return;
		}
		else if (offset >= 0x400) {
			if (offset == 0x460) {
				value &= 0x0000ffff;
			}
			
			sysctl->nvic.ipr[(offset - 0x400) >> 2] = value;
			return;
		}
		
		return;
	}
	else if (offset >= 0xd00 && offset < 0xd90) {
		// SCB
		switch (offset & 0xff) {
			case 0x04:
				sysctl->scb.icsr = value & 0x9e000000;
				return;
			case 0x08:
				sysctl->scb.vtor = value & 0xffffff80;
				return;
			case 0x0c: {
				if ((value & 0xffff0000) == 0x05fa0000) {
					sysctl->scb.aircr = value & 0x00000700;
					
					if ((value & 0x00000004) == 0x00000004) {
						uint32_t vectors[2];
						uc_mem_read(uc, sysctl->scb.vtor, &vectors, 2 * sizeof(uint32_t));
						
						vectors[1] |= 0x00000001;
						
						uc_reg_write(uc, UC_ARM_REG_SP, &vectors[0]);
						uc_reg_write(uc, UC_ARM_REG_PC, &vectors[1]);
						printf("[debug] Software reset to address 0x%08x", vectors[1]);
						return;
					}
				}
				return;
			}
			case 0x18:
				sysctl->scb.shpr[1] = value;
				return;
			case 0x1c:
				sysctl->scb.shpr[2] = value;
				return;
			case 0x20:
				sysctl->scb.shpr[3] = value;
				return;
			case 0x24:
				sysctl->scb.shcsr = (sysctl->scb.shcsr & 0x0000ffff) | (value & 0x00070000);
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
	else if (offset >= 0xf34 && offset < 0xf48) {
		// SCB FPU
		switch (offset & 0xff) {
			case 0x34:
				sysctl->scb.fpccr = (sysctl->scb.fpccr & 0x30000000) | (value & 0xc0000000);
			case 0x38:
				sysctl->scb.fpcar = value & 0xfffffff8;
			case 0x3c:
				sysctl->scb.fpdscr = value & 0x07c00000;
			default:
				return;
		}
	}
	else if (offset == 0xf00) {
		// STIR
		value &= 0x7f;
		if (value > 98) {
			value = 98;
		}
		
		asher_peripheral_dvt1_sysctl_nvic_set_pending(uc, periph, value + 16, true);
		return;
	}
}

void asher_peripheral_dvt1_sysctl_destroy(void *userdata) {
	free(userdata);
}

void asher_peripheral_dvt1_sysctl_tick(uc_engine *uc, asher_peripheral *periph, uint64_t cycles) {
	asher_dvt1_sysctl *sysctl = (asher_dvt1_sysctl *)(periph->userdata);
	
	if ((sysctl->systick.csr & 0x00000001) == 0x00000001) {
		if (((sysctl->systick.csr >> 2) & 0x1) == 0x0) {
			cycles >>= 3;
		}
		
		if (sysctl->systick.cvr >= cycles) {
			sysctl->systick.cvr -= (uint32_t)cycles;
		}
		else {
			sysctl->systick.csr |= 0x00010000;
			
			if (sysctl->systick.rvr == 0x00000000) {
				sysctl->systick.csr &= 0xfffffffe;
			}
			else {
				sysctl->systick.cvr = sysctl->systick.rvr - ((uint32_t)cycles - sysctl->systick.cvr);
			}
			
			if (((sysctl->systick.csr >> 1) & 0x1) == 0x1) {
				asher_peripheral_dvt1_sysctl_nvic_set_pending(uc, periph, 15, true);
			}
		}
	}
}

static void asher_peripheral_dvt1_sysctl_nvic_service(uc_engine *uc, asher_dvt1_sysctl *sysctl, int32_t currentPriority, uint8_t exceptionNum) {
	uint32_t control;
	uc_reg_read(uc, UC_ARM_REG_CONTROL, &control);
	
	uint32_t ipsr;
	uc_reg_read(uc, UC_ARM_REG_IPSR, &ipsr);
	
	uint32_t frameSize = 0x20;
	
	if ((control & 0x4) != 0x0) {
		frameSize = 0x68;
	}
	
	uint32_t sp;
	bool frameAlign;
	
	if ((control & 0x2) != 0x0 && (ipsr & 0x1ff) == 0x000) {
		uc_reg_read(uc, UC_ARM_REG_PSP, &sp);
		frameAlign = (sp & 0x00000004) != 0;
		sp = (sp - frameSize) & 0xfffffff8;
		uc_reg_write(uc, UC_ARM_REG_PSP, &sp);
	}
	else {
		uc_reg_read(uc, UC_ARM_REG_MSP, &sp);
		frameAlign = (sp & 0x00000004) != 0;
		sp = (sp - frameSize) & 0xfffffff8;
		uc_reg_write(uc, UC_ARM_REG_MSP, &sp);
	}
	
	uint32_t frame[frameSize >> 2];
	
	uc_reg_read(uc, UC_ARM_REG_R0, &frame[0]);
	uc_reg_read(uc, UC_ARM_REG_R1, &frame[1]);
	uc_reg_read(uc, UC_ARM_REG_R2, &frame[2]);
	uc_reg_read(uc, UC_ARM_REG_R3, &frame[3]);
	uc_reg_read(uc, UC_ARM_REG_R12, &frame[4]);
	uc_reg_read(uc, UC_ARM_REG_LR, &frame[5]);
	uc_reg_read(uc, UC_ARM_REG_PC, &frame[6]);
	uc_reg_read(uc, UC_ARM_REG_XPSR, &frame[7]);
	
	if (frameAlign) {
		frame[7] |= 0x00000200;
	}
	
	if ((control & 0x4) != 0x0) {
		if ((sysctl->scb.fpccr & 0x40000000) == 0x00000000) {
			uc_reg_read(uc, UC_ARM_REG_S0, &frame[8]);
			uc_reg_read(uc, UC_ARM_REG_S1, &frame[9]);
			uc_reg_read(uc, UC_ARM_REG_S2, &frame[10]);
			uc_reg_read(uc, UC_ARM_REG_S3, &frame[11]);
			uc_reg_read(uc, UC_ARM_REG_S4, &frame[12]);
			uc_reg_read(uc, UC_ARM_REG_S5, &frame[13]);
			uc_reg_read(uc, UC_ARM_REG_S6, &frame[14]);
			uc_reg_read(uc, UC_ARM_REG_S7, &frame[15]);
			uc_reg_read(uc, UC_ARM_REG_S8, &frame[16]);
			uc_reg_read(uc, UC_ARM_REG_S9, &frame[17]);
			uc_reg_read(uc, UC_ARM_REG_S10, &frame[18]);
			uc_reg_read(uc, UC_ARM_REG_S11, &frame[19]);
			uc_reg_read(uc, UC_ARM_REG_S12, &frame[20]);
			uc_reg_read(uc, UC_ARM_REG_S13, &frame[21]);
			uc_reg_read(uc, UC_ARM_REG_S14, &frame[22]);
			uc_reg_read(uc, UC_ARM_REG_S15, &frame[23]);
			uc_reg_read(uc, UC_ARM_REG_FPSCR, &frame[24]);
		}
		else {
			sysctl->scb.fpcar = sp + 0x20;
			sysctl->scb.fpccr |= 0x00000001;
			sysctl->scb.fpccr |= ((control & 0x1) == 0x0 || (ipsr & 0x1ff) > 0x000) ? 0x00000000 : 0x00000002;
			sysctl->scb.fpccr |= ((ipsr & 0x1ff) > 0x000) ? 0x00000000 : 0x00000008;
			sysctl->scb.fpccr |= (currentPriority > -1) ? 0x00000010 : 0x00000000;
			sysctl->scb.fpccr |= ((sysctl->scb.shcsr & 0x00020000) != 0x00000000 && currentPriority > ((sysctl->scb.shpr[1] >> 8) & 0xff)) ? 0x00000010 : 0x00000000;
			sysctl->scb.fpccr |= ((sysctl->scb.shcsr & 0x00010000) != 0x00000000 && currentPriority > (sysctl->scb.shpr[1] & 0xff)) ? 0x00000020 : 0x00000000;
		}
	}
	
	uc_mem_write(uc, sp, frame, frameSize);
	
	uint32_t temp = 0xffffffe1;
	temp |= ((ipsr & 0x1ff) == 0x000) ? 0x00000008 : 0x00000000;
	temp |= ((control & 0x4) != 0x0) ? 0x00000000 : 0x00000010;
	temp |= ((control & 0x2) != 0x0) ? 0x00000004 : 0x00000000;
	uc_reg_write(uc, UC_ARM_REG_LR, &temp);
	
	control &= 0x1;
	uc_reg_write(uc, UC_ARM_REG_CONTROL, &control);
	
	uint32_t vector = sysctl->scb.vtor + (exceptionNum << 2);
	uc_mem_read(uc, vector, &temp, sizeof(uint32_t));
	uc_reg_write(uc, UC_ARM_REG_PC, &temp);
	
	temp = frame[7];
	temp = (temp & 0xffff0200) | exceptionNum;
	uc_reg_write(uc, UC_ARM_REG_XPSR, &temp);
	
	if (exceptionNum >= 16) {
		sysctl->nvic.iabr[(exceptionNum - 16) >> 5] |= 0x00000001 << ((exceptionNum - 16) & 0x1f);
	}
	else if (exceptionNum == 4) {
		sysctl->scb.shcsr |= 0x00000001;
	}
	else if (exceptionNum == 5) {
		sysctl->scb.shcsr |= 0x00000002;
	}
	else if (exceptionNum == 6) {
		sysctl->scb.shcsr |= 0x00000008;
	}
	else if (exceptionNum == 11) {
		sysctl->scb.shcsr |= 0x00000080;
	}
	else if (exceptionNum == 14) {
		sysctl->scb.shcsr |= 0x00000400;
	}
	else if (exceptionNum == 15) {
		sysctl->scb.shcsr |= 0x00000800;
	}
}

// this is probably wrong!!
bool asher_peripheral_dvt1_sysctl_nvic_return(uc_engine *uc, asher_peripheral *periph, uint32_t excReturn) {
	asher_dvt1_sysctl *sysctl = (asher_dvt1_sysctl *)(periph->userdata);
	
	uint32_t ipsr;
	uc_reg_read(uc, UC_ARM_REG_IPSR, &ipsr);
	
	uint8_t exceptionNum = ipsr & 0xff;
	
	if (exceptionNum >= 16) {
		sysctl->nvic.iabr[(exceptionNum - 16) >> 5] &= 0xffffffff ^ 0x00000001 << ((exceptionNum - 16) & 0x1f);
	}
	else if (exceptionNum == 4) {
		sysctl->scb.shcsr &= 0xfffffffe;
	}
	else if (exceptionNum == 5) {
		sysctl->scb.shcsr &= 0xfffffffd;
	}
	else if (exceptionNum == 6) {
		sysctl->scb.shcsr &= 0xfffffff7;
	}
	else if (exceptionNum == 11) {
		sysctl->scb.shcsr &= 0xffffff70;
	}
	else if (exceptionNum == 14) {
		sysctl->scb.shcsr &= 0xfffffbff;
	}
	else if (exceptionNum == 15) {
		sysctl->scb.shcsr &= 0xfffff7ff;
	}
	
	if (exceptionNum != 2) {
		uint32_t faultmask;
		uc_reg_read(uc, UC_ARM_REG_FAULTMASK, &faultmask);
		
		faultmask &= 0xfffffffe;
		uc_reg_write(uc, UC_ARM_REG_FAULTMASK, &faultmask);
	}
	
	uint32_t frameSize = 0x20;
	
	if ((excReturn & 0x10) != 0x0) {
		frameSize = 0x68;
	}
	
	uint32_t control;
	uc_reg_read(uc, UC_ARM_REG_CONTROL, &control);
	
	uint32_t sp;
	
	switch (excReturn & 0x0000000f) {
		case 0x0:
		case 0x1:
		case 0x8:
		case 0x9:
			uc_reg_read(uc, UC_ARM_REG_MSP, &sp);
			sp += frameSize;
			uc_reg_write(uc, UC_ARM_REG_MSP, &sp);
			
			control &= 0x1;
			break;
		case 0xc:
		case 0xd:
			uc_reg_read(uc, UC_ARM_REG_PSP, &sp);
			sp += frameSize;
			uc_reg_write(uc, UC_ARM_REG_PSP, &sp);
			
			control = (control & 0x1) | 0x2;
			break;
		default:
			return false;
	}
	
	uint32_t frame[frameSize >> 2];
	
	uc_mem_read(uc, sp - frameSize, frame, frameSize);
	
	uc_reg_write(uc, UC_ARM_REG_R0, &frame[0]);
	uc_reg_write(uc, UC_ARM_REG_R1, &frame[1]);
	uc_reg_write(uc, UC_ARM_REG_R2, &frame[2]);
	uc_reg_write(uc, UC_ARM_REG_R3, &frame[3]);
	uc_reg_write(uc, UC_ARM_REG_R12, &frame[4]);
	uc_reg_write(uc, UC_ARM_REG_LR, &frame[5]);
	uc_reg_write(uc, UC_ARM_REG_PC, &frame[6]);
	uc_reg_write(uc, UC_ARM_REG_XPSR, &frame[7]);
	
	if ((excReturn & 0x10) == 0x0) {
		if ((sysctl->scb.fpccr & 0x00000001) != 0x00000000) {
			sysctl->scb.fpccr &= 0xfffffffe;
		}
		else {
			uc_reg_write(uc, UC_ARM_REG_S0, &frame[8]);
			uc_reg_write(uc, UC_ARM_REG_S1, &frame[9]);
			uc_reg_write(uc, UC_ARM_REG_S2, &frame[10]);
			uc_reg_write(uc, UC_ARM_REG_S3, &frame[11]);
			uc_reg_write(uc, UC_ARM_REG_S4, &frame[12]);
			uc_reg_write(uc, UC_ARM_REG_S5, &frame[13]);
			uc_reg_write(uc, UC_ARM_REG_S6, &frame[14]);
			uc_reg_write(uc, UC_ARM_REG_S7, &frame[15]);
			uc_reg_write(uc, UC_ARM_REG_S8, &frame[16]);
			uc_reg_write(uc, UC_ARM_REG_S9, &frame[17]);
			uc_reg_write(uc, UC_ARM_REG_S10, &frame[18]);
			uc_reg_write(uc, UC_ARM_REG_S11, &frame[19]);
			uc_reg_write(uc, UC_ARM_REG_S12, &frame[20]);
			uc_reg_write(uc, UC_ARM_REG_S13, &frame[21]);
			uc_reg_write(uc, UC_ARM_REG_S14, &frame[22]);
			uc_reg_write(uc, UC_ARM_REG_S15, &frame[23]);
			uc_reg_write(uc, UC_ARM_REG_FPSCR, &frame[24]);
		}
	}
	
	control |= ((excReturn & 0x10) != 0) ? 0x0 : 0x4;
	uc_reg_write(uc, UC_ARM_REG_CONTROL, &control);
	
	return true;
}

void asher_peripheral_dvt1_sysctl_nvic_set_pending(uc_engine *uc, asher_peripheral *periph, uint8_t exceptionNum, bool pending) {
	asher_dvt1_sysctl *sysctl = (asher_dvt1_sysctl *)(periph->userdata);
	
	if (exceptionNum >= 16 && exceptionNum < 114) {
		if (pending) {
			sysctl->nvic.ispr[(exceptionNum - 16) >> 5] |= 0x00000001 << ((exceptionNum - 16) & 0x1f);
		}
		else {
			sysctl->nvic.ispr[(exceptionNum - 16) >> 5] &= 0xffffffff ^ (0x00000001 << ((exceptionNum - 16) & 0x1f));
		}
	}
	else if (exceptionNum == 2) {
		if (pending) {
			sysctl->scb.icsr |= 0x80000000;
		}
		else {
			sysctl->scb.icsr &= 0x7fffffff;
		}
	}
	else if (exceptionNum == 4) {
		if (pending) {
			sysctl->scb.shcsr |= 0x00002000;
		}
		else {
			sysctl->scb.shcsr &= 0xffffdfff;
		}
	}
	else if (exceptionNum == 5) {
		if (pending) {
			sysctl->scb.shcsr |= 0x00004000;
		}
		else {
			sysctl->scb.shcsr &= 0xffffbfff;
		}
	}
	else if (exceptionNum == 6) {
		if (pending) {
			sysctl->scb.shcsr |= 0x00001000;
		}
		else {
			sysctl->scb.shcsr &= 0xffffefff;
		}
	}
	else if (exceptionNum == 11) {
		if (pending) {
			sysctl->scb.icsr |= 0x00008000;
		}
		else {
			sysctl->scb.icsr &= 0xffff7fff;
		}
	}
	else if (exceptionNum == 14) {
		if (pending) {
			sysctl->scb.icsr |= 0x10000000;
		}
		else {
			sysctl->scb.icsr &= 0xefffffff;
		}
	}
	else if (exceptionNum == 15) {
		if (pending) {
			sysctl->scb.icsr |= 0x04000000;
		}
		else {
			sysctl->scb.icsr &= 0xfbffffff;
		}
	}
	
	bool escalateIfMasked = pending && ((exceptionNum >= 4 && exceptionNum < 7) || exceptionNum == 11);
	
	uint32_t primask;
	uc_reg_read(uc, UC_ARM_REG_PRIMASK, &primask);
	
	uint32_t faultmask;
	uc_reg_read(uc, UC_ARM_REG_FAULTMASK, &faultmask);
	
	uint32_t ipsr;
	uc_reg_read(uc, UC_ARM_REG_IPSR, &ipsr);
	
	int32_t priority;
	
	if (ipsr >= 16) {
		uint16_t currentExcNum = (ipsr - 16) & 0x1ff;
		priority = (sysctl->nvic.ipr[currentExcNum >> 2] >> ((currentExcNum & 0x3) << 3)) & (0xff << ((sysctl->scb.aircr >> 8) & 0x7));
	}
	else if (ipsr > 0) {
		priority = (sysctl->scb.shpr[(ipsr & 0x1ff) >> 2] >> ((ipsr & 0x003) << 3)) & (0xff << ((sysctl->scb.aircr >> 8) & 0x7));
	}
	
	int32_t basepri;
	uc_reg_read(uc, UC_ARM_REG_BASEPRI, &basepri);
	
	if (basepri == 0) {
		basepri = 256;
	}
	else {
		basepri &= (0xff << ((sysctl->scb.aircr >> 8) & 0x7));
	}
	
	priority = (priority < basepri) ? priority : basepri;
	
	if (!primask && !faultmask) {
		uint8_t newExceptionNum = 0;
		int32_t maxPriority = priority;
		
		for (uint8_t i = 2; i < 114; i++) {
			bool anotherPending = false;
			
			if (i >= 16) {
				anotherPending = ((sysctl->nvic.ispr[(i - 16) >> 5] >> ((i - 16) & 0x1f)) & 0x00000001) != 0x00000000;
			}
			else if (i == 2) {
				anotherPending = (sysctl->scb.icsr & 0x80000000) != 0x00000000;
			}
			else if (i == 4) {
				anotherPending = (sysctl->scb.shcsr & 0x00002000) != 0x00000000;
			}
			else if (i == 5) {
				anotherPending = (sysctl->scb.shcsr & 0x00004000) != 0x00000000;
			}
			else if (i == 6) {
				anotherPending = (sysctl->scb.shcsr & 0x00001000) != 0x00000000;
			}
			else if (i == 11) {
				anotherPending = (sysctl->scb.shcsr & 0x00008000) != 0x00000000;
			}
			else if (i == 14) {
				anotherPending = (sysctl->scb.icsr & 0x10000000) != 0x00000000;
			}
			else if (i == 15) {
				anotherPending = (sysctl->scb.icsr & 0x04000000) != 0x00000000;
			}
			else {
				anotherPending = i == exceptionNum;
			}
			
			if (anotherPending) {
				int32_t newPriority;
				
				if (i >= 16) {
					uint16_t currentExcNum = (i - 16) & 0xffff;
					newPriority = (sysctl->nvic.ipr[currentExcNum >> 3] >> ((currentExcNum & 0x3) << 3)) & (0xff << ((sysctl->scb.aircr >> 8) & 0x7));
				}
				else {
					newPriority = (sysctl->scb.shpr[i >> 2] >> (i << 3)) & (0xff << ((sysctl->scb.aircr >> 8) & 0x7));
				}
				
				if (newPriority < maxPriority) {
					newExceptionNum = i;
					maxPriority = newPriority;
				}
			}
		}
		
		if (newExceptionNum > 0) {
			asher_peripheral_dvt1_sysctl_nvic_service(uc, sysctl, priority, newExceptionNum);
			return;
		}
		else if (escalateIfMasked) {
			bool enabled = false;
			
			if (exceptionNum == 4) {
				enabled = (sysctl->scb.shcsr & 0x00010000) != 0x00000000;
			}
			else if (exceptionNum == 5) {
				enabled = (sysctl->scb.shcsr & 0x00020000) != 0x00000000;
			}
			else if (exceptionNum == 6) {
				enabled = (sysctl->scb.shcsr & 0x00040000) != 0x00000000;
			}
			
			if (!enabled) {
				sysctl->scb.hfsr |= 0x40000000;
				asher_peripheral_dvt1_sysctl_nvic_service(uc, sysctl, priority, 3);
				return;
			}
		}
	}
	else if (faultmask && exceptionNum == 2) {
		asher_peripheral_dvt1_sysctl_nvic_service(uc, sysctl, priority, exceptionNum);
		return;
	}
	else if (primask && exceptionNum < 4) {
		asher_peripheral_dvt1_sysctl_nvic_service(uc, sysctl, priority, exceptionNum);
		return;
	}
}
