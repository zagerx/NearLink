/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test usb source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-09, Create file. \n
 */
#include "gadget/f_hid.h"
#include "test_usb_hid.h"

#define input(size)             (0x80 | (size))
#define output(size)            (0x90 | (size))
#define feature(size)           (0xb0 | (size))
#define collection(size)        (0xa0 | (size))
#define end_collection(size)    (0xc0 | (size))

/* Global items */
#define usage_page(size)        (0x04 | (size))
#define logical_minimum(size)   (0x14 | (size))
#define logical_maximum(size)   (0x24 | (size))
#define physical_minimum(size)  (0x34 | (size))
#define physical_maximum(size)  (0x44 | (size))
#define uint_exponent(size)     (0x54 | (size))
#define uint(size)              (0x64 | (size))
#define report_size(size)       (0x74 | (size))
#define report_id(size)         (0x84 | (size))
#define report_count(size)      (0x94 | (size))
#define push(size)              (0xa4 | (size))
#define pop(size)               (0xb4 | (size))

/* Local items */
#define usage(size)                 (0x08 | (size))
#define usage_minimum(size)         (0x18 | (size))
#define usage_maximum(size)         (0x28 | (size))
#define designator_index(size)      (0x38 | (size))
#define designator_minimum(size)    (0x48 | (size))
#define designator_maximum(size)    (0x58 | (size))
#define string_index(size)          (0x78 | (size))
#define string_minimum(size)        (0x88 | (size))
#define string_maximum(size)        (0x98 | (size))
#define delimiter(size)             (0xa8 | (size))

static uint8_t g_report_desc_hid[ ] = {
    usage_page(1),      0x01,
    usage(1),           0x06,
    collection(1),      0x01,
    report_id(1),       0x01,

    usage_page(1),      0x07,
    usage_minimum(1),   0xE0,
    usage_maximum(1),   0xE7,
    logical_minimum(1), 0x00,
    logical_maximum(1), 0x01,
    report_size(1),     0x01,
    report_count(1),    0x08,
    input(1),           0x02,

    report_count(1),    0x01,
    report_size(1),     0x08,
    input(1),           0x01,

    report_count(1),    0x05,
    report_size(1),     0x01,
    usage_page(1),      0x08,
    usage_minimum(1),   0x01,
    usage_maximum(1),   0x05,
    output(1),          0x02,
    report_count(1),    0x01,
    report_size(1),     0x03,
    output(1),          0x01,

    report_count(1),    0x06,
    report_size(1),     0x08,
    logical_minimum(1), 0x00,
    logical_maximum(1), 0x65,
    usage_page(1),      0x07,
    usage_minimum(1),   0x00,
    usage_maximum(1),   0x65,
    input(1),           0x00,

    end_collection(0),

    usage_page(2), 0xB1, 0xFF,
    usage(1),           0x1,
    collection(1),      0x01,
    report_id(1),       0x08,
    collection(1),      0x00,
    report_count(1),    0xc,
    report_size(1),     0x8,
    usage_minimum(1),   0x0,
    usage_maximum(1),   0xFF,
    logical_minimum(1), 0,
    logical_maximum(1), 0xff,
    output(1),           2,
    end_collection(0),
    end_collection(0),
    usage_page(2), 0xB2, 0xFF,
    usage(1),           0x1,
    collection(1),      0x01,
    report_id(1),       0x09,
    collection(1),      0x00,
    report_count(1),    0x3f,
    report_size(1),     0x8,
    usage_minimum(1),   0x0,
    usage_maximum(1),   0xFF,
    logical_minimum(1), 0,
    logical_maximum(1), 0xff,
    output(1),           2,
    usage(1),           0x2,
    report_count(1),    0x3f,
    report_size(1),     0x8,
    usage_minimum(1),   0x0,
    usage_maximum(1),   0xFF,
    logical_minimum(1), 0,
    logical_maximum(1), 0xff,
    input(1),           0,
    end_collection(0),
    end_collection(0),
};

static uint8_t g_usb_hid_mouse_desc[] = {
    usage_page(1),      0x01,
    usage(1),           0x02,
    collection(1),      0x01,
    usage(1),           0x01,

    report_count(1),    0x03,
    report_size(1),     0x01,
    usage_page(1),      0x09,
    usage_minimum(1),   0x1,
    usage_maximum(1),   0x3,
    logical_minimum(1), 0x00,
    logical_maximum(1), 0x01,
    input(1),           0x02,
    report_count(1),    0x01,
    report_size(1),     0x05,
    input(1),           0x01,
    usage_page(1),      0x01,
    usage(1),           0x38,
    report_count(1),    0x01,
    report_size(1),     0x08,
    logical_minimum(1), 0x81,
    logical_maximum(1), 0x7f,
    input(1),           0x06,
    usage(1),           0x30,
    usage(1),           0x31,
    report_count(1),    0x02,
    report_size(1),     0x10,
    logical_minimum(2), 0x01, 0x80,
    logical_maximum(2), 0xff, 0x7f,
    input(1),           0x06,
    end_collection(0)
};


int tesetsuit_usb_add_report_desc_hid(usb_hid_type_t type)
{
    if (type == USB_HID_KEYBOARD) {
        return hid_add_report_descriptor(g_report_desc_hid, sizeof(g_report_desc_hid), 1);
    } else {
        return hid_add_report_descriptor(g_usb_hid_mouse_desc, sizeof(g_usb_hid_mouse_desc), 0);
    }
}
