#include "../peripheral.h"

typedef struct {
	uint8_t id;
	
	uint32_t lisr;
	uint32_t hisr;
	struct {
		uint32_t cr;
		uint32_t ndtr;
		uint32_t par;
		uint32_t m0ar;
		uint32_t m1ar;
		uint32_t fcr;
	} streams[8];
} asher_dvt1_dma;

void *asher_peripheral_dvt1_dma_create(uint32_t baseAddr) {
	asher_dvt1_dma *dma = malloc(sizeof(asher_dvt1_dma));
	
	if (baseAddr == 0x40026000) {
		dma->id = 1;
	}
	else if (baseAddr == 0x40026400) {
		dma->id = 2;
	}
	
	asher_peripheral_dvt1_dma_reset(dma);
	return dma;
}

void asher_peripheral_dvt1_dma_reset(void *userdata) {
	asher_dvt1_dma *dma = (asher_dvt1_dma *)userdata;
	
	dma->lisr = 0x00000000;
	dma->hisr = 0x00000000;
	
	for (uint8_t i = 0; i < 8; i++) {
		dma->streams[i].cr = 0x00000000;
		dma->streams[i].ndtr = 0x00000000;
		dma->streams[i].par = 0x00000000;
		dma->streams[i].m0ar = 0x00000000;
		dma->streams[i].m1ar = 0x00000000;
		dma->streams[i].fcr = 0x00000021;
	}
}

uint64_t asher_peripheral_dvt1_dma_read(uc_engine *uc, uint64_t offset, unsigned size, void *periph) {
	asher_dvt1_dma *dma = (asher_dvt1_dma *)(((asher_peripheral *)periph)->userdata);
	
	if (offset == 0x00) {
		return dma->lisr;
	}
	else if (offset == 0x04) {
		return dma->hisr;
	}
	else if (offset >= 0x10) {
		uint8_t n = (offset - 0x10) / 0x18;
		
		switch ((offset - 0x10) % 0x18) {
			case 0x00:
				return dma->streams[n].cr;
			case 0x04:
				return dma->streams[n].ndtr;
			case 0x08:
				return dma->streams[n].par;
			case 0x0c:
				return dma->streams[n].m0ar;
			case 0x10:
				return dma->streams[n].m1ar;
			case 0x14:
				return dma->streams[n].fcr;
			default:
				return 0x00000000;
		}
	}
	
	return 0x00000000;
}

void asher_peripheral_dvt1_dma_write(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *periph) {
	asher_dvt1_dma *dma = (asher_dvt1_dma *)(((asher_peripheral *)periph)->userdata);
	
	if (offset == 0x08) {
		dma->lisr &= 0xffffffff ^ (value & 0x0f7d0f7d);
	}
	else if (offset == 0x0c) {
		dma->hisr &= 0xffffffff ^ (value & 0x0f7d0f7d);
	}
	else if (offset >= 0x10) {
		uint8_t n = (offset - 0x10) / 0x18;
		
		switch ((offset - 0x10) % 0x18) {
			case 0x00: {
				if ((dma->streams[n].cr & 0x00000001) == 0) {
					if ((value & 0x00000001) == 1) {
						// todo: start a DMA transfer
						printf("[debug] DMA%d_S%d transfer started", dma->id, n);
						
					}
					dma->streams[n].cr = value & 0x0fefffff;
				}
				else {
					dma->streams[n].cr &= 0xffffffe0;
					dma->streams[n].cr |= value & 0x0000001f;
				}
				return;
			}
			case 0x04: {
				if ((dma->streams[n].cr & 0x00000001) == 0) {
					dma->streams[n].ndtr = value & 0xffff;
				}
				return;
			}
			case 0x08: {
				if ((dma->streams[n].cr & 0x00000001) == 0) {
					dma->streams[n].par = value;
				}
				return;
			}
			case 0x0c: {
				if ((dma->streams[n].cr & 0x00000001) == 0 || (dma->streams[n].cr & 0x00080000) == 0x00080000) {
					dma->streams[n].m0ar = value;
				}
				return;
			}
			case 0x10: {
				if ((dma->streams[n].cr & 0x00000001) == 0 || (dma->streams[n].cr & 0x00080000) == 0) {
					dma->streams[n].m1ar = value;
				}
				return;
			}
			case 0x14: {
				if ((dma->streams[n].cr & 0x00000001) == 0) {
					dma->streams[n].fcr = (dma->streams[n].fcr & 0x00000038) | (value & 0x00000087);
				}
				else {
					dma->streams[n].fcr = (dma->streams[n].fcr & 0x0000003f) | (value & 0x00000080);
				}
				return;
			}
			default:
				return;
		}
	}
}

void asher_peripheral_dvt1_dma_destroy(void *userdata) {
	free(userdata);
}
