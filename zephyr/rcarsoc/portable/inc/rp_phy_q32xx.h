/*
 * Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef RP_PHY_Q32XX_H
#define RP_PHY_Q32XX_H

#include <stdbool.h>
#include <stdint.h>

struct device;

enum rp_phy_q32xx_line_rate
{
	RP_PHY_Q32XX_LINE_RATE_NONE = 0,
	RP_PHY_Q32XX_LINE_RATE_2P5G,
	RP_PHY_Q32XX_LINE_RATE_5G,
	RP_PHY_Q32XX_LINE_RATE_10G,
};

enum rp_phy_q32xx_operation_mode
{
	RP_PHY_Q32XX_OP_SLAVE = 0,
	RP_PHY_Q32XX_OP_MASTER,
};

enum rp_phy_q32xx_serdes_speed
{
	RP_PHY_Q32XX_SERDES_SPEED_DISABLED = 0,
	RP_PHY_Q32XX_SERDES_2500_BASEX,
	RP_PHY_Q32XX_SERDES_2P5G_BASEX,
	RP_PHY_Q32XX_SERDES_5000_BASER,
	RP_PHY_Q32XX_SERDES_5G_BASER,
	RP_PHY_Q32XX_SERDES_10G_BASER,
	RP_PHY_Q32XX_SERDES_5G_USXGMII,
	RP_PHY_Q32XX_SERDES_10G_USXGMII,
};

enum rp_phy_q32xx_init_flag
{
	RP_PHY_Q32XX_CFG_T1_RATE = 0x1U,
	RP_PHY_Q32XX_CFG_SERDES_SPEED = 0x2U,
	RP_PHY_Q32XX_CFG_MASTER_SLAVE = 0x4U,
};

struct rp_phy_q32xx_init_cfg
{
	uint32_t flags;
	enum rp_phy_q32xx_line_rate t1_rate;
	enum rp_phy_q32xx_serdes_speed serdes_speed;
	enum rp_phy_q32xx_operation_mode operation_mode;
};

struct rp_phy_q32xx_link_state
{
	bool is_up;
	enum rp_phy_q32xx_line_rate line_rate;
};

/** @brief Read the current T1 link status and line rate. */
int rp_phy_q32xx_get_link_state(const struct device *mdio_dev,
				 uint8_t phy_addr,
				 struct rp_phy_q32xx_link_state *state);

/** @brief Set the T1 line rate and restart the line-side link. */
int rp_phy_q32xx_set_line_rate(const struct device *mdio_dev, uint8_t phy_addr,
				enum rp_phy_q32xx_line_rate line_rate);

/** @brief Initialize the PHY and apply the configuration overrides selected by flags. */
int rp_phy_q32xx_init(const struct device *mdio_dev, uint8_t phy_addr,
		       const struct rp_phy_q32xx_init_cfg *cfg);

#endif /* RP_PHY_Q32XX_H */
