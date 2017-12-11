#include <stddef.h>
#include <stdint.h>

#ifndef UART_H
#define UART_H

struct uart_flags {
    uint8_t cts: 1;
    uint8_t dsr: 1;
    uint8_t dcd: 1;
    uint8_t busy: 1;
    uint8_t rxfe: 1;
    uint8_t txff: 1;
    uint8_t rxff: 1;
    uint8_t ri: 1;
    uint32_t padding: 24;
};

void mmio_write(uint32_t reg, uint32_t data);

uint32_t mmio_read(uint32_t reg);

// Loop <delay> times in a way that the compiler won't optimize away
void delay(int32_t count);

enum
{
    // The GPIO registers base address.
    GPIO_BASE = 0x3F200000, // for raspi2 & 3, 0x20200000 for raspi1


    // The offsets for reach register.

    // Controls actuation of pull up/down to ALL GPIO pins.
    GPPUD = (GPIO_BASE + 0x94),

    // Controls actuation of pull up/down for specific GPIO pin.
    GPPUDCLK0 = (GPIO_BASE + 0x98),

    // The base address for UART.
    UART0_BASE = 0x3F201000, // for raspi2 & 3, 0x20201000 for raspi1

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

struct uart_flags read_flags(void);

void uart_putc(unsigned char c);

unsigned char uart_getc();

void uart_puts(const char* str);
#endif
