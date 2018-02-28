/*
 * Copyright (C) 2016 - 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */
 
#include <arch_helpers.h>
#include <plat_marvell.h>
#include <debug.h>
#include <mv_ddr_if.h>
#include <plat_def.h>
#include <mmio.h>
#include <a8k_i2c.h>

#define MVEBU_CP_MPP_CTRL37_OFFS		20
#define MVEBU_CP_MPP_CTRL38_OFFS		24
#define MVEBU_CP_MPP_CTRL37_I2C0_SCK_ENA	0x2
#define MVEBU_CP_MPP_CTRL38_I2C0_SDA_ENA	0x2

#define MVEBU_MPP_CTRL_MASK			0xf

struct dram_config dram_cfg;

/*
 * This struct provides the DRAM training code with
 * the appropriate board DRAM configuration
 */
static struct mv_ddr_topology_map board_topology_map = {
	/* Board with 1CS 8Gb x4 devices of Micron 2400T */
	DEBUG_LEVEL_ERROR,
	0x1, /* active interfaces */
	/* cs_mask, mirror, dqs_swap, ck_swap X subphys */
	{ { { {0x1, 0x0, 0, 0},	/* FIXME: change the cs mask for all 64 bit */
	      {0x1, 0x0, 0, 0},
	      {0x1, 0x0, 0, 0},
	      {0x1, 0x0, 0, 0},
	      {0x1, 0x0, 0, 0},
	      {0x1, 0x0, 0, 0},
	      {0x1, 0x0, 0, 0},
	      {0x1, 0x0, 0, 0},
	      {0x1, 0x0, 0, 0} },
	   /* TODO: double check if the speed bin is 2400T */
	   SPEED_BIN_DDR_2400T,		/* speed_bin */
	   MV_DDR_DEV_WIDTH_8BIT,	/* sdram device width */
	   MV_DDR_DIE_CAP_8GBIT,	/* die capacity */
	   MV_DDR_FREQ_SAR,		/* frequency */
	   0, 0,			/* cas_l, cas_wl */
	   MV_DDR_TEMP_LOW} },		/* temperature */
	   MV_DDR_64BIT_BUS_MASK,	/* subphys mask */
	   MV_DDR_CFG_SPD,		/* ddr configuration data source */
	{ {0} },			/* raw spd data */
	{0}				/* timing parameters */
};

struct mv_ddr_topology_map *mv_ddr_topology_map_get(void)
{
	/* Return the board topology as defined in the board code */
	return &board_topology_map;
}

struct dram_config *mv_ddr_dram_config_get(void)
{
	/* Return dram configuration as defined in the board code */
	return &dram_cfg;
}

static void mpp_config(void)
{
	uint32_t val;
	uintptr_t reg = MVEBU_CP_MPP_REGS(0, 4);

	/* configure CP0 MPP 37 and 38 to i2c */
	val = mmio_read_32(reg);
	val &= ~((MVEBU_MPP_CTRL_MASK << MVEBU_CP_MPP_CTRL37_OFFS) |
		(MVEBU_MPP_CTRL_MASK << MVEBU_CP_MPP_CTRL38_OFFS));
	val |= (MVEBU_CP_MPP_CTRL37_I2C0_SCK_ENA << MVEBU_CP_MPP_CTRL37_OFFS) |
		(MVEBU_CP_MPP_CTRL38_I2C0_SDA_ENA << MVEBU_CP_MPP_CTRL38_OFFS);
	mmio_write_32(reg, val);
}

/*
 * This function may modify the default DRAM parameters
 * based on information recieved from SPD or bootloader
 * configuration located on non volatile storage
 */
void plat_dram_update_topology(void)
{
	struct mv_ddr_topology_map *tm = mv_ddr_topology_map_get();

	INFO("Gathering DRAM information\n");

	if (tm->cfg_src == MV_DDR_CFG_SPD) {
		/* configure MPPs to enable i2c */
		mpp_config();
		/* initialize the i2c */
		i2c_init((void *)MVEBU_CP0_I2C_BASE);
		/* select SPD memory page 0 to access DRAM configuration */
		i2c_write(I2C_SPD_P0_ADDR, 0x0, 1, tm->spd_data.all_bytes, 1);
		/* read data from spd */
		i2c_read(I2C_SPD_ADDR, 0x0, 1, tm->spd_data.all_bytes,
			 sizeof(tm->spd_data.all_bytes));
	}
}

void *plat_get_dram_data(void)
{
	return &dram_cfg;
}