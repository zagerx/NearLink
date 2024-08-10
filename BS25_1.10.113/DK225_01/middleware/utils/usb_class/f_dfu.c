/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: LiteOS USB Driver DFU Protocol
 * Author: Huawei LiteOS Team
 * Create: 2017-12-25
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include <los_atomic.h>
#include "usb_os_adapt.h"
#include UART_HEADER_PATH
#include "gadget/f_dfu_pri.h"
#include "gadget/composite.h"

static int usbclass_dfu_setup(struct usbdevclass_driver_s *driver, struct usbdev_s *dev,
                              const struct usb_ctrlreq_s *ctrl, uint8_t *dataout, size_t outlen);
static int usbclass_dfu_bind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev);
static int usbclass_dfu_unbind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev);
static void usbclass_dfu_disconnect(struct usbdevclass_driver_s *driver, struct usbdev_s *dev);

static void dfu_mode_change(void);
#ifndef CONFIG_DRIVERS_USB_DFU_DOWNLOAD_CALLBACK
static int usb_dfu_write(struct usb_dfu_entity *dfu, const void *buf, int size, uint32_t blk_seq_num);
#endif

static struct usb_dfu_dev *g_dfu_dev = NULL;
static struct usb_dfu_entity g_dfu_entity;
static Atomic g_dfu_status = DFU_FREE_STATUS;
static uint8_t g_dfu_init_event;
static EVENT_CB_S g_dfu_finish_event;

volatile uint8_t g_flash_upgrade_state = DFU_FLASH_STATE_UPGRADING;

/* define descriptor */

#define DFU_DETACH_TIMEOUT      255
#define DFU_TRANSFER_SIZE       USB_COMP_EP0_BUFSIZ
#define DFU_VERSION             0x0110

struct dfu_config_desc
{
  struct usb_cfgdesc_s config_desc;
  struct usb_ifdesc_s if_desc;
  struct dfu_func_desc func_desc;
};

static struct usb_devdesc_s g_dfu_device_desc =
{
  .len          = sizeof(struct usb_devdesc_s),
  .type         = USB_DESC_TYPE_DEVICE,
  HSETW(.usb, 0x0200),
  .classid      = 0x02,
  .subclass     = 0x02, /* CDC-modem */
  .protocol     = 0x01, /* Interface Association Descriptor */
  .mxpacketsize = 64,   /* Control Endpoint packet size */
  HSETW(.vendor,  0),
  HSETW(.product, 0),
  HSETW(.device, DFU_VERSION),
  .imfgr        = 1,    /* Manufacturer name, string index */
  .iproduct     = 2,
  .serno        = 3,
  .nconfigs     = 1,
};

static const struct usbdevclass_driverops_s g_dfu_driverops =
{
  usbclass_dfu_bind,       /* bind */
  usbclass_dfu_unbind,     /* unbind */
  usbclass_dfu_setup,      /* setup */
  usbclass_dfu_disconnect, /* disconnect */
  NULL,                    /* suspend */
  NULL                     /* resume */
};

static struct dfu_config_desc g_dfu_config_desc =
{
  {
    .len         = USB_SIZEOF_CFGDESC,
    .type        = USB_DESC_TYPE_CONFIG,
    HSETW(.totallen, sizeof(struct dfu_config_desc)),               /* Modify according to the actual length */
    .ninterfaces = 1,
    .cfgvalue    = 1,
    .icfg        = 0,
    .attr        = USB_CONFIG_ATTR_SELFPOWER | USB_CONFIG_ATTR_ONE, /* 0xc0 */
    .mxpower     = 1                                                /* Max power */
  },
  {
    .len      = sizeof(struct usb_ifdesc_s),
    .type     = USB_DESC_TYPE_INTERFACE,
    .ifno     = 0,
    .alt      = 0,
    .neps     = 0,
    .classid  = 0xfe,
    .subclass = 0x01,
    .protocol = 0x01, /* DFU runtime protocol */
    .iif      = 0x01
  },
  {
    .bFunctionLength = sizeof(struct dfu_func_desc),
    .bDescriptorType = 0x21,
    .bAttributes     = 0x0b,
    HSETW(.bDetachTimeout, DFU_DETACH_TIMEOUT),
    HSETW(.bTransferSize, DFU_TRANSFER_SIZE),
    HSETW(.bDfuVersion, DFU_VERSION)
  }
};

#define DFU_STRING_ID_LEN 4
static const char g_dfu_string_id[DFU_STRING_ID_LEN] =
{
  DFU_STRING_ID_LEN,
  USB_DESC_TYPE_STRING,
  0x09, 0x04
};

#ifdef CONFIG_DRIVERS_USB_DFU_DRIVE_FREE
#define CONFIG_COMPOSITE_MSFT_OS_DESCRIPTORS
#endif
#ifdef CONFIG_DRIVERS_USB_DFU_DRIVE_FREE
#define OS_STRING_SIGN_LEN 18
static const char g_dfu_msft_string[OS_STRING_SIGN_LEN] =
{
  OS_STRING_SIGN_LEN,
  USB_DESC_TYPE_STRING,
  'M', 0, 'S', 0, 'F', 0, 'T', 0,
  '1', 0, '0', 0, '0', 0, 0xEE, 0
};
#define DEF_MSFT_STRING { 0xEE, g_dfu_msft_string }
#endif


