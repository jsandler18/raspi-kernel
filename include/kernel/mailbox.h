#include <stdint.h>
#include <kernel/peripheral.h>

#ifndef MAILBOX_H
#define MAILBOX_H

#define MAILBOX_BASE PERIPHERAL_BASE + MAILBOX_OFFSET
#define MAIL0_READ (((mail_message_t *)(0x00 + MAILBOX_BASE)))
#define MAIL0_STATUS (((mail_status_t *)(0x18 + MAILBOX_BASE)))
#define MAIL0_WRITE (((mail_message_t *)(0x20 + MAILBOX_BASE)))
#define PROPERTY_CHANNEL 8
#define FRAMEBUFFER_CHANNEL 1

typedef struct {
    uint8_t channel: 4;
    uint32_t data: 28;
} mail_message_t;

typedef struct {
    uint32_t reserved: 30;
    uint8_t empty: 1;
    uint8_t full:1;
} mail_status_t;

mail_message_t mailbox_read(int channel);
void mailbox_send(mail_message_t msg, int channel);

/**
 * A property message can either be a request, or a response, and a response can be successfull or an error
 */
typedef enum {
    REQUEST = 0x00000000,
    RESPONSE_SUCCESS = 0x80000000,
    RESPONSE_ERROR = 0x80000001
} buffer_req_res_code_t;


/*
 * A buffer that holds many property messages.
 * The last tag must be a 4 byte zero, and then padding to make the whole thing 4 byte aligned
 */
typedef struct {
    uint32_t size;                      // Size includes the size itself
    buffer_req_res_code_t req_res_code;
    uint32_t tags[1];                    // A concatenated sequence of tags. will use overrun to make large enough
} property_message_buffer_t;


/**
 * A message is identified by a tag. These are some of the possible tags
 */
typedef enum {
    NULL_TAG = 0,
    FB_ALLOCATE_BUFFER = 0x00040001,
    FB_RELESE_BUFFER = 0x00048001,
    FB_GET_PHYSICAL_DIMENSIONS = 0x00040003,
    FB_SET_PHYSICAL_DIMENSIONS = 0x00048003,
    FB_GET_VIRTUAL_DIMENSIONS = 0x00040004,
    FB_SET_VIRTUAL_DIMENSIONS = 0x00048004,
    FB_GET_BITS_PER_PIXEL = 0x00040005,
    FB_SET_BITS_PER_PIXEL = 0x00048005,
    FB_GET_BYTES_PER_ROW = 0x00040008
} property_tag_t;

/**
 * For each possible tag, we create a struct corresponding to the request value buffer, and the response value buffer
 */

typedef struct {
    void * fb_addr;
    uint32_t fb_size;
} fb_allocate_res_t;

typedef struct {
    uint32_t width;
    uint32_t height;
} fb_screen_size_t;


/*
 * The value buffer can be any one of these types
 */
typedef union {
    uint32_t fb_allocate_align;
    fb_allocate_res_t fb_allocate_res;
    fb_screen_size_t fb_screen_size;
    uint32_t fb_bits_per_pixel;
    uint32_t fb_bytes_per_row;
} value_buffer_t;

/*
 * A message_buffer can contain any number of these
 */
typedef struct {
    property_tag_t proptag;
    value_buffer_t value_buffer;
} property_message_tag_t;


/**
 * given an array of tags, will send all of the tags given, and will populate that array with the responses.
 * the given array should end with a "null tag" with the proptag field set to 0.
 * returns 0 on success
 */
int send_messages(property_message_tag_t * tags);

#endif
