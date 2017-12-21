#include <stddef.h>
#include <stdint.h>
#include <kernel/peripheral.h>

#ifndef UART_H
#define UART_H


typedef union uart_flags {
    struct {
        uint8_t clear_to_send: 1;
        uint8_t data_set_ready: 1;
        uint8_t data_carrier_detected: 1;
        uint8_t busy: 1;
        uint8_t recieve_queue_empty: 1;
        uint8_t transmit_queue_full: 1;
        uint8_t recieve_queue_full: 1;
        uint8_t transmit_queue_empty: 1;
        uint8_t ring_indicator: 1;
        uint32_t padding: 23;
    };
    uint32_t as_int;
} uart_flags_t;

typedef union uart_control {
    struct {
        uint8_t uart_enabled: 1;
        uint8_t sir_enabled: 1;
        uint8_t sir_low_power_mode: 1;
        uint8_t reserved: 4;
        uint8_t loop_back_enabled: 1;
        uint8_t transmit_enabled: 1;
        uint8_t receive_enabled: 1;
        uint8_t data_transmit_ready: 1;
        uint8_t request_to_send: 1;
        uint8_t out1: 1;
        uint8_t out2: 1;
        uint8_t rts_hardware_flow_control_enabled: 1;
        uint8_t cts_hardware_flow_control_enabled: 1;
        uint16_t padding;
    };
    uint32_t as_int;
} uart_control_t;

void mmio_write(uint32_t reg, uint32_t data);

uint32_t mmio_read(uint32_t reg);

// Loop <delay> times in a way that the compiler won't optimize away
void delay(int32_t count);

enum
{
    // The GPIO registers base address.
    GPIO_BASE = PERIPHERAL_BASE + GPIO_OFFSET,
    // The offsets for reach register.

    // Controls actuation of pull up/down to ALL GPIO pins.
    GPPUD = (GPIO_BASE + 0x94),

    // Controls actuation of pull up/down for specific GPIO pin.
    GPPUDCLK0 = (GPIO_BASE + 0x98),

    // The base address for UART.
    UART0_BASE = PERIPHERAL_BASE + UART0_OFFSET,

    // The offsets for reach register for the UART.
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
};

void uart_init();

uart_flags_t read_flags(void);

void uart_putc(unsigned char c);

unsigned char uart_getc();

void uart_puts(const char* str);

#endif
