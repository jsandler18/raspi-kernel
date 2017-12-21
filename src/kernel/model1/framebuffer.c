#include <kernel/framebuffer.h>
#include <kernel/gpu.h>
#include <kernel/mem.h>
#include <kernel/mailbox.h>

typedef struct {
	uint32_t width;
	uint32_t height;
	uint32_t vwidth;
	uint32_t vheight;
	uint32_t bytes;
	uint32_t depth;
	uint32_t ignorex;
	uint32_t ignorey;
	void * pointer;;
	uint32_t size;
} fb_init_t;

fb_init_t fbinit __attribute__((aligned(16)));

int framebuffer_init(void) {
	mail_message_t msg;

	fbinit.width = 640;
	fbinit.height = 480;
	fbinit.vwidth = fbinit.width;
	fbinit.vheight = fbinit.height;
	fbinit.depth = COLORDEPTH;

	msg.data = ((uint32_t)&fbinit + 0x40000000) >> 4;

	mailbox_send(msg, FRAMEBUFFER_CHANNEL);
	msg = mailbox_read(FRAMEBUFFER_CHANNEL);

	if (!msg.data)
		return -1;

	fbinfo.width = fbinit.width;
	fbinfo.height = fbinit.height;
	fbinfo.chars_width = fbinfo.width / CHAR_WIDTH; 
    fbinfo.chars_height = fbinfo.height / CHAR_HEIGHT;
    fbinfo.chars_x = 0;
    fbinfo.chars_y = 0;
	fbinfo.pitch = fbinit.bytes;
	fbinfo.buf = fbinit.pointer;
	fbinfo.buf_size = fbinit.size;

	return 0;
}