static struct usbd_string g_dfu_string_defs[] =
{
  { 0, g_dfu_string_id },
  { 1, NULL },
  { 2, NULL },
  { 3, NULL },
#if CONFIG_DRIVERS_USB_DFU_DRIVE_FREE
  DEF_MSFT_STRING,
#endif
  USBD_DEVICE_STRINGS_END /* end of list */
};

#ifdef CONFIG_DRIVERS_USB_DFU_DRIVE_FREE
static const struct usb_os_msft_proerty_desc g_dfu_msft_proerty =
{
  .dwSize = sizeof(struct usb_os_msft_proerty_desc),
  .dwPropertyDataType = MULTI_LINE_STRING_TYPE,
  .wPropertyNameLength = STRING_PROPERTY_NAME_LEN,
  .bPropertyName =
    {
      'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0,
      'e', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0,
      'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0,
      'G', 0, 'U', 0, 'I', 0, 'D', 0, 's', 0,
      0, 0,
    }, /* "DeviceInterfaceGUIDs" */
  .dwPropertyDataLength = STRING_GUID_LEN,
  .bPropertyData =
    {
      '{', 0, '1', 0, '2', 0, '3', 0, '4', 0,
      '5', 0, '6', 0, '7', 0, '8', 0, '-', 0,
      '1', 0, '2', 0, '3', 0, '4', 0, '-', 0,
      '1', 0, '2', 0, '3', 0, '4', 0, '-', 0,
      '1', 0, '2', 0, '3', 0, '4', 0, '5', 0,
      '6', 0, '7', 0, '8', 0, '9', 0, 'A', 0,
      'D', 0, 'C', 0, '}', 0, 0, 0, 0, 0,
    }, /* "{12345678-1234-1234-123456789ADC}" It is self-configurable if needed. */
};

static int dfu_get_msft_proerty_desc(uint8_t *buf)
{
  struct usb_msft_os_extprop_hdr_s *dfu_hdr = (struct usb_msft_os_extprop_hdr_s *)buf;
  uint32_t totallen = sizeof(struct usb_msft_os_extprop_hdr_s) + sizeof(struct usb_os_msft_proerty_desc);
  int ret = -1;
  if (totallen > USB_COMP_EP0_BUFSIZ)
    {
      return ret;
    }
  ret = memset_s(buf, USB_COMP_EP0_BUFSIZ, 0, totallen);
  if (ret != EOK)
    {
      return ret;
    }
  dfu_hdr->len[0] = totallen;
  dfu_hdr->version[1] = 1;
  dfu_hdr->index[0] = MSFTOSDESC_INDEX_EXTPROP;
  dfu_hdr->count[0] = 1;
  ret = memcpy_s(buf + sizeof(struct usb_msft_os_extprop_hdr_s),
                 USB_COMP_EP0_BUFSIZ - sizeof(struct usb_msft_os_extprop_hdr_s),
                 &g_dfu_msft_proerty, sizeof(struct usb_os_msft_proerty_desc));
  if (ret != EOK)
    {
      usb_err("memcpy_s fail!\n");
      return ret;
    }
  return totallen;
}
#endif

#ifdef CONFIG_DRIVERS_USB_DFU_DOWNLOAD_CALLBACK
/* This function is implemented by the application layer according to the business scenario.
 * NOTE: The function that triggered the dispatch cannot be used in this function. For example, the mutex lock.
 */
__attribute__((weak)) void
usb_dfu_download_callback(const uint8_t *buf, uint32_t len)
{

}
#endif

static void to_runtime_mode(void)
{
  g_dfu_config_desc.if_desc.protocol = 0x01; /* The runtime mode */
}

static void to_dfu_mode(void)
{
  g_dfu_config_desc.if_desc.protocol = 0x02; /* The DFU mode */
}

void dfu_get_device_dec_info(struct usbd_string **device_strings, struct usb_devdesc_s **device_desc)
{
  *device_strings = &g_dfu_string_defs[1];
  *device_desc    = &g_dfu_device_desc;
}

void set_flash_state(volatile uint8_t flash_state)
{
  if (flash_state != DFU_FLASH_STATE_UPGRADING &&
      flash_state != DFU_FLASH_STATE_UPGRADED &&
      flash_state != DFU_FLASH_STATE_ERROR)
    {
      usb_err("The state of flash upgrade is set failed!\n");
      return;
    }

  g_flash_upgrade_state = flash_state;
}

void get_flash_state(const struct usbdev_req_s *req)
{
  uint8_t *buf = req->buf;

  buf[0] = g_flash_upgrade_state; /* The 0 bit is valid */
}

struct usb_dfu_entity *usb_dfu_get_entity(int alter)
{
  (void)alter;
  if (LOS_AtomicRead(&g_dfu_status) == DFU_FREE_STATUS)
    {
      return NULL;
    }
  return &g_dfu_entity;
}

bool usb_dfu_running(void)
{
#ifdef CONFIG_DRIVERS_USB_DFU_DOWNLOAD_CALLBACK
  if (LOS_AtomicCmpXchg32bits(&g_dfu_status, DFU_FREE_STATUS, DFU_IDLE_STATUS))
    {
      return true;
    }

  (void)LOS_EventWrite(&g_dfu_finish_event, USB_DFU_FREE_EVENT);
  return false;
#else
  return (LOS_AtomicRead(&g_dfu_status) == DFU_FREE_STATUS) ? false : true;
#endif
}

uint32_t usb_dfu_update_status(void)
{
  struct usb_dfu_entity *entity = usb_dfu_get_entity(0);

  return (entity == NULL) ? 0 : entity->trans_complete;
}

