#include <stdint.h>
#include <kernel/peripheral.h>
#ifndef EMMC_H
#define EMMC_H

#define EMMC_BASE (PERIPHERAL_BASE + EMMC_OFFSET)

#define AUTO_NO_COMMAND 0
#define AUTO_COMMAND_CMD12 1
#define AUTO_COMMAND_CMD23 2
#define AUTO_RESERVED 3
#define DIRECTION_HOST_TO_CARD 0
#define DIRECTION_CARD_TO_HOST 1
#define RESPONSE_NO_RESPONSE 0
#define RESPONSE_136_BIT_RESPONSE 1
#define RESPONSE_48_BIT_RESPONSE 2
#define RESPONSE_48_BIT_BUSY_RESPONSE 3
#define COMMAND_NORMAL 0
#define COMMAND_SUSPEND_CURRENT 1
#define COMMAND_RESUME_LAST 2
#define COMMAND_ABORT_CURRENT 3
#define SD_CLOCK_GEN_DIVIDED 0
#define SD_CLOCK_GEN_PROGRAMMABLE 1
#define DATA_TIMEOUT_EXPONENT_DISABLED 15
#define SD_SPEED_SDR12 0
#define SD_SPEED_SDR25 1
#define SD_SPEED_SDR50 2
#define SD_SPEED_SDR104 3
#define SD_SPEED_DDR50 4


typedef struct {
    uint32_t block_size: 10;
    uint32_t reserved: 6;
    uint32_t block_count: 16;
} block_size_count_t;


typedef struct {
    uint32_t reserved0: 1;
    uint32_t enable_block_counter: 1;
    uint32_t auto_command: 2;
    uint32_t transfer_direction: 1;
    uint32_t transfer_multi_block: 1;
    uint32_t reserved1: 10;
    uint32_t response_type: 2;
    uint32_t reserved2: 1;
    uint32_t check_response_crc: 1;
    uint32_t check_response_index: 1;
    uint32_t command_has_data: 1;
    uint32_t command_type: 2;
    uint32_t command_index: 6;
    uint32_t reserved3: 2;
} command_transfer_mode_t;

typedef struct {
    uint32_t reserved0: 1;
    uint32_t use_4_data_lines: 1;
    uint32_t high_speed_mode: 1;
    uint32_t reserved1: 2;
    uint32_t use_8_data_lines: 1;
    uint32_t reserved2: 10;
    uint32_t stop_next_gap: 1;
    uint32_t restart_from_gap: 1;
    uint32_t use_dat2_readwait: 1;
    uint32_t enable_sdio_interrupt: 1;
    uint32_t enable_spi_mode: 1;
    uint32_t start_boot_access_mode: 1;
    uint32_t enable_alt_boot_access_mode: 1;
    uint32_t reserved3: 9;
} control_0_t;

typedef struct {
    uint32_t emmc_clock_enable: 1;
    uint32_t sd_clock_stable: 1;
    uint32_t sd_clock_enable: 1;
    uint32_t reserved0: 2;
    uint32_t sd_clock_generation_mode: 1;
    uint32_t sd_clock_base_divider_most_sig_bits: 2;
    uint32_t sd_clock_base_divider_least_sig_bits: 8;
    uint32_t data_timeout_exponent: 4;
    uint32_t reserved1: 4;
    uint32_t reset_complete_host_circuit: 1;
    uint32_t reset_command_handle_circuit: 1;
    uint32_t reset_data_handle_circuit: 1;
    uint32_t reserved2: 5;
} control_1_t;

typedef struct {
    uint32_t auto_command_not_executed: 1;
    uint32_t auto_command_timeout: 1;
    uint32_t auto_command_crc_error: 1;
    uint32_t auto_command_line_end_error: 1;
    uint32_t auto_command_index_error: 1;
    uint32_t reserved0: 2;
    uint32_t auto_command_cmd12_error: 1;
    uint32_t reserved1: 8;
    uint32_t sd_speed_mode: 3;
    uint32_t reserved2: 3;
    uint32_t start_sd_clock_tuning: 1;
    uint32_t tuned_clock_used_for_sampling: 1;
    uint32_t reserved3: 8;
} control_2_t;

typedef struct {
    uint32_t command_done: 1;
    uint32_t data_transfer_done: 1;
    uint32_t transfer_stopped_at_block_gap: 1;
    uint32_t reserved0: 1;
    uint32_t can_write: 1;
    uint32_t can_read: 1;
    uint32_t reserved1: 2;
    uint32_t card_made_irq: 1;
    uint32_t reserved2: 3;
    uint32_t clock_retune_request: 1;
    uint32_t boot_ack_received: 1;
    uint32_t boot_ended: 1;
    uint32_t error: 1;
    uint32_t command_line_timeout: 1;
    uint32_t command_crc_error: 1;
    uint32_t command_line_end_error: 1;
    uint32_t incorrect_response_index: 1;
    uint32_t data_line_timeout: 1;
    uint32_t data_crc_error: 1;
    uint32_t data_line_end_error: 1;
    uint32_t reserved3: 1;
    uint32_t auto_command_error: 1;
    uint32_t reserved4: 7;
} emmc_interrupts_t;

typedef struct {
    uint32_t arg2;
    block_size_count_t block_size_count;
    uint32_t arg1;
    command_transfer_mode_t command_transfer_mode;
    uint32_t response_0;
    uint32_t response_1;
    uint32_t response_2;
    uint32_t response_3;
    uint32_t data;
    uint32_t debug;
    control_0_t control_0;
    control_1_t control_1;
    emmc_interrupts_t interrupts_occured_flags;
    emmc_interrupts_t enable_set_flags;
    emmc_interrupts_t enable_irq;
    control_2_t control_2;
    emmc_interrupts_t fake_interrupt;
    uint32_t boot_mode_timeout;
    uint32_t debug_select;
    uint32_t extended_read_fifo_config;
    uint32_t extended_read_fifo_enable;
    uint32_t sd_clock_tune_step;
    uint32_t sd_clock_tune_sdr_delay;
    uint32_t sd_clock_tune_ddr_delay;
    uint32_t spi_interrupt_enable;
    uint32_t version_info;
} emmc_registers_t;

#endif
