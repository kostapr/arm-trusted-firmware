/*
 * Copyright (C) 2016 - 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */
 
#ifndef __APN806_SETUP_H__
#define __APN806_SETUP_H__

#include <mmio.h>
#include <plat_def.h>

void apn806_init(void);
uint32_t apn806_sar_get_bootsrc(void);

static inline int apn806_rev_id_get(void)
{
	return (mmio_read_32(MVEBU_CSS_GWD_CTRL_IIDR2_REG) >>
		GWD_IIDR2_REV_ID_OFFSET) &
		GWD_IIDR2_REV_ID_MASK;
}

#endif /* __APN806_SETUP_H__ */