#ifndef CONFIG_DRIVERS_USB_DFU_DOWNLOAD_CALLBACK
uint64_t *usb_dfu_update_size_get(void)
{
  struct usb_dfu_entity *entity = usb_dfu_get_entity(0);

  return (entity == NULL) ? NULL : &entity->trans_size;
}

static int dfu_ram_read(uint8_t *buf, uint32_t len)
{
  struct usb_dfu_entity *entity = usb_dfu_get_entity(0);
  uint32_t offset;
  uint32_t temp_len;
  int ret = -1;

  if (buf == NULL || entity == NULL)
    {
      return ret;
    }

  if (LOS_AtomicCmpXchg32bits(&g_dfu_status, DFU_READ_STATUS, DFU_IDLE_STATUS))
    {
      return ret;
    }

  if (entity->ram_addr == 0)
    {
      goto read_done;
    }

  offset = entity->offset;
  if (offset > entity->trans_size)
    {
      goto read_done;
    }

  temp_len = MIN(len, (entity->trans_size - offset));

  ret = memcpy_s(buf, len, (const void *)((uint8_t *)entity->ram_addr + offset), temp_len);
  if (ret != EOK)
    {
      usb_err("memcpy failed!\n");
      ret = -1;
      goto read_done;
    }
  entity->offset += temp_len;

  PRINTK("#");
  ret = (int)temp_len;

read_done:
  LOS_AtomicSet(&g_dfu_status, DFU_IDLE_STATUS);
  return ret;
}

static int dfu_ram_write(const uint8_t *buf, uint32_t len)
{
  struct usb_dfu_entity *entity = usb_dfu_get_entity(0);
  uint32_t offset;
  int ret = -1;

  if (buf == NULL || entity == NULL)
    {
      return ret;
    }

  if (LOS_AtomicCmpXchg32bits(&g_dfu_status, DFU_WRITE_STATUS, DFU_IDLE_STATUS))
    {
      return ret;
    }

  if (entity->ram_addr == 0)
    {
      goto write_done;
    }

  offset = entity->offset;
  if (offset > entity->ram_size)
    {
      goto write_done;
    }

  len = MIN(len, (entity->ram_size - offset));

  ret = memcpy_s((void *)((uint8_t *)entity->ram_addr + offset), entity->ram_size - offset, buf, len);
  if (ret != EOK)
    {
      usb_err("memcpy failed!\n");
      ret = -1;
      goto write_done;
    }
  entity->offset     += len;
  entity->trans_size += len;

  PRINTK("#");

write_done:
  LOS_AtomicSet(&g_dfu_status, DFU_IDLE_STATUS);
  return ret;
}
#endif

void usbdev_dfu_dev_init(void)
{
  struct usb_dfu_dev *dev;

  dev = malloc(sizeof(struct usb_dfu_dev));
  if (dev == NULL)
    {
      return;
    }
  (void)memset_s(dev, sizeof(struct usb_dfu_dev), 0, sizeof(struct usb_dfu_dev));

  g_dfu_dev = dev;
  g_dfu_dev->dfu_state = USB_DFU_IDLE;
  to_dfu_mode();

  (void)memset_s(&g_dfu_entity, sizeof(struct usb_dfu_entity), 0, sizeof(struct usb_dfu_entity));

  /* After the event resource application is successful, it will not be deleted.
   * In order to avoid the situation of continuing reading after destruction
   */

  if (g_dfu_init_event == 0)
    {
      (void)LOS_EventInit(&g_dfu_finish_event);
      g_dfu_init_event = 1;
    }
#ifdef CONFIG_DRIVERS_USB_DFU_DOWNLOAD_CALLBACK
  LOS_AtomicSet(&g_dfu_status, DFU_IDLE_STATUS);
#endif
}

void usbdev_dfu_dev_deinit(void)
{
#ifdef CONFIG_DRIVERS_USB_DFU_DOWNLOAD_CALLBACK
  (void)memset_s(&g_dfu_entity, sizeof(struct usb_dfu_entity), 0, sizeof(struct usb_dfu_entity));
#endif
  dfu_mode_change();
  if (g_dfu_dev != NULL)
    {
      free(g_dfu_dev);
      g_dfu_dev = NULL;
    }
  dev_str_desc_free(&g_dfu_string_defs[1]);
}

static struct usb_dfu_dev *usb_dfu_get_dev(void)
{
  return g_dfu_dev;
}

static inline void dfu_set_state(int new_state)
{
  struct usb_dfu_dev *dev = usb_dfu_get_dev();
  dev->dfu_state = new_state;
}

static inline int dfu_get_state(void)
{
  struct usb_dfu_dev *dev = usb_dfu_get_dev();
  return dev->dfu_state;
}

static void dfu_dnload_complete(struct usbdev_ep_s *ep, struct usbdev_req_s *req)
{
  (void)ep;

#ifdef CONFIG_DRIVERS_USB_DFU_DOWNLOAD_CALLBACK
  usb_dfu_download_callback(req->buf, req->len);
#else
  struct usb_dfu_dev *dev = usb_dfu_get_dev();
  if (usb_dfu_write(usb_dfu_get_entity(0), req->buf, (int)req->len, dev->seq_num) < 0)
    {
      dev->dfu_status.bStatus = USB_DFU_ERR_UNKNOWN;
      dev->dfu_state = USB_DFU_ERROR;
    }
#endif
}

