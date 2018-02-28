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
	/* CP1 (MCI0) internal regs */
	{0x00000000f4000000,		0x2000000,  MCI_0_TID},
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
struct addr_map_win iob_memory_map_cp0[] = {
	/* CP1 */
	/* PEX0_X4 window */
	{0x0000008000000000,	0x800000000,	PEX0_TID}
};

int marvell_get_iob_memory_map(struct addr_map_win **win, uint32_t *size, uintptr_t base)
{
	switch (base) {
	case MVEBU_CP_REGS_BASE(0):
		*win = iob_memory_map_cp0;
		*size = sizeof(iob_memory_map_cp0)/sizeof(iob_memory_map_cp0[0]);
		return 0;
	case MVEBU_CP_REGS_BASE(1):
		*size = 0;
		*win = 0;
		return 0;
	default:
		*size = 0;
		*win = 0;
		return 1;
	}
}

/*******************************************************************************
 * CCU Configuration
 ******************************************************************************/
struct addr_map_win ccu_memory_map[] = {
	{0x00000000f2000000,	0xe000000,  IO_0_TID}, /* IO window */
	{0x0000008000000000,	0x800000000,   IO_0_TID}, /* IO window */
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
/*******************************************************************************
 * PCIe Configuration
 ******************************************************************************/
struct pci_hw_cfg ocp_pci_hw_cfg = {
	.delay_cfg	= 1,
	.master_en	= 1,
	.lane_width	= 4,
	.lane_ids	= {0, 1, 2, 3},
	.clk_src	= 0,
	.clk_out	= 0,	/* clk is not output */
	.is_end_point	= 1,
	.mac_base	= MVEBU_PCIE_X4_MAC_BASE(0),
	.comphy_base	= MVEBU_COMPHY_BASE(0),
	.hpipe_base	= MVEBU_HPIPE_BASE(0),
	.dfx_base	= MVEBU_CP_REGS_BASE(0) + MVEBU_CP_DFX_OFFSET,
};

struct pci_hw_cfg *plat_get_pcie_hw_data(void)
{
	return &ocp_pci_hw_cfg;
}

/*******************************************************************************
 * SKIP IMAGE Configuration
 ******************************************************************************/

void *plat_get_skip_image_data(void)
{
	/* Return the skip_image configurations */
	return NULL;
}
#endif