/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Usb custom protocol \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-08-18, Create file. \n
 */

#include "gadget/composite.h"
#include "implementation/_thread_ref.h"

static int usbclass_test_bind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev);
static int usbclass_test_unbind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev);
static int usbclass_test_setup(struct usbdevclass_driver_s *driver, struct usbdev_s *dev,
                              const struct usb_ctrlreq_s *ctrl, uint8_t *dataout, size_t outlen);
static void usbclass_test_disconnect(struct usbdevclass_driver_s *driver, struct usbdev_s *dev);

#define TEST_WRITE_BUF_SIZE 64
#define TEST_READ_BUF_SIZE 64

enum test_read_event
{
  USB_TEST_READ_EVENT = 1u << 0,
  USB_TEST_EXIT_EVENT = 1u << 1,
};

struct test_dev_s
{
  struct usbdev_ep_s *in_ep;
  bool in_ep_enabled;
  struct usbdev_req_s inputreq;

  uint8_t write_buf[TEST_WRITE_BUF_SIZE];
  uint8_t write_busy;

  struct usbdev_ep_s *out_ep;
  bool out_ep_enabled;
  struct usbdev_req_s outputreq;

  void *recv_buf;
  uint8_t read_buf[TEST_READ_BUF_SIZE];
  size_t read_len;
  EVENT_CB_S read_event;
  bool event_flag;

  uint8_t connected; /* test device connection status. 1: connected, 0: disconnected */
};

struct test_driver_s
{
  struct usbdevclass_driver_s drvr;
  struct test_dev_s *dev;
};

struct test_softc
{
  struct test_dev_s dev;
  struct test_driver_s drvr;
};

static struct test_dev_s *g_test_dev = NULL;

/* USB driver operations */

static const struct usbdevclass_driverops_s g_test_driverops =
{
  usbclass_test_bind,
  usbclass_test_unbind,
  usbclass_test_setup,
  usbclass_test_disconnect,
  NULL,
  NULL
};

#define TEST_STR_LANG 4
static const char g_ftest_str_lang[TEST_STR_LANG] =
{
  TEST_STR_LANG,
  USB_DESC_TYPE_STRING,
  0x09, 0x04
};

static const char g_ftest_str_manufacturer[] =
{
  26,
  USB_DESC_TYPE_STRING,
  'S', 0, 'M', 0, '-', 0, '2', 0, '0', 0, '8', 0, '-', 0, 'A', 0, '-', 0, 'R', 0, 'G', 0, 'B', 0
};

static const char g_ftest_str_product[] =
{
  22,
  USB_DESC_TYPE_STRING,
  'L', 0, 'i', 0, 't', 0, 'e', 0, 'O', 0, 'S', 0, ' ', 0,
  'S', 0, 'L', 0, 'E', 0
};

static const char g_ftest_str_serial[] =
{
  18,
  USB_DESC_TYPE_STRING,
  '2', 0, '0', 0, '2', 0, '3', 0, '0', 0, '8', 0, '1', 0,
  '8', 0
};

#define TEST_STR_IDX_INTERFACE 28
static const char g_ftest_str_interface[TEST_STR_IDX_INTERFACE] =
{
  TEST_STR_IDX_INTERFACE,
  USB_DESC_TYPE_STRING,
  'H', 0, 'I', 0, 'D', 0, ' ', 0, 'I', 0, 'n', 0, 't', 0,
  'e', 0, 'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0
};

static struct usbd_string g_ftest_device_strings[6] =
{
  { 0, g_ftest_str_lang },
  { 1, g_ftest_str_manufacturer },
  { 2, g_ftest_str_product },
  { 3, g_ftest_str_serial },
  { 4, g_ftest_str_interface },
  USBD_DEVICE_STRINGS_END
};

static struct usb_devdesc_s g_ftest_device_desc =
{
  .len          = sizeof(struct usb_devdesc_s),
  .type         = USB_DESC_TYPE_DEVICE, /* Constant for device descriptor */
  HSETW(.usb, USB_VERSION_BCD),         /* USB version required: 2.0 */
  .classid      = 0x00,                 /* Miscellaneous Device Class */
  .subclass     = 0x00,                 /* Common Class */
  .protocol     = 0x00,                 /* Interface Association Descriptor */
  .mxpacketsize = USB_EP_MPS,           /* Control Endpoint packet size */
  HSETW(.vendor, 0xffff),
  HSETW(.product, 0xffff),
  HSETW(.device, 0x0801),               /* Device release code */
  .imfgr        = 1,                    /* Manufacturer name, string index */
  .iproduct     = 2,                    /* Product name, string index */
  .serno        = 3,                    /* Used */
  .nconfigs     = 1                     /* One Configuration */
};

