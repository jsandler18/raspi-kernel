#include <kernel/peripheral.h>
#include <stdint.h>
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

typedef struct framebuffer_info {
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    void * buf;
    uint32_t buf_size;
    uint32_t chars_width;
    uint32_t chars_height;
    uint32_t chars_x;
    uint32_t chars_y;
} framebuffer_info_t;

typedef struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_t;


void gpu_init(void);

void gpu_putc(char c);

#endif
