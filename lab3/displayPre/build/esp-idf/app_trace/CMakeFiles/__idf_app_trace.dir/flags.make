# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# compile C with C:/Users/aulasingenieria/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20230928/xtensa-esp-elf/bin/xtensa-esp32-elf-gcc.exe
C_DEFINES = -DESP_PLATFORM -DIDF_VER=\"v5.2.1\" -DSOC_MMU_PAGE_SIZE=CONFIG_MMU_PAGE_SIZE -DSOC_XTAL_FREQ_MHZ=CONFIG_XTAL_FREQ -D_GLIBCXX_HAVE_POSIX_SEMAPHORE -D_GLIBCXX_USE_POSIX_SEMAPHORE -D_GNU_SOURCE -D_POSIX_READER_WRITER_LOCKS

C_INCLUDES = -IC:/Users/aulasingenieria/Documents/GitHub/progrmacionSE/lab3/displayPre/build/config -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/app_trace/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/app_trace/private_include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/app_trace/port/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/newlib/platform_include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/freertos/config/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/freertos/config/include/freertos -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/freertos/config/xtensa/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/freertos/FreeRTOS-Kernel/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/include/freertos -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/freertos/esp_additions/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_hw_support/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_hw_support/include/soc -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_hw_support/include/soc/esp32 -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_hw_support/port/esp32/. -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/heap/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/log/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/soc/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/soc/esp32 -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/soc/esp32/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/hal/platform_port/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/hal/esp32/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/hal/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_rom/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_rom/include/esp32 -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_rom/esp32 -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_common/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_system/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_system/port/soc -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_system/port/include/private -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/xtensa/esp32/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/xtensa/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/xtensa/deprecated_include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/lwip/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/lwip/include/apps -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/lwip/include/apps/sntp -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/lwip/lwip/src/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/lwip/port/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/lwip/port/freertos/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/lwip/port/esp32xx/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/lwip/port/esp32xx/include/arch -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/lwip/port/esp32xx/include/sys -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_timer/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/deprecated -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/analog_comparator/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/dac/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/gpio/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/gptimer/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/i2c/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/i2s/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/ledc/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/mcpwm/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/parlio/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/pcnt/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/rmt/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/sdio_slave/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/sdmmc/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/sigma_delta/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/spi/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/temperature_sensor/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/touch_sensor/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/twai/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/uart/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/usb_serial_jtag/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/driver/touch_sensor/esp32/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_pm/include -IC:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esp_ringbuf/include

C_FLAGS = -mlongcalls -Wno-frame-address  -fdiagnostics-color=always -ffunction-sections -fdata-sections -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=unused-but-set-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-enum-conversion -gdwarf-4 -ggdb -Og -fno-shrink-wrap -fmacro-prefix-map=C:/Users/aulasingenieria/Documents/GitHub/progrmacionSE/lab3/displayPre=. -fmacro-prefix-map=C:/Users/aulasingenieria/esp/v5.2.1/esp-idf=/IDF -fstrict-volatile-bitfields -fno-jump-tables -fno-tree-switch-conversion -std=gnu17 -Wno-old-style-declaration
