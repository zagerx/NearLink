/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: usb vbus status detect(connect or disconnect) \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-24， Create file. \n
 */

#ifndef USB_STATUS_H
#define USB_STATUS_H

typedef enum {
    USB_CONNECTED,
    USB_DISCONNECTED,
} usb_vbus_status_t;

typedef void (*usb_status_callback_t)(usb_vbus_status_t status);

/**
 * @if Eng
 * @brief  It will call cb when vbus connect/disconnect.
 * @param  [in]  cb  vbus connect/disconnect callback.
 * @else
 * @brief  当vbus连接或者断开时会调用cb.
 * @param  [in]  cb  vbus 连接或断开的回调函数.
 * @endif
 */
void usb_status_init(usb_status_callback_t cb);


/**
 * @if Eng
 * @brief  To query the vbus insertion status.
 * @return true if usb vbus connected, otherwise false.
 * @else
 * @brief  查询vbus插入状态.
 * @return 如果vbus处于连接状态会返回true，否则返回false.
 * @endif
 */
bool usb_is_vbus_connected(void);
#endif