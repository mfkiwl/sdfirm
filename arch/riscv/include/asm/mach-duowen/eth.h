#ifndef __ETH_DUOWEN_H_INCLUDE__
#define __ETH_DUOWEN_H_INCLUDE__

#include <target/clk.h>

#define DW_XGMAC_BASE		XGMAC_BASE
#define DW_XGMAC_CLK_SRC	sysfab_clk

#define DW_XPCS_BASE		XPCS_BASE
#define DW_XPCS_PHY_ADDR	0
#define DW_XPCS_APBI_OFFSET	1
#define DW_XPCS_PMA_TYPE	SR_PMA_10000BASE_KR
#define DW_XPCS_PCS_TYPE	SR_PCS_10000BASE_R

#include <driver/dw_xgmac.h>
#include <driver/dw_xpcs.h>

#define dw_xpcs_hw_sram_init()				\
	do {						\
		dw_xpcs_int_sram_init();		\
		dw_xpcs_ext_sram_init();		\
	} while (0)

#ifdef CONFIG_DUOWEN_ETH_LOOPBACK
#define dw_xpcs_hw_ctrl_init()		dw_xpcs_enable_loopback()
#else
#define dw_xpcs_hw_ctrl_init()		do { } while (0)
#endif

#define duowen_eth_clock_init()				\
	do {						\
		clk_enable(eth_alt_ref_clk);		\
		clk_enable(sgmii_ref_clk);		\
	} while (0)

#ifdef CONFIG_DUOWEN_ETH
#define duowen_eth_init()				\
	do {						\
		duowen_eth_clock_init();		\
		dw_xgmac_init_ctrl();			\
	} while (0)
#define eth_hw_mdio_read(a, r, d)	dw_xgmac_mdio_read(a, r, d)
#define eth_hw_mdio_write(a, r, d)	dw_xgmac_mdio_write(a, r, d)
#else
#define duowen_eth_init()		duowen_eth_clock_init()
#endif

#ifdef CONFIG_DUOWEN_ETH_INIT
#define duowen_eth_late_init()		duowen_eth_init()
#else
#define duowen_eth_late_init()		do { } while (0)
#endif

#endif /* __ETH_DUOWEN_H_INCLUDE__ */