static struct usb_cfgdesc_s g_ftest_config_desc =
{
  .len         = sizeof(struct usb_cfgdesc_s),
  .type        = USB_DESC_TYPE_CONFIG,
  HSETW(.totallen, 0), /* Size of all descriptors, set later */
  .ninterfaces = 0x1,  /* Number of Interfaces */
  .cfgvalue    = 0x1,  /* ID of this configuration */
  .icfg        = 0x0,  /* Index of string descriptor */
  .attr        = 0xc0, /* Bus-powered */
  .mxpower     = 0x32  /* Maximum power consumption from the bus */
};

static struct usb_ifdesc_s g_ftest_intf_desc =
{
  .len      = sizeof(struct usb_ifdesc_s),
  .type     = USB_DESC_TYPE_INTERFACE,
  .ifno     = 0,    /* Index number of this interface */
  .alt      = 0,    /* Index of this settings */
  .neps     = 2,    /* Number of endpoint */
  .classid  = 0xff,
  .subclass = 0xff,
  .protocol = 0xff,
  .iif      = 0     /* Index of string descriptor */
};

static struct usb_epdesc_s g_ftest_in_ep_desc =
{
  .len      = sizeof(struct usb_epdesc_s),
  .type     = USB_DESC_TYPE_ENDPOINT,
  .addr     = USB_DIR_IN | 0x01,
  .attr     = 0x02,
  HSETW(.mxpacketsize, 0x0200),
  .interval = 0
};

static struct usb_epdesc_s g_ftest_out_ep_desc =
{
  .len      = sizeof(struct usb_epdesc_s),
  .type     = USB_DESC_TYPE_ENDPOINT,
  .addr     = USB_DIR_OUT | 0x01,
  .attr     = 0x02,
  HSETW(.mxpacketsize, 0x0200),
  .interval = 0
};


#define TEST_DESC_ARRAY_MAX_NUM 5
static const uint8_t *g_ftest_desc_array[TEST_DESC_ARRAY_MAX_NUM] =
{
  (const uint8_t *)&g_ftest_config_desc,
  (const uint8_t *)&g_ftest_intf_desc,
  (const uint8_t *)&g_ftest_in_ep_desc,
  (const uint8_t *)&g_ftest_out_ep_desc,
  NULL,
};

ssize_t ftest_recv_data(char *buf, size_t buflen)
{
  struct test_dev_s *test_data = g_test_dev;
  uint32_t ret_event;
  errno_t ret;
  ssize_t len;

  if (buf == NULL || buflen == 0 || buflen > TEST_READ_BUF_SIZE)
    {
      usb_err("invalid buf or invalid buflen. buflen: %u, max size: %u\n", buflen, TEST_READ_BUF_SIZE);
      return -1;
    }

  if (test_data == NULL || test_data->connected == 0)
    {
      usb_warn("invalid test or test is disconnected!\n");
      return -1;
    }

  ret_event = LOS_EventRead(&test_data->read_event, USB_TEST_READ_EVENT | USB_TEST_EXIT_EVENT,
                            LOS_WAITMODE_OR, LOS_WAIT_FOREVER);
  if (ret_event & USB_TEST_EXIT_EVENT)
    {
      return -1;
    }
  (void)LOS_EventClear(&test_data->read_event, ~USB_TEST_READ_EVENT);

  ret = memcpy_s(buf, buflen, test_data->read_buf, test_data->read_len);
  if (ret != EOK)
    {
      usb_err("memcpy failed! ret = %d\n", ret);
      return -1;
    }
  len = test_data->read_len;
  test_data->read_len = 0;

  return len;
}

