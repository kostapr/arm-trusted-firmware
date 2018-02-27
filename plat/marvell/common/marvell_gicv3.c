/*
 * Copyright (C) 2016 - 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */
/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <arm_def.h>
#include <debug.h>
#include <gicv3.h>
#include <plat_marvell.h>
#include <platform.h>
#include <platform_def.h>

/******************************************************************************
 * The following functions are defined as weak to allow a platform to override
 * the way the GICv3 driver is initialised and used.
 *****************************************************************************/
#pragma weak plat_marvell_gic_driver_init
#pragma weak plat_marvell_gic_init
#pragma weak plat_marvell_gic_cpuif_enable
#pragma weak plat_marvell_gic_cpuif_disable
#pragma weak plat_marvell_gic_pcpu_init

/* The GICv3 driver only needs to be initialized in EL3 */
static uintptr_t rdistif_base_addrs[PLATFORM_CORE_COUNT];

/* Array of Group1 secure interrupts to be configured by the gic driver */
static const unsigned int g1s_interrupt_array[] = {
	PLAT_MARVELL_G1S_IRQS
};

/* Array of Group0 interrupts to be configured by the gic driver */
static const unsigned int g0_interrupt_array[] = {
	PLAT_MARVELL_G0_IRQS
};

const gicv3_driver_data_t marvell_gic_data = {
	.gicd_base = PLAT_MARVELL_GICD_BASE,
	.gicr_base = PLAT_MARVELL_GICR_BASE,
	.g0_interrupt_num = ARRAY_SIZE(g0_interrupt_array),
	.g1s_interrupt_num = ARRAY_SIZE(g1s_interrupt_array),
	.g0_interrupt_array = g0_interrupt_array,
	.g1s_interrupt_array = g1s_interrupt_array,
	.rdistif_num = PLATFORM_CORE_COUNT,
	.rdistif_base_addrs = rdistif_base_addrs,
	.mpidr_to_core_pos = plat_marvell_calc_core_pos
};

void plat_marvell_gic_driver_init(void)
{
	/*
	 * The GICv3 driver is initialized in EL3 and does not need
	 * to be initialized again in SEL1. This is because the S-EL1
	 * can use GIC system registers to manage interrupts and does
	 * not need GIC interface base addresses to be configured.
	 */
#if IMAGE_BL31
	gicv3_driver_init(&marvell_gic_data);
#endif
}

/******************************************************************************
 * Marvell common helper to initialize the GIC. Only invoked by BL31
 *****************************************************************************/
void plat_marvell_gic_init(void)
{
	/* Initialize GIC-600 Multi Chip feature,
	 * only if the maximum number of north bridges
	 * is more than 1 - otherwise no need for multi
	 * chip feature initialization
	 */
#if (PLAT_MARVELL_NORTHB_COUNT > 1)
	if (gic600_multi_chip_init())
		ERROR("GIC-600 Multi Chip initialization failed\n");
#endif
	gicv3_distif_init();
	gicv3_rdistif_init(plat_my_core_pos());
	gicv3_cpuif_enable(plat_my_core_pos());
}

/******************************************************************************
 * Marvell common helper to enable the GIC CPU interface
 *****************************************************************************/
void plat_marvell_gic_cpuif_enable(void)
{
	gicv3_cpuif_enable(plat_my_core_pos());
}

/******************************************************************************
 * Marvell common helper to disable the GIC CPU interface
 *****************************************************************************/
void plat_marvell_gic_cpuif_disable(void)
{
	gicv3_cpuif_disable(plat_my_core_pos());
}

/******************************************************************************
 * Marvell common helper to initialize the per-cpu redistributor interface in GICv3
 *****************************************************************************/
void plat_marvell_gic_pcpu_init(void)
{
	gicv3_rdistif_init(plat_my_core_pos());
}

/******************************************************************************
 * Marvell common helper to save SPI irq states in GICv3
 *****************************************************************************/
void plat_marvell_gic_irq_save(void)
{
	gicv3_irq_save((uintptr_t)PLAT_MARVELL_GICD_BASE);
}

/******************************************************************************
 * Marvell common helper to restore SPI irq states in GICv3
 *****************************************************************************/
void plat_marvell_gic_irq_restore(void)
{
	gicv3_irq_restore((uintptr_t)PLAT_MARVELL_GICD_BASE);
}

/******************************************************************************
 * Marvell common helper to save per-cpu PPI irq states in GICv3
 *****************************************************************************/
void plat_marvell_gic_irq_pcpu_save(void)
{
	gicv3_irq_pcpu_save(marvell_gic_data.rdistif_base_addrs[plat_my_core_pos()],
			    plat_my_core_pos());
}

/******************************************************************************
 * Marvell common helper to restore per-cpu PPI irq states in GICv3
 *****************************************************************************/
void plat_marvell_gic_irq_pcpu_restore(void)
{
	gicv3_irq_pcpu_restore(marvell_gic_data.rdistif_base_addrs[plat_my_core_pos()],
			       plat_my_core_pos());
}
