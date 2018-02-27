/*
 * Copyright (C) 2016 - 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */
 
#include <plat_config.h>
/*
 * If bootrom is currently at BLE there's no need to include the memory
 * maps structure at this point
 */
#include <plat_def.h>
#ifndef IMAGE_BLE

/*******************************************************************************
 * AMB Configuration
 ******************************************************************************/
struct addr_map_win *amb_memory_map;

int marvell_get_amb_memory_map(struct addr_map_win **win, uint32_t *size, uintptr_t base)
{
	*win = amb_memory_map;
	if (*win == NULL)
		*size = 0;
	else
		*size = sizeof(amb_memory_map)/sizeof(amb_memory_map[0]);

	return 0;
}

/*******************************************************************************
 * IO WIN Configuration
 ******************************************************************************/

struct addr_map_win io_win_memory_map[] = {
	/* MCI 0 indirect window */
	{MVEBU_MCI_REG_BASE_REMAP(0),	0x100000,   MCI_0_TID},
	/* MCI 1 indirect window */
	{MVEBU_MCI_REG_BASE_REMAP(1),	0x100000,   MCI_1_TID},
};

uint32_t marvell_get_io_win_gcr_target(int ap_index)
{
	return PIDI_TID;
}

int marvell_get_io_win_memory_map(int ap_index, struct addr_map_win **win, uint32_t *size)
{
	*win = io_win_memory_map;
	if (*win == NULL)
		*size = 0;
	else
		*size = sizeof(io_win_memory_map)/sizeof(io_win_memory_map[0]);

	return 0;
}

/*******************************************************************************
 * IOB Configuration
 ******************************************************************************/
struct addr_map_win iob_memory_map[] = {
	/* PEX0_X4 window */
	{0x00000000f6000000,	0x6000000,	PEX0_TID},
};

int marvell_get_iob_memory_map(struct addr_map_win **win, uint32_t *size, uintptr_t base)
{
	*win = iob_memory_map;
	*size = sizeof(iob_memory_map)/sizeof(iob_memory_map[0]);

	return 0;
}

/*******************************************************************************
 * CCU Configuration
 ******************************************************************************/
struct addr_map_win ccu_memory_map[] = {
	{0x0000000f2000000,	0xe000000,   IO_0_TID}, /* IO window */
};

uint32_t marvell_get_ccu_gcr_target(int ap)
{
	return DRAM_0_TID;
}

int marvell_get_ccu_memory_map(int ap_index, struct addr_map_win **win, uint32_t *size)
{
	*win = ccu_memory_map;
	*size = sizeof(ccu_memory_map)/sizeof(ccu_memory_map[0]);

	return 0;
}

/* In reference to #ifndef IMAGE_BLE, this part is used for BLE only. */
#else

struct pci_hw_cfg *plat_get_pcie_hw_data(void)
{
	return NULL;
}

/*******************************************************************************
 * SKIP IMAGE Configuration
 ******************************************************************************/

#if PLAT_RECOVERY_IMAGE_ENABLE
struct skip_image skip_im = {
	.detection_method = GPIO,
	.info.gpio.num = 33,
	.info.gpio.button_state = HIGH,
	.info.test.cp_ap = CP,
	.info.test.cp_index = 0,
};

void *plat_get_skip_image_data(void)
{
	/* Return the skip_image configurations */
	return &skip_im;
}
#endif
#endif
