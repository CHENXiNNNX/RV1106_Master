# ESP-Hosted NG 主机驱动（SPI）— 输出 esp32_spi.ko 到 drv_ko/out，随 OEM ko 打包
#
# 源码路径（优先）：
#   1) 环境变量 ESP_HOSTED_NG_HOST_DIR
#   2) 默认：<rv1106-sdk>/esp-hosted/esp_hosted_ng/host（esp-hosted 放在 SDK 根目录下即可）
#   3) 备选：与 SDK 并列的 WorkSpace 下的 esp-hosted/esp_hosted_ng/host（未在 SDK 下找到 Makefile 时使用）
#
# 依赖：已编译内核模块树 $(SYSDRV_KERNEL_OBJS_OUTPUT_DIR)（先执行 sysdrv drv 中的 kernel modules）

ifeq ($(SYSDRV_PARAM), )
	SYSDRV_PARAM := ../../Makefile.param
	include $(SYSDRV_PARAM)
endif

ifeq ($(M_OUT_DIR),)
$(error Please build from drv_ko top directory)
endif

# Makefile.param 里 CURRENT_DIR 为 sysdrv 目录
ESP_CAND_SDK := $(abspath $(CURRENT_DIR)/../esp-hosted/esp_hosted_ng/host)
ESP_CAND_WS := $(abspath $(CURRENT_DIR)/../../../../esp-hosted/esp_hosted_ng/host)

ifeq ($(ESP_HOSTED_NG_HOST_DIR),)
  ifneq ($(wildcard $(ESP_CAND_SDK)/Makefile),)
    ESP_HOSTED_NG_HOST_DIR := $(ESP_CAND_SDK)
  else
    ESP_HOSTED_NG_HOST_DIR := $(ESP_CAND_WS)
  endif
endif

KDIR := $(SYSDRV_KERNEL_OBJS_OUTPUT_DIR)

.PHONY: all clean

all:
	@if [ ! -f "$(ESP_HOSTED_NG_HOST_DIR)/Makefile" ]; then \
		echo "esp_hosted: 未找到源码目录。请设置 ESP_HOSTED_NG_HOST_DIR，或确认存在:"; \
		echo "  $(ESP_CAND_SDK)"; \
		echo "（备选）$(ESP_CAND_WS)"; \
		exit 1; \
	fi
	@if [ ! -f "$(KDIR)/Makefile" ]; then \
		echo "esp_hosted: 内核构建目录不存在: $(KDIR)"; \
		exit 1; \
	fi
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(ESP_HOSTED_NG_HOST_DIR) target=spi modules
	@test -f "$(ESP_HOSTED_NG_HOST_DIR)/esp32_spi.ko" || (echo "esp_hosted: 未生成 esp32_spi.ko"; exit 1)
	cp -f "$(ESP_HOSTED_NG_HOST_DIR)/esp32_spi.ko" "$(M_OUT_DIR)/"

clean:
	@if [ -f "$(ESP_HOSTED_NG_HOST_DIR)/Makefile" ] && [ -f "$(KDIR)/Makefile" ]; then \
		$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(ESP_HOSTED_NG_HOST_DIR) target=spi clean; \
	fi
	rm -f "$(M_OUT_DIR)/esp32_spi.ko"
