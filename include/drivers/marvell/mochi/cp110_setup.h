/*
 * Copyright (C) 2016 - 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */
 
#ifndef __PLAT_CP110_H__
#define __PLAT_CP110_H__

#include <mmio.h>
#include <plat_def.h>
#include <plat_marvell.h>

#define MVEBU_DEVICE_ID_REG		(MVEBU_CP_DFX_OFFSET + 0x40)
#define MVEBU_DEVICE_ID_OFFSET		(0)
#define MVEBU_DEVICE_ID_MASK		(0xffff << MVEBU_DEVICE_ID_OFFSET)
#define MVEBU_DEVICE_REV_OFFSET		(16)
#define MVEBU_DEVICE_REV_MASK		(0xf << MVEBU_DEVICE_REV_OFFSET)
#define MVEBU_70X0_DEV_ID		(0x7040)
#define MVEBU_80X0_DEV_ID		(0x8040)
#define MVEBU_CP110_SA_DEV_ID		(0x110)
#define MVEBU_CP110_REF_ID_A1		1
#define MVEBU_CP110_REF_ID_A2		2

static inline uint32_t cp110_device_id_get(uintptr_t base)
{
	/* Returns:
	 * - MVEBU_70X0_DEV_ID for A70X0 family
	 * - MVEBU_80X0_DEV_ID for A80X0 family
	 * - MVEBU_CP110_SA_DEV_ID for CP that connected stand alone
	 */
	return (mmio_read_32(base + MVEBU_DEVICE_ID_REG) >>
		MVEBU_DEVICE_ID_OFFSET) &
		MVEBU_DEVICE_ID_MASK;
}

static inline uint32_t cp110_rev_id_get(uintptr_t base)
{
	return (mmio_read_32(base + MVEBU_DEVICE_ID_REG) &
		MVEBU_DEVICE_REV_MASK) >>
		MVEBU_DEVICE_REV_OFFSET;
}

void cp110_init(uintptr_t cp110_base);
void cp110_ble_init(uintptr_t cp110_base);

#endif /* __PLAT_CP110_H__ */