void usbdev_dfu_set_config(void)
{
  struct usb_dfu_dev *dev = usb_dfu_get_dev();

  if (dev->dfu_state == USB_DFU_APP_DETACH)
    {
      dev->dfu_state = USB_DFU_IDLE;
    }

  dev->dfu_status.bStatus = USB_DFU_STATUS_OK;
}

void usbdev_dfu_transaction_cleanup(void)
{
  struct usb_dfu_entity *entity = usb_dfu_get_entity(0);

  if (entity == NULL)
    {
      return;
    }

#ifndef CONFIG_DRIVERS_USB_DFU_DOWNLOAD_CALLBACK
  entity->trans_size     = 0;
  entity->seq_num        = 0;
  entity->offset         = 0;
#endif
  entity->trans_complete = 0;
}

static void dfu_mode_change(void)
{
  struct usb_dfu_dev *dev = usb_dfu_get_dev();

  if (dev == NULL)
    {
      return;
    }

  /* When the host confirm that the dfu "bInterfaceProtocol" is the "Runtime mode"
   * it will send a "DFU_DETACH" request to the device. When unload device, the device
   * mode should change from "DFU mode" to "Runtime mode", so that next upgrade action
   * can be performed smoothly.
   *
   * bInterfaceProtocol value: 0x1 --- Runtime protocol.
   * bInterfaceProtocol value: 0x2 --- DFU mode protocol.
   *
   * More information, please refer to DFU_1.1.pdf
   */

  to_runtime_mode();
  dev->dfu_state = USB_DFU_APP_IDLE;
  (void)memset_s(&dev->dfu_status, sizeof(struct dfu_status_response),
                 0, sizeof(struct dfu_status_response));
  dev->dfu_status.bPollTimeout[0] = USB_DFU_POLLTIMEOUT;
}

static int dfu_get_status_for_req(const struct usbdev_req_s *ctrlreq)
{
  struct dfu_status_response *status = (struct dfu_status_response *)ctrlreq->buf;
  struct usb_dfu_dev *dev = usb_dfu_get_dev();
  int dfu_state = dev->dfu_state;

  if (dfu_state == USB_DFU_DNLOAD_SYNC)
    {
      dev->dfu_state = USB_DFU_DNLOAD_IDLE;
    }
  else if (dfu_state == USB_DFU_DNBUSY)
    {
      dev->dfu_state = USB_DFU_DNLOAD_SYNC;
    }
  else if (dfu_state == USB_DFU_MANIFEST_SYNC)
    {
      dev->dfu_state = USB_DFU_MANIFEST;
    }
  else if (dfu_state == USB_DFU_MANIFEST)
    {
      dev->dfu_state = USB_DFU_IDLE;
    }

  *status = dev->dfu_status;
  status->bState = (uint8_t)dev->dfu_state;

  return USB_DFU_RET_STATUS_LEN;
}

static int dfu_get_state_for_req(const struct usbdev_req_s *ctrlreq)
{
  uint8_t *state = ctrlreq->buf;

  *state = (uint8_t)dfu_get_state();

  return USB_DFU_RET_STATE_LEN;
}

static int dfu_dnload(const struct usb_ctrlreq_s *ctrl, struct usbdev_req_s *ctrlreq)
{
  struct usb_dfu_dev *dev = usb_dfu_get_dev();
  int len                 = USB_GETW(ctrl->len);
  uint16_t value          = USB_GETW(ctrl->value);

  dev->dfu_state = USB_DFU_DNLOAD_SYNC;
  dev->seq_num   = value;

  if (len == 0)
    {
      if (dev->dfu_state == USB_DFU_IDLE)
        {
          dev->dfu_state = USB_DFU_ERROR;
          return USB_DFU_RET_ERR;
        }
      else
        {
          dev->dfu_state = USB_DFU_MANIFEST_SYNC;
        }
    }

  ctrlreq->callback = dfu_dnload_complete;

  return len;
}

#ifdef DFU_SUPPORT_UPLOAD
static int dfu_upload(const struct usb_ctrlreq_s *ctrl, const struct usbdev_req_s *ctrlreq)
{
  struct usb_dfu_dev *dev = usb_dfu_get_dev();
  uint16_t value          = USB_GETW(ctrl->value);

  if (dev->dfu_state == USB_DFU_IDLE)
    {
      dev->dfu_state = USB_DFU_UPLOAD_IDLE;
      dev->seq_num   = 0;
      usbdev_dfu_transaction_cleanup();
    }
  else if (dev->dfu_state == USB_DFU_UPLOAD_IDLE)
    {
      dev->seq_num = value;
    }

  return usb_dfu_read(usb_dfu_get_entity(0), ctrlreq->buf, (int)ctrlreq->len, dev->seq_num);
}
#endif

static int dfu_app_idle_request(const struct usb_ctrlreq_s *ctrl, const struct usbdev_req_s *ctrlreq)
{
  int ret = USB_DFU_RET_OK;

  switch (ctrl->req)
    {
    case USB_DFU_DETACH:
      {
        to_dfu_mode();
        dfu_set_state(USB_DFU_APP_DETACH);
      }
      break;

    case USB_DFU_GET_STATUS:
      ret = dfu_get_status_for_req(ctrlreq);
      break;

    case USB_DFU_GET_STATE:
      ret = dfu_get_state_for_req(ctrlreq);
      break;

    default:
      {
        ret = USB_DFU_RET_ERR;
        dfu_set_state(USB_DFU_APP_IDLE);
      }
      break;
    }

  return ret;
}

