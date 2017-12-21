#include <kernel/uart.h>
#include <kernel/kerio.h>
#include <kernel/gpu.h>
#include <common/stdlib.h>
#include <stdarg.h>

char getc(void) {
    return uart_getc();
}

void putc(char c) {
    gpu_putc(c);
}

void puts(const char * str) {
    int i;
    for (i = 0; str[i] != '\0'; i ++)
        putc(str[i]);
}

void gets(char * buf, int buflen) {
    int i;
    char c;
    // Leave a spot for null char in buffer
    for (i = 0; (c = getc()) != '\r' && buflen > 1; i++, buflen--) {
        putc(c);
        buf[i] = c;
    }

    putc('\n');
    if (c == '\n') {
        buf[i] = '\0';
    }
    else
        buf[buflen-1] = '\0';
}

void printf(const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (; *fmt != '\0'; fmt++) {
        if (*fmt == '%') {
            switch (*(++fmt)) {
                case '%':
                    putc('%');
                    break;
                case 'd':
                    puts(itoa(va_arg(args, int), 10));
                    break;
                case 'x':
                    puts(itoa(va_arg(args, int), 16));
                    break;
                case 's':
                    puts(va_arg(args, char *));
                    break;
            }
        } else putc(*fmt);
    }

    va_end(args);
}
