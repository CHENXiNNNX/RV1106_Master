// SPDX-License-Identifier: GPL-2.0-only
/*
 * SPDX-FileCopyrightText: 2015-2023 Espressif Systems (Shanghai) CO LTD
 *
 */

#ifndef _ESP_SPI_H_
#define _ESP_SPI_H_

#include "esp.h"

/*
 * Luckfox RV1106 ↔ ESP32-S3 (custom wiring):
 * Handshake: 1106 GPIO1_C5 -> Linux GPIO 53 (S3 IO14)
 * Data ready: 1106 GPIO1_C4 -> Linux GPIO 52 (S3 IO15)
 * EN reset:   1106 GPIO1_B1 -> Linux GPIO 41 (module param resetpin=41)
 * SPI: host SPI0 on PC0-PC3; S3 side MOSI/MISO/CLK/CS = IO9/8/7/6
 * Mode: SPI_MODE_1 (CPOL=0, CPHA=1) — must match firmware g_spi_mode in spi_slave_api.c
 */
#define HANDSHAKE_PIN           53
#define SPI_IRQ                 gpio_to_irq(HANDSHAKE_PIN)
#define SPI_DATA_READY_PIN      52
#define SPI_DATA_READY_IRQ      gpio_to_irq(SPI_DATA_READY_PIN)
#define SPI_BUF_SIZE            1600

enum spi_flags_e {
	ESP_SPI_BUS_CLAIMED,
	ESP_SPI_BUS_SET,
	ESP_SPI_GPIO_HS_REQUESTED,
	ESP_SPI_GPIO_HS_IRQ_DONE,
	ESP_SPI_GPIO_DR_REQUESTED,
	ESP_SPI_GPIO_DR_IRQ_DONE,
	ESP_SPI_DATAPATH_OPEN,
};

struct esp_spi_context {
	struct esp_adapter          *adapter;
	struct spi_device           *esp_spi_dev;
	struct sk_buff_head         tx_q[MAX_PRIORITY_QUEUES];
	struct sk_buff_head         rx_q[MAX_PRIORITY_QUEUES];
	struct workqueue_struct     *spi_workqueue;
	struct work_struct          spi_work;
	struct workqueue_struct     *nw_cmd_reinit_workqueue;
	struct work_struct          nw_cmd_reinit_work;
	uint8_t                     spi_clk_mhz;
	uint8_t                     reserved[2];
	unsigned long               spi_flags;
};

enum {
	CLOSE_DATAPATH,
	OPEN_DATAPATH,
};


#endif