static int dfu_app_detach_request(const struct usb_ctrlreq_s *ctrl, const struct usbdev_req_s *ctrlreq)
{
  int ret;

  switch (ctrl->req)
    {
    case USB_DFU_GET_STATUS:
      ret = dfu_get_status_for_req(ctrlreq);
      break;

    case USB_DFU_GET_STATE:
      ret = dfu_get_state_for_req(ctrlreq);
      break;

    default:
      {
        ret = USB_DFU_RET_ERR;
        dfu_set_state(USB_DFU_APP_IDLE);
      }
      break;
    }

  return ret;
}

static int dfu_idle_request(const struct usb_ctrlreq_s *ctrl, struct usbdev_req_s *ctrlreq)
{
  int ret = USB_DFU_RET_OK;

  switch (ctrl->req)
    {
    case USB_DFU_GET_STATUS:
      ret = dfu_get_status_for_req(ctrlreq);
      break;

    case USB_DFU_GET_STATE:
      ret = dfu_get_state_for_req(ctrlreq);
      break;

    case USB_DFU_DNLOAD:
      ret = dfu_dnload(ctrl, ctrlreq);
      break;

#ifdef DFU_SUPPORT_UPLOAD
    case USB_DFU_UPLOAD:
      ret = dfu_upload(ctrl, ctrlreq);
      break;
#endif

    case USB_DFU_ABORT:
      dfu_set_state(USB_DFU_IDLE);
      break;

    case USB_DFU_FLASH_STATE:
      {
        get_flash_state(ctrlreq);
        ret = 1;
      }
      break;

    default:
      {
        ret = USB_DFU_RET_ERR;
        dfu_set_state(USB_DFU_ERROR);
      }
      break;
    }

  return ret;
}

static int dfu_dnload_sync_request(const struct usb_ctrlreq_s *ctrl, const struct usbdev_req_s *ctrlreq)
{
  int ret;

  switch (ctrl->req)
    {
    case USB_DFU_GET_STATUS:
      ret = dfu_get_status_for_req(ctrlreq);
      break;

    case USB_DFU_GET_STATE:
      ret = dfu_get_state_for_req(ctrlreq);
      break;

    default:
      {
        ret = USB_DFU_RET_ERR;
        dfu_set_state(USB_DFU_ERROR);
      }
      break;
    }

  return ret;
}

static int dfu_dnbusy_request(const struct usb_ctrlreq_s *ctrl, const struct usbdev_req_s *ctrlreq)
{
  int ret;

  switch (ctrl->req)
    {
    case USB_DFU_GET_STATUS:
      ret = dfu_get_status_for_req(ctrlreq);
      break;

    default:
      {
        ret = USB_DFU_RET_ERR;
        dfu_set_state(USB_DFU_ERROR);
      }
      break;
    }

  return ret;
}

static int dfu_dnload_idle_request(const struct usb_ctrlreq_s *ctrl, struct usbdev_req_s *ctrlreq)
{
  int ret = USB_DFU_RET_OK;

  switch (ctrl->req)
    {
    case USB_DFU_GET_STATUS:
      ret = dfu_get_status_for_req(ctrlreq);
      break;

    case USB_DFU_GET_STATE:
      ret = dfu_get_state_for_req(ctrlreq);
      break;

    case USB_DFU_DNLOAD:
      ret = dfu_dnload(ctrl, ctrlreq);
      break;

    case USB_DFU_ABORT:
      dfu_set_state(USB_DFU_IDLE);
      break;

    default:
      {
        ret = USB_DFU_RET_ERR;
        dfu_set_state(USB_DFU_ERROR);
      }
      break;
    }

  return ret;
}

static int dfu_manifest_sync_request(const struct usb_ctrlreq_s *ctrl, const struct usbdev_req_s *ctrlreq)
{
  struct usb_dfu_dev *dev = usb_dfu_get_dev();
  int ret;

  switch (ctrl->req)
    {
    case USB_DFU_GET_STATUS:
      {
        dev->seq_num = 0;
        ret = dfu_get_status_for_req(ctrlreq);
#ifndef CONFIG_DRIVERS_USB_DFU_DOWNLOAD_CALLBACK
        g_dfu_entity.seq_num = 0;
        g_dfu_entity.offset  = 0;
#endif
      }
      break;

    case USB_DFU_GET_STATE:
      ret = dfu_get_state_for_req(ctrlreq);
      break;

    default:
      {
        ret = USB_DFU_RET_ERR;
        dfu_set_state(USB_DFU_ERROR);
      }
      break;
    }

  return ret;
}

static int dfu_manifest_request(const struct usb_ctrlreq_s *ctrl, const struct usbdev_req_s *ctrlreq)
{
  struct usb_dfu_dev *dev = usb_dfu_get_dev();
  struct usb_dfu_entity *entity = usb_dfu_get_entity(0);
  int ret;

  switch (ctrl->req)
    {
    case USB_DFU_GET_STATUS:
      {
        ret = dfu_get_status_for_req(ctrlreq);
        dev->seq_num = 0;
        if (entity != NULL)
          {
            entity->trans_complete = 1;
            (void)LOS_EventWrite(&g_dfu_finish_event, USB_DFU_FINISH_EVENT);
          }
        PRINTK("\nDOWNLOAD ... OK\n");
      }
      break;

    case USB_DFU_GET_STATE:
      ret = dfu_get_state_for_req(ctrlreq);
      break;

    default:
      {
        ret = USB_DFU_RET_ERR;
        dfu_set_state(USB_DFU_ERROR);
      }
      break;
    }

  return ret;
}