ssize_t ftest_send_data(const char *buf, size_t buflen)
{
  struct test_dev_s *test_data = g_test_dev;
  struct usbdev_req_s *req;
  errno_t err;

  if (buf == NULL || buflen == 0 || buflen > TEST_WRITE_BUF_SIZE)
    {
      usb_err("invalid buf or invalid buflen!\n");
      return -1;
    }

  if (test_data == NULL || test_data->connected == 0)
    {
      usb_warn("invalid test or test is disconnected!\n");
      return -1;
    }

  if (test_data->write_busy)
    {
      return -1;
    }

  err = memcpy_s(test_data->write_buf, TEST_WRITE_BUF_SIZE, buf, buflen);
  if (err != EOK)
    {
      return -1;
    }
  test_data->write_busy = 1;

  req = &test_data->inputreq;
  req->buf = test_data->write_buf;
  req->len = buflen;
  (void)EP_SUBMIT(test_data->in_ep, req);

  return buflen;
}

static void ftest_output_request_complete(struct usbdev_ep_s *ep, struct usbdev_req_s *req)
{
  struct test_dev_s *test_data;
  errno_t ret;
  uint32_t ret_event;

  if (ep == NULL || req == NULL || req->priv == NULL)
    {
      usb_err("Illegal request or ep!\n");
      return;
    }

  if (req->result != 0)
    {
      return;
    }

  test_data = (struct test_dev_s *)req->priv;

  ret = memcpy_s(test_data->read_buf, TEST_READ_BUF_SIZE, req->buf, req->xfrd);
  if (ret != EOK)
    {
      usb_err("memcpy fail!\n");
      return;
    }
  test_data->read_len = req->xfrd;

  ret_event = LOS_EventWrite(&test_data->read_event, USB_TEST_READ_EVENT);
  if (ret_event != LOS_OK)
    {
      usb_err("write event failed!\r\n");
    }

  (void)EP_SUBMIT(test_data->out_ep, req);
}

static void ftest_input_req_complete(struct usbdev_ep_s *ep, struct usbdev_req_s *req)
{
  struct test_dev_s *test_data = (struct test_dev_s *)ep->priv;
  (void)req;

  test_data->write_busy = 0;
}

static int test_ep_data_init(struct test_dev_s *test_data, struct usbdev_s *dev, struct usbdev_devinfo_s *devinfo)
{
  struct usbdev_ep_s *ep;

  /* Initialize the interrupt input endpoint */

  ep = DEV_ALLOCEP(dev, g_ftest_in_ep_desc.addr, (struct usb_epdesc_s *)&g_ftest_in_ep_desc);
  if (ep == NULL)
    {
      return -1;
    }

  (void)memset_s(&test_data->inputreq, sizeof(struct usbdev_req_s), 0, sizeof(struct usbdev_req_s));
  test_data->inputreq.callback = ftest_input_req_complete;
  test_data->inputreq.priv     = (void *)test_data;
  test_data->inputreq.buf      = NULL;
  ep->priv                     = (void *)test_data;
  ep->handle_req               = &test_data->inputreq;
  test_data->in_ep             = ep;
  devinfo->epno[0]             = ep->eplog;

  /* When the abnormal branch returns, the applied resources are released in usbclass_test_unbind */

  test_data->write_busy = 1;

  ep = DEV_ALLOCEP(dev, g_ftest_out_ep_desc.addr, (struct usb_epdesc_s *)&g_ftest_out_ep_desc);
  if (ep == NULL)
    {
      return -1;
    }

  (void)memset_s(&(test_data->outputreq), sizeof(struct usbdev_req_s), 0, sizeof(struct usbdev_req_s));
  test_data->outputreq.callback = ftest_output_request_complete;
  test_data->outputreq.priv     = (void *)test_data;
  test_data->outputreq.buf      = NULL;
  ep->priv                      = (void *)test_data;
  ep->handle_req                = &test_data->outputreq;
  test_data->out_ep             = ep;
  devinfo->epno[1]              = ep->eplog;

  test_data->recv_buf = memalign(USB_CACHE_ALIGN_SIZE, TEST_READ_BUF_SIZE);
  if (test_data->recv_buf == NULL)
    {
      return -1;
    }

  (void)LOS_EventInit(&test_data->read_event);
  test_data->event_flag = true;

  return 0;
}

static void test_ep_data_deinit(struct test_dev_s *test_data, struct usbdev_s *dev)
{
  /* Destroy read event */

  if (test_data->event_flag == true)
    {
      test_data->event_flag = false;
      (void)LOS_EventDestroy(&test_data->read_event);
    }

  if (test_data->recv_buf != NULL)
    {
      free(test_data->recv_buf);
      test_data->recv_buf = NULL;
    }

  if (test_data->out_ep != NULL)
    {
      DEV_FREEEP(dev, test_data->out_ep);
      test_data->out_ep = NULL;
    }

  if (test_data->in_ep != NULL)
    {
      DEV_FREEEP(dev, test_data->in_ep);
      test_data->in_ep = NULL;
    }
}

