#ifndef ASHER_PERIPHERALS_H
#define ASHER_PERIPHERALS_H

#include <stdbool.h>

#include "device.h"

bool asher_peripherals_dvt1_register(asher_device *device);
bool asher_peripherals_dvt1_unregister(asher_device *device);

bool asher_peripherals_h7d1_register(asher_device *device);
bool asher_peripherals_h7d1_unregister(asher_device *device);

#endif