static int dfu_manifest_wait_reset(const struct usb_ctrlreq_s *ctrl, const struct usbdev_req_s *ctrlreq)
{
  (void)ctrl;
  (void)ctrlreq;
  PRINTK("Do nothing!\n");
  return 0;
}

#ifdef DFU_SUPPORT_UPLOAD
static int dfu_upload_idle_request(const struct usb_ctrlreq_s *ctrl, const struct usbdev_req_s *ctrlreq)
{
  int ret = USB_DFU_RET_OK;
  int len = USB_GETW(ctrl->len);

  switch (ctrl->req)
    {
    case USB_DFU_GET_STATUS:
      ret = dfu_get_status_for_req(ctrlreq);
      break;

    case USB_DFU_GET_STATE:
      ret = dfu_get_state_for_req(ctrlreq);
      break;

    case USB_DFU_UPLOAD:
      {
        ret = dfu_upload(ctrl, ctrlreq);
        if (ret >= 0 && ret < len)
          {
            dfu_set_state(USB_DFU_IDLE);
          }
      }
      break;

    case USB_DFU_ABORT:
      dfu_set_state(USB_DFU_IDLE);
      break;

    default:
      {
        ret = USB_DFU_RET_ERR;
        dfu_set_state(USB_DFU_ERROR);
      }
      break;
    }

  return ret;
}
#endif

static int dfu_error_request(const struct usb_ctrlreq_s *ctrl, const struct usbdev_req_s *ctrlreq)
{
  struct usb_dfu_dev *dev = usb_dfu_get_dev();
  int ret                 = USB_DFU_RET_OK;

  switch (ctrl->req)
    {
    case USB_DFU_GET_STATUS:
      ret = dfu_get_status_for_req(ctrlreq);
      break;

    case USB_DFU_GET_STATE:
      ret = dfu_get_state_for_req(ctrlreq);
      break;

    case USB_DFU_CLR_STATUS:
      {
        dev->dfu_status.bStatus = USB_DFU_STATUS_OK;
        dfu_set_state(USB_DFU_IDLE);
      }
      break;

    default:
      {
        ret = USB_DFU_RET_ERR;
        dfu_set_state(USB_DFU_ERROR);
      }
      break;
    }

  return ret;
}

int usbdev_dfu_class_requests(const struct usbdev_s *dev,
                              const struct usb_ctrlreq_s *ctrl,
                              struct usbdev_req_s *ctrlreq)
{
  int ret = -EOPNOTSUPP;

  (void)dev;

  switch (dfu_get_state())
    {
    case USB_DFU_APP_IDLE:
      ret = dfu_app_idle_request(ctrl, ctrlreq);
      break;

    case USB_DFU_APP_DETACH:
      ret = dfu_app_detach_request(ctrl, ctrlreq);
      break;

    case USB_DFU_IDLE:
      ret = dfu_idle_request(ctrl, ctrlreq);
      break;

    case USB_DFU_DNLOAD_SYNC:
      ret = dfu_dnload_sync_request(ctrl, ctrlreq);
      break;

    case USB_DFU_DNBUSY:
      ret = dfu_dnbusy_request(ctrl, ctrlreq);
      break;

    case USB_DFU_DNLOAD_IDLE:
      ret = dfu_dnload_idle_request(ctrl, ctrlreq);
      break;

    case USB_DFU_MANIFEST_SYNC:
      ret = dfu_manifest_sync_request(ctrl, ctrlreq);
      break;

    case USB_DFU_MANIFEST:
      ret = dfu_manifest_request(ctrl, ctrlreq);
      break;

    case USB_DFU_MANIFEST_WAIT_RESET:
      ret = dfu_manifest_wait_reset(ctrl, ctrlreq);
      break;

#ifdef DFU_SUPPORT_UPLOAD
    case USB_DFU_UPLOAD_IDLE:
      ret = dfu_upload_idle_request(ctrl, ctrlreq);
      break;
#endif

    case USB_DFU_ERROR:
      ret = dfu_error_request(ctrl, ctrlreq);
      (void)LOS_EventWrite(&g_dfu_finish_event, USB_DFU_ERROR_EVENT);
      break;

    default:
      usb_err("The state = %d of dfu is not support!\n", dfu_get_state());
      break;
    }

  return ret;
}