static int usbclass_test_bind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  struct test_driver_s *drvr;
  struct composite_dev_s *cdev;
  struct test_dev_s *test_data;
  struct composite_devobj_s *devobj;
  struct usbdev_devinfo_s *devinfo;
  int ret = -1;

  if (driver == NULL || dev == NULL)
    {
      return ret;
    }

  cdev = dev->ep0->priv;
  drvr = (struct test_driver_s *)driver;
  test_data  = drvr->dev;
  if (test_data == NULL)
    {
      return ret;
    }

  devobj = usbclass_devobj_get(cdev, DEV_CUSTOM);
  if (devobj == NULL)
    {
      return ret;
    }

  devinfo = &devobj->compdesc.devinfo;

  test_data->connected = 0;

  ret = test_ep_data_init(test_data, dev, devinfo);
  if (ret != 0)
    {
      goto errout;
    }

  g_test_dev = test_data;
  return ret;
errout:
  (void)usbclass_test_unbind(driver, dev);
  return ret;
}

static int usbclass_test_unbind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  struct composite_dev_s *cdev;
  struct composite_devobj_s *devobj;
  struct usbdev_devinfo_s *devinfo;
  struct test_driver_s *drvr;
  struct test_dev_s *test_data;

  if (driver == NULL || dev == NULL)
    {
      return -1;
    }

  drvr = (struct test_driver_s *)driver;
  test_data  = drvr->dev;

  /* If there is a read data task waiting, must write an event to notify the task to exit,
   * otherwise it will never be unbind.
   */

  if (test_data->event_flag == true && !LOS_ListEmpty(&test_data->read_event.stEventList))
    {
      (void)LOS_EventWrite(&test_data->read_event, USB_TEST_EXIT_EVENT);
    }

  g_test_dev = NULL;

  usbclass_test_disconnect(driver, dev);

  test_ep_data_deinit(test_data, dev);

  cdev = dev->ep0->priv;
  devobj = usbclass_devobj_get(cdev, DEV_CUSTOM);
  if (devobj == NULL)
    {
      return -1;
    }
  devinfo = &devobj->compdesc.devinfo;
  (void)memset_s(devinfo, sizeof(struct usbdev_devinfo_s), 0, sizeof(struct usbdev_devinfo_s));

  return 0;
}

static void usbclass_test_set_endpoint(struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  struct test_driver_s *drvr;
  struct test_dev_s *test_data;
  struct usbdev_req_s *req;
  int ret;

  drvr = (struct test_driver_s *)driver;
  test_data  = drvr->dev;

  if (test_data->in_ep_enabled == true)
    {
      (void)EP_DISABLE(test_data->in_ep);
      test_data->in_ep_enabled = false;
    }

  ret = EP_CONFIGURE(test_data->in_ep, (const struct usb_epdesc_s *)&g_ftest_in_ep_desc, 0);
  if (ret < 0)
    {
      usb_err("Config interrupt in_ep failed!\n");
      goto errout;
    }
  test_data->in_ep_enabled = true;

  if (test_data->out_ep_enabled == true)
    {
      (void)EP_DISABLE(test_data->out_ep);
      test_data->out_ep_enabled = false;
    }

  ret = EP_CONFIGURE(test_data->out_ep, (const struct usb_epdesc_s *)&g_ftest_out_ep_desc, 0);
  if (ret < 0)
    {
      usb_err("Config interrupt out_ep failed!\n");
      goto errout;
    }
  test_data->out_ep_enabled = true;

  (void)LOS_EventClear(&test_data->read_event, ~(USB_TEST_EXIT_EVENT | USB_TEST_READ_EVENT));
  req = &test_data->outputreq;
  req->buf = (uint8_t *)test_data->recv_buf;
  req->len = TEST_READ_BUF_SIZE;
  ret      = EP_SUBMIT(test_data->out_ep, req);
  if (ret != 0)
    {
      usb_err("out_ep submit failed!\n");
      goto errout;
    }

  test_data->write_busy = 0;

  if (test_data->connected == 0)
    {
      test_data->connected = 1;
    }

  return;

errout:
  usbclass_test_disconnect(driver, dev);
}

