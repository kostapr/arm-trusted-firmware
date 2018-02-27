#
# Copyright (C) 2016 - 2018 Marvell International Ltd.
#
# SPDX-License-Identifier:     BSD-3-Clause
# https://spdx.org/licenses
 

PLAT_MARVELL		:= 	plat/marvell
A8K_MSS_SOURCE		:= 	$(PLAT_MARVELL)/a8k/common/mss

BL2_SOURCES		+=	$(A8K_MSS_SOURCE)/mss_bl2_setup.c

BL31_SOURCES		+=	$(A8K_MSS_SOURCE)/mss_pm_ipc.c

PLAT_INCLUDES           +=      -I$(A8K_MSS_SOURCE)

ifneq (${SCP_BL2},)
# Subsystems require a SCP_BL2 image
$(eval $(call FIP_ADD_IMG,SCP_BL2,--scp-fw))

# This define is used to inidcate the SCP image is present
$(eval $(call add_define,SCP_IMAGE))
endif