static int usbclass_dfu_setup(struct usbdevclass_driver_s *driver, struct usbdev_s *dev,
                              const struct usb_ctrlreq_s *ctrl, uint8_t *dataout, size_t outlen)
{
  struct dfu_dev_s *priv = &((struct dfu_driver_s *)driver)->dev;
  struct usbdev_req_s *ctrlreq = priv->ctrlreq;
  uint16_t len = 0;
  int ret = -EOPNOTSUPP;

  (void)dataout;
  (void)outlen;
  len = USB_GETW(ctrl->len);
  if ((ctrl->type & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_STANDARD)
    {
      switch (ctrl->req)
        {
        case USB_REQ_SETCONFIGURATION:
          {
            usbdev_dfu_set_config();
            return 0;
          }

        case USB_REQ_SETINTERFACE:
          {
            ret = 0;
          }
          break;

        case USB_REQ_GETINTERFACE:
          {
            /* Gets alternate settings of interface descriptors */

            ctrlreq->buf[0] = g_dfu_config_desc.if_desc.alt;
            ret = 1;
          }
          break;

        default:
          break;
        }
    }
  else if ((ctrl->type & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_CLASS)
    {
      ret = usbdev_dfu_class_requests(dev, ctrl, ctrlreq);
    }
#ifdef CONFIG_DRIVERS_USB_DFU_DRIVE_FREE
  else if (ctrl->req == USB_REQ_GETMSFTOSDESCRIPTOR)
    {
      ret = dfu_get_msft_proerty_desc(ctrlreq->buf);
    }
#endif
  if (ret >= 0)
    {
      ctrlreq->len = MIN(ret, len);
      ret          = EP_SUBMIT(dev->ep0, ctrlreq);
      if (ret < 0)
        {
          ctrlreq->result = OK;
        }
    }

  return ret;
}

static int usbclass_dfu_bind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  struct dfu_dev_s *priv = &((struct dfu_driver_s *)driver)->dev;
  struct composite_dev_s *cdev = dev->ep0->priv;

  priv->ctrlreq = composite_get_ctrlreq_by_dev(cdev);
  if (priv->ctrlreq == NULL)
    {
      return -1;
    }
  usbdev_dfu_dev_init();

  return 0;
}

static int usbclass_dfu_unbind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  (void)driver;
  (void)dev;

  if (usb_dfu_running())
    {
      usb_err("unbind fail, dfu busy\n");
      return -1;
    }

  usbdev_dfu_dev_deinit();
  return 0;
}

static void usbclass_dfu_disconnect(struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  (void)driver;
  (void)dev;

  usbdev_dfu_transaction_cleanup();
}

/* Waiting for the transfer complete by internal suspension,
 * This interface is recommended.
 */

int usb_dfu_wait_transmit_finish(uint32_t timeout)
{
  uint32_t ret_event;

  if (LOS_AtomicRead(&g_dfu_status) == DFU_FREE_STATUS)
    {
      return -1;
    }

  ret_event = LOS_EventRead(&g_dfu_finish_event,
                            USB_DFU_FINISH_EVENT | USB_DFU_ERROR_EVENT | USB_DFU_FREE_EVENT,
                            LOS_WAITMODE_OR | LOS_WAITMODE_CLR, timeout);
  if ((ret_event & USB_DFU_ERROR_EVENT) || (ret_event & USB_DFU_FREE_EVENT))
    {
      return -1;
    }
  return g_dfu_entity.trans_complete;
}

#ifndef CONFIG_DRIVERS_USB_DFU_DOWNLOAD_CALLBACK
static int dfu_ram_init_env(struct usb_dfu_entity *entity, char *envstr)
{
  char *st;
  int ret;

  st = strsep(&envstr, " ");
  if (st == NULL)
    {
      return -1;
    }
  ret = strncpy_s(entity->dfu_name, DFU_NAME_MAX_LEN, st, strlen(st) + 1);
  if (ret != EOK)
    {
      return -1;
    }

  st = strsep(&envstr, " ");
  if (st == NULL)
    {
      return -1;
    }
  if (strcmp(st, "ram"))
    {
      return -1;
    }

  st = strsep(&envstr, " ");
  if (st == NULL)
    {
      return -1;
    }
  entity->ram_addr = (void *)strtoul(st, &st, 16);

  st = strsep(&envstr, " ");
  if (st == NULL)
    {
      return -1;
    }
  entity->ram_size = strtoul(st, &st, 16);
  return 0;
}

int usb_dfu_init_env_entities(const char *type, char *envstr, const char *devstr)
{
  struct usb_dfu_entity *entity;
  int ret = -1;

  (void)devstr;

  if (type == NULL || envstr == NULL)
    {
      return ret;
    }

  if (LOS_AtomicCmpXchg32bits(&g_dfu_status, DFU_INIT_STATUS, DFU_FREE_STATUS))
    {
      return ret;
    }

  if (strcmp(type, "ram"))
    {
      usb_err("Device type not (yet) supported!\n");
      goto init_err;
    }

  entity = &g_dfu_entity;
  ret = dfu_ram_init_env(entity, envstr);
  if (ret < 0)
    {
      goto init_err;
    }

  usbdev_dfu_transaction_cleanup();
  LOS_AtomicSet(&g_dfu_status, DFU_IDLE_STATUS);
  return ret;

init_err:
  LOS_AtomicSet(&g_dfu_status, DFU_FREE_STATUS);
  return ret;
}

void usb_dfu_free_entities(void)
{
  if (LOS_AtomicCmpXchg32bits(&g_dfu_status, DFU_FREE_STATUS, DFU_IDLE_STATUS))
    {
      usb_err("DFU is busy.\n");
      return;
    }

  (void)memset_s(&g_dfu_entity, sizeof(struct usb_dfu_entity), 0, sizeof(struct usb_dfu_entity));
}

int usb_dfu_read(struct usb_dfu_entity *dfu, void *buf, int size, uint32_t blk_seq_num)
{
  int ret;

  if (dfu == NULL || buf == NULL || size <= 0)
    {
      return -1;
    }

  if (dfu->seq_num != blk_seq_num || dfu->trans_complete == 0)
    {
      return -1;
    }
  dfu->seq_num++;

  ret = dfu_ram_read(buf, size);
  if (ret < 0)
    {
      return -1;
    }

  if (dfu->offset >= dfu->trans_size)
    {
      PRINTK("\nUPLOAD ... done\n");
      usbdev_dfu_transaction_cleanup();
    }

  return ret;
}