static int usbclass_test_setup(struct usbdevclass_driver_s *driver, struct usbdev_s *dev,
                               const struct usb_ctrlreq_s *ctrl, uint8_t *dataout, size_t outlen)
{
  uint8_t req_type;
  struct test_dev_s *test_data;
  struct test_driver_s *drvr;
  struct usbdev_req_s *req;
  (void)dataout; (void)outlen;

  if (dev == NULL || driver == NULL || ctrl == NULL)
    {
      return -1;
    }

  drvr = (struct test_driver_s *)driver;
  test_data  = drvr->dev;
  if (test_data == NULL)
    {
      return -1;
    }

  req       = dev->ep0->handle_req;
  req_type  = ctrl->type;
  req->priv = test_data;
  req->len  = 0;

  if ((req_type & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_STANDARD)
    {
      switch (ctrl->req)
        {
        case USB_REQ_SETCONFIGURATION:
        case USB_REQ_SETINTERFACE:
          usbclass_test_set_endpoint(driver, dev);
          break;

        default:
          break;
        }
    }

  return 0;
}

static void usbclass_test_disconnect(struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  struct test_driver_s *test_drvr;
  struct test_dev_s *test_data;

  (void)dev;

  test_drvr = (struct test_driver_s *)driver;
  test_data  = test_drvr->dev;
  if (test_data == NULL)
    {
      return;
    }
  test_data->connected = 0;

  if (test_data->in_ep_enabled == true)
    {
      test_data->write_busy = 1;

      (void)EP_DISABLE(test_data->in_ep);
      test_data->in_ep_enabled = false;
    }

  if (test_data->out_ep_enabled == true)
      {
        (void)EP_DISABLE(test_data->out_ep);
        test_data->out_ep_enabled = false;
      }

  if (test_data->event_flag == true && !LOS_ListEmpty(&test_data->read_event.stEventList))
    {
      /* If there is a read data task waiting, must write an event to notify the task to exit */

      (void)LOS_EventWrite(&test_data->read_event, USB_TEST_EXIT_EVENT);
    }
}

static uint8_t *link_ftest_descriptors(uint16_t *total_size)
{
  int i;
  uint8_t *des;
  uint8_t *temp_des;
  uint16_t temp_des_len;
  uint16_t ds = 0;
  errno_t ret;

  /* Add the length of descriptors one by one */

  for (i = 0; i < TEST_DESC_ARRAY_MAX_NUM && g_ftest_desc_array[i] != NULL; ++i)
    {
      ds += (uint16_t)(*g_ftest_desc_array[i]);
    }

  *total_size = ds;

  temp_des_len = skb_data_align(ds);
  des = memalign(USB_CACHE_ALIGN_SIZE, temp_des_len);
  if (des == NULL)
    {
      usb_err("System out of memory! Descriptors length: %u\n", ds);
      return NULL;
    }
  (void)memset_s((void *)des, temp_des_len, 0, temp_des_len);

  temp_des = des;

  /* Configuration descriptor needs to have the full length of rest of descriptors */

  USB_SETW(g_ftest_config_desc.totallen, ds);

  for (i = 0; i < TEST_DESC_ARRAY_MAX_NUM && g_ftest_desc_array[i] != NULL; ++i)
    {
      const uint8_t *des_src = g_ftest_desc_array[i];
      uint8_t des_len        = *des_src;
      ret = memcpy_s(temp_des, temp_des_len, des_src, des_len);
      if (ret != EOK)
        {
          usb_err("memcpy fail!\n");
          free(des);
          return NULL;
        }
      temp_des += des_len;
      temp_des_len -= des_len;
    }

  return des;
}

static void test_mkdevdesc(uint8_t *buf)
{
  errno_t ret;

  ret = memcpy_s(buf, USB_COMP_EP0_BUFSIZ, &g_ftest_device_desc, sizeof(g_ftest_device_desc));
  if (ret != EOK)
    {
      usb_err("memcpy_s fail!, ret:%d\n", ret);
      return;
    }
}

static int16_t test_mkcfgdesc(uint8_t *buf, struct usbdev_devinfo_s *devinfo)
{
  uint16_t total_len;
  uint8_t *des;
  errno_t ret;

  (void)devinfo;

  des = link_ftest_descriptors(&total_len);
  if (des == NULL)
    {
      return 0;
    }

  ret = memcpy_s(buf, USB_COMP_EP0_BUFSIZ, des, total_len);
  if (ret != EOK)
    {
      usb_err("memcpy_s fail!, ret:%d\n", ret);
      free(des);
      return 0;
    }
  free(des);

  return (int16_t)total_len;
}

static int test_mkstrdesc(uint8_t id, struct usb_strdesc_s *buf)
{
  errno_t ret;
  const char *str;
  int i;

  for (i = 0; g_ftest_device_strings[i].s != NULL; i++)
    {
      str = g_ftest_device_strings[i].s;
      if (g_ftest_device_strings[i].id == id)
        {
          ret = memcpy_s(buf, USB_COMP_EP0_BUFSIZ, (const void *)str, (uint32_t)str[0]);
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

#define TEST_NCONFIGS    1
#define TEST_CONFIGID    0
#define TEST_NSTRIDS     5
static void test_get_composite_devdesc(struct composite_devdesc_s *dev)
{
  (void)memset_s(dev, sizeof(struct composite_devdesc_s), 0, sizeof(struct composite_devdesc_s));

  dev->mkdevdesc  = test_mkdevdesc;
  dev->mkconfdesc = test_mkcfgdesc;
  dev->mkstrdesc  = test_mkstrdesc;

  dev->nconfigs = TEST_NCONFIGS; /* Number of configurations supported */
  dev->configid = TEST_CONFIGID; /* The only supported configuration ID */

  dev->devinfo.ninterfaces = 1; /* Number of interfaces in the configuration */
  dev->devinfo.nendpoints = 2;  /* Number of endpoint in the configuration */
  dev->devinfo.nstrings = TEST_NSTRIDS; /* Number of Strings */
}

static int test_classobject(int minor, struct usbdev_devinfo_s *devinfo,
                            struct usbdevclass_driver_s **classdev)
{
  struct test_softc *test_s;
  struct test_dev_s *priv;
  struct test_driver_s *drvr;

  (void)minor;
  (void)devinfo;

  /* Allocate the structures needed */

  test_s = (struct test_softc *)malloc(sizeof(struct test_softc));
  if (test_s == NULL)
    {
      return -1;
    }

  /* Convenience pointers into the allocated blob */

  priv = &test_s->dev;
  drvr = &test_s->drvr;

  /* Initialize the USB test driver structure */

  (void)memset_s(priv, sizeof(struct test_dev_s), 0, sizeof(struct test_dev_s));

  /* Initialize the USB class driver structure */

  drvr->drvr.speed = USB_SPEED_HIGH;
  drvr->drvr.ops   = &g_test_driverops;
  drvr->dev        = priv;

  *classdev = &drvr->drvr;
  return 0;
}

static void test_uninitialize(struct usbdevclass_driver_s *classdev)
{
  struct test_driver_s *test_drvr = (struct test_driver_s *)classdev;
  struct test_dev_s *priv;
  struct test_softc *test_s;

  if (test_drvr == NULL)
    {
      return;
    }

  priv = test_drvr->dev;
  if (priv == NULL)
    {
      return;
    }

  test_s = container_of(test_drvr, struct test_softc, drvr);
  if (test_s != NULL)
    {
      free(test_s);
    }
}

static void usbdev_test_initialize_sub(struct composite_devdesc_s *dev, int ifnobase, int minor)
{
  /* Ask the test driver to fill in the constants we didn't
   * know here.
   */

  test_get_composite_devdesc(dev);

  /* The callback functions for the TEST class */

  dev->classobject = test_classobject;
  dev->uninitialize = test_uninitialize;

  dev->devinfo.ifnobase = ifnobase; /* Offset to Interface-IDs */
  dev->minor = minor;               /* The minor interface number */

  dev->devinfo.strbase = 0;         /* Offset to String Numbers */
}

int usbdev_test_initialize(void *handle)
{
  struct composite_devdesc_s dev;
  int ret;

  usbdev_test_initialize_sub(&dev, 0, DEV_CUSTOM);

  ret = composite_initialize_softc(1, &dev, handle);
  if (ret < 0)
    {
      return -1;
    }

  PRINTK("  ** test device initialized successfully! **\n");
  return 0;
}
