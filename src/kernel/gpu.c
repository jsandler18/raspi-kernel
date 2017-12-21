#include <kernel/framebuffer.h>
#include <kernel/gpu.h>
#include <kernel/mem.h>
#include <kernel/kerio.h>
#include <kernel/mailbox.h>
#include <kernel/chars_pixels.h>
#include <common/stdlib.h>


const pixel_t RED = {0xff, 0x00, 0x00};
const pixel_t GREEN = {0x00, 0xff, 0x00};
const pixel_t BLUE = {0x00, 0x00, 0xff};
const pixel_t WHITE = {0xff, 0xff, 0xff};
const pixel_t BLACK = {0x00, 0x00, 0x00};

void write_pixel(uint32_t x, uint32_t y, const pixel_t * pix) {
     void * location = fbinfo.buf + y*fbinfo.pitch + x*BYTES_PER_PIXEL;
     //uint16_t p = (pix->blue & 0x1f) | ((pix->green & 0x3f) << 5) | ((pix->red & 0x1f) << 11);

     memcpy(location, pix, BYTES_PER_PIXEL);
}

void gpu_putc(char c) {
    uint8_t w,h;
	uint8_t mask, * bmp = font[(int)c];

	if (c == '\n') {
        fbinfo.chars_x = 0;
        fbinfo.chars_y++;
		return;
	}

    for(w = 0; w < CHAR_WIDTH; w++) {
        for(h = 0; h < CHAR_HEIGHT; h++) {
			mask = 1 << (w);
           if (bmp[h] & mask)
               write_pixel(fbinfo.chars_x*CHAR_WIDTH + w, fbinfo.chars_y*CHAR_HEIGHT + h + 100, &WHITE);
           else
               write_pixel(fbinfo.chars_x*CHAR_WIDTH + w, fbinfo.chars_y*CHAR_HEIGHT + h + 100, &BLACK);
        }
    }

    fbinfo.chars_x++;
    if (fbinfo.chars_x > fbinfo.chars_width) {
        fbinfo.chars_x = 0;
        fbinfo.chars_y++;
    }
}

void gpu_init(void) {
    // Aparantly, this sometimes does not work, so try in a loop
    while(framebuffer_init());

    // clear screen
    pixel_t pix;
    pix.green = 0;
    for (int i = 0; i < fbinfo.width; i++) {
            pix.blue = (398 * (fbinfo.width - i))/1000;
            pix.green = (398*i)/1000;
        for (int j = 0; j < fbinfo.height; j++) {
            write_pixel(i,j,&pix);
        }
    }
}