static int usb_dfu_write(struct usb_dfu_entity *dfu, const void *buf, int size, uint32_t blk_seq_num)
{
  int ret;

  if (dfu == NULL || buf == NULL || size < 0)
    {
      return -1;
    }

  if (dfu->seq_num != blk_seq_num)
    {
      usbdev_dfu_transaction_cleanup();
      return -1;
    }
  dfu->seq_num++;

  ret = dfu_ram_write(buf, size);
  if (ret < 0)
    {
      usbdev_dfu_transaction_cleanup();
      return -1;
    }

  return 0;
}
#endif

static void usbdev_dfu_mkdevdesc(uint8_t *buf)
{
  errno_t ret;

  if (USB_GETW(g_dfu_device_desc.vendor) == 0)
    {
      usb_err("VID is not set!\n");
      return;
    }

  ret = memcpy_s(buf, USB_COMP_EP0_BUFSIZ, &g_dfu_device_desc, sizeof(g_dfu_device_desc));
  if (ret != EOK)
    {
      usb_err("memcpy_s fail!, ret:%d\n", ret);
      return;
    }
}

static int usbdev_dfu_mkstrdesc(uint8_t id, struct usb_strdesc_s *buf)
{
  errno_t ret;
  const char *str;
  int i;

  for (i = 0; g_dfu_string_defs[i].s != NULL; i++)
    {
      str = g_dfu_string_defs[i].s;
      if (g_dfu_string_defs[i].id == id)
        {
          ret = memcpy_s(buf, USB_COMP_EP0_BUFSIZ, str, str[0]);
          if (ret != EOK)
            {
              usb_err("memcpy_s failed, ret = %d\n", ret);
              return -1;
            }
          return str[0];
        }
    }
  usb_err("Can not find the id = %u of string\n", id);

  return -1;
}

static int16_t usbdev_dfu_mkcfgdesc(uint8_t *buf,
                                    struct usbdev_devinfo_s *devinfo)
{
  int ret;
  size_t len = sizeof(struct dfu_config_desc);
  (void)devinfo;

  ret = memcpy_s(buf, USB_COMP_EP0_BUFSIZ, &g_dfu_config_desc, len);
  if (ret != EOK)
    {
      usb_err("memcpy_s fail, ret:%d\n", ret);
      return -1;
    }

  return (int16_t)len;
}

static int usbdev_dfu_classobject(int minor, struct usbdev_devinfo_s *devinfo,
                                  struct usbdevclass_driver_s **classdev)
{
  struct dfu_driver_s *dfu_drvr;

  (void)minor;
  (void)devinfo;

  dfu_drvr = (struct dfu_driver_s *)malloc(sizeof(struct dfu_driver_s));
  if (dfu_drvr == NULL)
    {
      return -1;
    }

  dfu_drvr->drvr.speed = USB_SPEED_FULL;
  dfu_drvr->drvr.ops = &g_dfu_driverops;
  *classdev = &dfu_drvr->drvr;

  return 0;
}

/****************************************************************************
 * Name: usbdev_dfu_uninitialize
 *
 * Description:
 *   Free allocated memory
 *
 ****************************************************************************/

static void usbdev_dfu_uninitialize(struct usbdevclass_driver_s *classdev)
{
  struct dfu_driver_s *dfu_s;

  if (classdev == NULL)
    {
      usb_err("Invalid pointer!\n");
      return;
    }

  dfu_s = container_of(classdev, struct dfu_driver_s, drvr);
  if (dfu_s != NULL)
    {
      free(dfu_s);
    }
}

static void usbdev_dfu_initialize_dev(struct composite_devdesc_s *dev, int ifnobase, int minor)
{
  (void)memset_s(dev, sizeof(struct composite_devdesc_s), 0, sizeof(struct composite_devdesc_s));

  dev->classobject  = usbdev_dfu_classobject;
  dev->uninitialize = usbdev_dfu_uninitialize;

  dev->mkdevdesc  = usbdev_dfu_mkdevdesc;
  dev->mkconfdesc = usbdev_dfu_mkcfgdesc;
  dev->mkstrdesc  = usbdev_dfu_mkstrdesc;

  dev->devinfo.ninterfaces = 1;
  dev->devinfo.nstrings    = 1;
  dev->devinfo.nendpoints  = 0;
  dev->devinfo.ifnobase    = ifnobase; /* Offset to Interface-IDs */
  dev->devinfo.strbase     = 0; /* Offset to String Numbers */

  dev->minor    = minor; /* The minor interface number */
  dev->nconfigs = 1;
  dev->configid = 0;
#ifdef CONFIG_DRIVERS_USB_DFU_DRIVE_FREE
  if (strncpy_s((char *)dev->msft_compatible_id, COMPATIBLE_ID_LEN, "WINUSB", STRING_WINUSB_LEN) != EOK)
    {
      usb_err("strncpy_s fail!\n");
    }
#endif
}

int usbdev_dfu_initialize(void *handle)
{
  struct composite_devdesc_s dev;
  int ret;

  /* Ignore compilation warnings */

  usbdev_dfu_initialize_dev(&dev, 0, DEV_DFU);

  ret = composite_initialize_softc(1, &dev, handle);
  if (ret < 0)
    {
      return -1;
    }

  PRINTK("** DFU device initialized successfully! **\n");
  return 0;
}
