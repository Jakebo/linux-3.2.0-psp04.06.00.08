/*
 * Copyright (C) 2009 Nokia
 * Copyright (C) 2009-2010 Texas Instruments
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define OMAP_MUX_TERMINATOR	0xffff

/* 34xx mux mode options for each pin. See TRM for options */
#define OMAP_MUX_MODE0      0
#define OMAP_MUX_MODE1      1
#define OMAP_MUX_MODE2      2
#define OMAP_MUX_MODE3      3
#define OMAP_MUX_MODE4      4
#define OMAP_MUX_MODE5      5
#define OMAP_MUX_MODE6      6
#define OMAP_MUX_MODE7      7

/* 24xx/34xx mux bit defines */
#define OMAP_PULL_ENA			(1 << 3)
#define OMAP_PULL_UP			(1 << 4)
#define OMAP_ALTELECTRICALSEL		(1 << 5)

/* 34xx specific mux bit defines */
#define OMAP_INPUT_EN			(1 << 8)
#define OMAP_OFF_EN			(1 << 9)
#define OMAP_OFFOUT_EN			(1 << 10)
#define OMAP_OFFOUT_VAL			(1 << 11)
#define OMAP_OFF_PULL_EN		(1 << 12)
#define OMAP_OFF_PULL_UP		(1 << 13)
#define OMAP_WAKEUP_EN			(1 << 14)

/* 44xx specific mux bit defines */
#define OMAP_WAKEUP_EVENT		(1 << 15)

/* am33xx specific mux bit defines */
#define AM33XX_SLEWCTRL_FAST		(0 << 6)
#define AM33XX_SLEWCTRL_SLOW		(1 << 6)
#define AM33XX_INPUT_EN			(1 << 5)
#define AM33XX_PULL_UP			(1 << 4)
/* bit 3: 0 - enable, 1 - disable for pull enable */
#define AM33XX_PULL_DISA		(1 << 3)
#define AM33XX_PULL_ENBL		(0 << 3)

/* Definition of output pin could have pull disabled, but
 * this has not been done due to two reasons
 * 1. AM33XX_MUX will take care of it
 * 2. If pull was disabled for out macro, combining out & in pull on macros
 *    would disable pull resistor and AM33XX_MUX cannot take care of the
 *    correct pull setting and unintentionally pull would get disabled
 */
#define	AM33XX_PIN_OUTPUT		(0)
#define	AM33XX_PIN_OUTPUT_PULLUP	(AM33XX_PULL_UP)
#define	AM33XX_PIN_INPUT		(AM33XX_INPUT_EN | AM33XX_PULL_DISA)
#define	AM33XX_PIN_INPUT_PULLUP		(AM33XX_INPUT_EN | AM33XX_PULL_UP)
#define	AM33XX_PIN_INPUT_PULLDOWN	(AM33XX_INPUT_EN)

/* Active pin states */
#define OMAP_PIN_OUTPUT			0
#define OMAP_PIN_INPUT			OMAP_INPUT_EN
#define OMAP_PIN_INPUT_PULLUP		(OMAP_PULL_ENA | OMAP_INPUT_EN \
						| OMAP_PULL_UP)
#define OMAP_PIN_INPUT_PULLDOWN		(OMAP_PULL_ENA | OMAP_INPUT_EN)

/* Off mode states */
#define OMAP_PIN_OFF_NONE		0
#define OMAP_PIN_OFF_OUTPUT_HIGH	(OMAP_OFF_EN | OMAP_OFFOUT_EN \
						| OMAP_OFFOUT_VAL)
#define OMAP_PIN_OFF_OUTPUT_LOW		(OMAP_OFF_EN | OMAP_OFFOUT_EN)
#define OMAP_PIN_OFF_INPUT_PULLUP	(OMAP_OFF_EN | OMAP_OFF_PULL_EN \
						| OMAP_OFF_PULL_UP)
#define OMAP_PIN_OFF_INPUT_PULLDOWN	(OMAP_OFF_EN | OMAP_OFF_PULL_EN)
#define OMAP_PIN_OFF_WAKEUPENABLE	OMAP_WAKEUP_EN

#define OMAP_MODE_GPIO(x)	(((x) & OMAP_MUX_MODE7) == OMAP_MUX_MODE4)

/* Flags for omapX_mux_init */
#define OMAP_PACKAGE_MASK		0xffff
#define OMAP_PACKAGE_CBS		8		/* 547-pin 0.40 0.40 */
#define OMAP_PACKAGE_CBL		7		/* 547-pin 0.40 0.40 */
#define OMAP_PACKAGE_CBP		6		/* 515-pin 0.40 0.50 */
#define OMAP_PACKAGE_CUS		5		/* 423-pin 0.65 */
#define OMAP_PACKAGE_CBB		4		/* 515-pin 0.40 0.50 */
#define OMAP_PACKAGE_CBC		3		/* 515-pin 0.50 0.65 */
#define OMAP_PACKAGE_ZAC		2		/* 24xx 447-pin POP */
#define OMAP_PACKAGE_ZAF		1		/* 2420 447-pin SIP */


#define OMAP_MUX_NR_MODES		8		/* Available modes */
#define OMAP_MUX_NR_SIDES		2		/* Bottom & top */

/*
 * omap_mux_init flags definition:
 *
 * OMAP_MUX_REG_8BIT: Ensure that access to padconf is done in 8 bits.
 * The default value is 16 bits.
 * OMAP_MUX_GPIO_IN_MODE3: The GPIO is selected in mode3.
 * The default is mode4.
 */
#define OMAP_MUX_REG_8BIT		(1 << 0)
#define OMAP_MUX_GPIO_IN_MODE3		(1 << 1)
