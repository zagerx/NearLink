/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2023. All rights reserved.
 * Description: LiteOS USB Driver UAC Protocol
 * Author: Huawei LiteOS Team
 * Create: 2017-12-12
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

#include "gadget/f_uac_pri.h"
#include "gadget/usbd_audio_pri.h"
#include "implementation/usb_list.h"

static int usbclass_uac_bind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev);
static int usbclass_uac_unbind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev);
static int usbclass_uac_setup(struct usbdevclass_driver_s *driver, struct usbdev_s *dev,
                              const struct usb_ctrlreq_s *ctrl, uint8_t *dataout, size_t outlen);
static void usbclass_uac_disconnect(struct usbdevclass_driver_s *driver, struct usbdev_s *dev);

/* USB driver operations */

static const struct usbdevclass_driverops_s g_uac_driverops =
{
  usbclass_uac_bind,
  usbclass_uac_unbind,
  usbclass_uac_setup,
  usbclass_uac_disconnect,
  NULL,
  NULL
};

static const char g_fuac_str_lang[] =
{
  4, USB_DESC_TYPE_STRING,
  0x09, 0x04
};

static const char g_fuac_str_ac_if[] =
{
  UAC_AC_IF_STRDESC_LEN, USB_DESC_TYPE_STRING,
  'H', 0, 'i', 0, 's', 0, 'i', 0, 'l', 0, 'i', 0, 'c', 0, 'o', 0, 'n', 0,
  ' ', 0, 'H', 0, 'D', 0, ' ', 0, 'A', 0, 'u', 0, 'd', 0, 'i', 0, 'o', 0,
#ifndef UAC_VER_1_0
  '2', 0, '.', 0, '0', 0
#endif
};

static const char g_fuac_usb_in_it[] =
{
  46, USB_DESC_TYPE_STRING,
  'C', 0, 'a', 0, 'p', 0, 't', 0, 'u', 0, 'r', 0, 'e', 0, ' ', 0, 'I', 0,
  'n', 0, 'p', 0, 'u', 0, 't', 0, ' ', 0, 't', 0, 'e', 0, 'r', 0, 'm', 0,
  'i', 0, 'n', 0, 'a', 0, 'l', 0
};

static const char g_fuac_usb_in_ot[] =
{
  48, USB_DESC_TYPE_STRING,
  'U', 0, 'S', 0, 'B', 0, ' ', 0, 'I', 0, 'n', 0, ' ', 0, ' ', 0, 'O', 0,
  'u', 0, 't', 0, 'p', 0, 'u', 0, 't', 0, ' ', 0, 't', 0, 'e', 0, 'r', 0,
  'm', 0, 'i', 0, 'n', 0, 'a', 0, 'l', 0
};

static const char g_fuac_usb_out_it[] =
{
  48, USB_DESC_TYPE_STRING,
  'U', 0, 'S', 0, 'B', 0, ' ', 0, 'O', 0, 'u', 0, 't', 0, ' ', 0, ' ', 0,
  'I', 0, 'n', 0, 'p', 0, 'u', 0, 't', 0, ' ', 0, 't', 0, 'e', 0, 'r', 0,
  'm', 0, 'i', 0, 'n', 0, 'a', 0, 'l', 0
};

static const char g_fuac_usb_out_ot[] =
{
  48, USB_DESC_TYPE_STRING,
  'C', 0, 'a', 0, 'p', 0, 't', 0, 'u', 0, 'r', 0, 'e', 0, ' ', 0, 'O', 0,
  'u', 0, 't', 0, 'p', 0, 'u', 0, 't', 0, ' ', 0, 't', 0, 'e', 0, 'r', 0,
  'm', 0, 'i', 0, 'n', 0, 'a', 0, 'l', 0
};

size_t g_uac_iso_data_size = 16;

#ifdef UAC_VER_1_0
struct uac_opts g_uac_opts =
{
  UAC_DEF_CCHMASK, UAC_DEF_CSRATE, UAC_DEF_CSSIZE,
  UAC_DEF_PCHMASK, UAC_DEF_PSRATE, UAC_DEF_PSSIZE
};

int fuac_opts_set(const struct uac_opts *opts)
{
  if (opts == NULL)
    {
      usb_err("opts is NULL\n");
      return -UAC_ERROR_PTR;
    }

  if (opts->c_chmask > UAC_DEF_CCHMASK || opts->c_chmask < 1 ||
      opts->p_chmask > UAC_DEF_PCHMASK || opts->p_chmask < 1)
    {
      usb_err("c_chmask is invalid %d or p_chmask is invalid %d\n", opts->c_chmask, opts->p_chmask);
      return -UAC_ERROR_VALUE;
    }

  if (opts->c_ssize > UAC_DEF_CSSIZE || opts->c_ssize < 1 ||
      opts->p_ssize > UAC_DEF_PSSIZE || opts->p_ssize < 1)
    {
      usb_err("c_ssize is invalid %d or p_ssize is invalid %d\n", opts->c_ssize, opts->p_ssize);
      return -UAC_ERROR_VALUE;
    }

  if (opts->c_srate < UAC_SAMPLING_RATE_8K || opts->c_srate > UAC_SAMPLING_RATE_48K)
    {
      usb_err("c_srate is invalid %d\n", opts->c_srate);
      return -UAC_ERROR_VALUE;
    }

  if (opts->p_srate < UAC_SAMPLING_RATE_8K || opts->p_srate > UAC_SAMPLING_RATE_48K)
    {
      usb_err("p_srate is invalid %d\n", opts->p_srate);
      return -UAC_ERROR_VALUE;
    }

  (void)memcpy_s(&g_uac_opts, sizeof(g_uac_opts), opts, sizeof(struct uac_opts));

  return 0;
}

uint32_t fuac_rate_get(void)
{
  return g_uac_opts.p_srate;
}

int fuac_get_opts(struct uac_opts *opts)
{
  if (opts == NULL)
    {
      usb_err("opts is NULL\n");
      return -UAC_ERROR_PTR;
    }

  (void)memcpy_s(opts, sizeof(struct uac_opts), &g_uac_opts, sizeof(g_uac_opts));
  return 0;
}
#else
struct uac2_opts g_uac2_opts =
{
  .c_chmask     = UAC_DEF_CCHMASK,
  .c_ssize      = UAC_DEF_CSSIZE,
  .c_srate_curr = UAC_DEF_CSRATE,
  .c_srate      = { UAC_SAMPLING_RATE_8K, UAC_SAMPLING_RATE_32K, UAC_SAMPLING_RATE_48K,
                   UAC_SAMPLING_RATE_96K, UAC_SAMPLING_RATE_192K, UAC_SAMPLING_RATE_384K },
  .p_chmask     = UAC_DEF_PCHMASK,
  .p_ssize      = UAC_DEF_PSSIZE,
  .p_srate_curr = UAC_DEF_PSRATE,
  .p_srate      = { UAC_SAMPLING_RATE_8K, UAC_SAMPLING_RATE_32K, UAC_SAMPLING_RATE_48K,
                   UAC_SAMPLING_RATE_96K, UAC_SAMPLING_RATE_192K, UAC_SAMPLING_RATE_384K }
};

int fuac2_get_opts(struct uac2_opts *opts)
{
  if (opts == NULL)
    {
      usb_err("opts is NULL\n");
      return -UAC_ERROR_PTR;
    }
  (void)memcpy_s(opts, sizeof(struct uac2_opts), &g_uac2_opts, sizeof(g_uac2_opts));
  return 0;
}

int fuac2_set_opts(const struct uac2_opts *opts)
{
  if (opts == NULL)
    {
      usb_err("opts is NULL\n");
      return -UAC_ERROR_PTR;
    }

  if (opts->c_chmask > UAC_DEF_CCHMASK || opts->c_chmask < 1 ||
      opts->p_chmask > UAC_DEF_PCHMASK || opts->p_chmask < 1)
    {
      usb_err("c_chmask is invalid %d or p_chmask is invalid %d\n", opts->c_chmask, opts->p_chmask);
      return -UAC_ERROR_VALUE;
    }

  if (opts->c_ssize > UAC_DEF_CSSIZE || opts->c_ssize < 1 ||
      opts->p_ssize > UAC_DEF_PSSIZE || opts->p_ssize < 1)
    {
      usb_err("c_ssize is invalid %d or p_ssize is invalid %d\n", opts->c_ssize, opts->p_ssize);
      return -UAC_ERROR_VALUE;
    }

  for (int i = 0; i < UAC2_MAX_RATES; i++)
    {
      if (opts->c_srate[i] < UAC_SAMPLING_RATE_8K || opts->c_srate[i] > UAC_SAMPLING_RATE_384K)
        {
          usb_err("c_srate[%d] is invalid %d\n", i, opts->c_srate[i]);
          return -UAC_ERROR_VALUE;
        }
      if (opts->p_srate[i] < UAC_SAMPLING_RATE_8K || opts->p_srate[i] > UAC_SAMPLING_RATE_384K)
        {
          usb_err("p_srate[%d] is invalid %d\n", i, opts->p_srate[i]);
          return -UAC_ERROR_VALUE;
        }
    }

  if (opts->c_srate_curr < UAC_SAMPLING_RATE_8K || opts->c_srate_curr > UAC_SAMPLING_RATE_384K)
    {
      usb_err("c_srate_curr is invalid %u\n", opts->c_srate_curr);
      return -UAC_ERROR_VALUE;
    }

  if (opts->p_srate_curr < UAC_SAMPLING_RATE_8K || opts->p_srate_curr > UAC_SAMPLING_RATE_384K)
    {
      usb_err("p_srate_curr is invalid %u\n", opts->p_srate_curr);
      return -UAC_ERROR_VALUE;
    }

  (void)memcpy_s(&g_uac2_opts, sizeof(g_uac2_opts), opts, sizeof(struct uac2_opts));
  return 0;
}
#endif

static struct uac_softc *g_fuac_softc = NULL;

struct uac_softc *get_uac(void)
{
  return g_fuac_softc;
}

static uint8_t fuac_channel_num_get(int channel_mask)
{
  uint8_t num = 0;

  while (channel_mask > 0)
    {
      if (channel_mask & 1)
        {
          num++;
        }
      channel_mask >>= 1;
    }

  return num;
}

static struct usb_devdesc_s g_fuac_device_desc =
{
  .len          = sizeof(struct usb_devdesc_s),
  .type         = USB_DESC_TYPE_DEVICE, /* Constant for device descriptor */
  HSETW(.usb, USB_VERSION_BCD),         /* USB version required: 2.0 */
  .classid      = USB_CLASS_MISC,       /* Miscellaneous Device Class */
  .subclass     = 0x2,                  /* Common Class */
  .protocol     = 0x1,                  /* Interface Association Descriptor */
  .mxpacketsize = 64,                   /* Control Endpoint packet size */
  HSETW(.vendor,  0),                   /* Vendor ID */
  HSETW(.product, 0),                   /* Product ID */
  HSETW(.device,  0x318),               /* Device release code */
  .imfgr        = 1,                    /* Manufacturer name, string index */
  .iproduct     = 2,                    /* Product name, string index */
  .serno        = 3,                    /* Device serial number, string index */
  .nconfigs     = 1                     /* One Configuration */
};

static struct usb_cfgdesc_s g_fuac_config_desc =
{
  .len         = sizeof(struct usb_cfgdesc_s),
  .type        = USB_DESC_TYPE_CONFIG,
  HSETW(.totallen, 0),                     /* Size of all descriptors, set later */
  .ninterfaces = UAC_NINTERFACES,          /* Number of Interfaces */
  .cfgvalue    = 0x1,                      /* ID of this configuration */
  .icfg        = 0x0,                      /* Index of string descriptor */
  .attr        = 0xc0,                     /* Self-powered */
  .mxpower     = 0x1                       /* Maximum power consumption from the bus */
};

static struct usb_iaddesc_s g_fuac_iad =
{
  .len       = sizeof(struct usb_iaddesc_s),
  .type      = USB_DESC_TYPE_INTERFACEASSOCIATION,
  .firstif   = 0,               /* Interface number of VideoControl interface */
  .nifs      = UAC_NINTERFACES, /* Number of contiguous Video interfaces */
  .classid   = USB_CLASS_AUDIO,
  .subclass  = 0,
  .protocol  = UAC_VERSION,
  .ifunction = STR_AC_IF        /* Index of string descriptor */
};

static struct usb_ifdesc_s g_fuac_ac_intf_desc =
{
  .len      = sizeof(struct usb_ifdesc_s),
  .type     = USB_DESC_TYPE_INTERFACE,
  .ifno     = 0,         /* Index number of this interface */
  .alt      = 0,         /* Index of this settings */
  .neps     = 0,         /* One endpoint */
  .classid  = USB_CLASS_AUDIO,
  .subclass = UAC_AUDIOCONTROL,
  .protocol = UAC_VERSION,
  .iif      = STR_AC_IF  /* Index of string descriptor */
};

static struct uac_ac_header_descriptor g_fuac_ac_head_desc =
{
  .bLength            = sizeof(struct uac_ac_header_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC_HEADER,
  .bcdADC             = UAC_SPEC_VERSION,       /* UAC specification version */
  .wTotalLength       = 0,                      /* total length, currently not set */
#ifdef UAC_VER_1_0
  .bIncollection      = 1 + UAC_GADGET_SPEAKER, /* Number of streaming interfaces */
  .bIfnr              = {0x01}                  /* Associated Audio Streaming Interface */
#else
  .bCategory          = UAC2_AFC_IO_BOX,
  .bmControls         = 0,
#endif
};

#ifndef UAC_VER_1_0
static const struct uac2_clock_source_descriptor g_fuac2_clk_src_in_desc =
{
  .bLength            = sizeof(struct uac2_clock_source_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC2_CLOCK_SOURCE,
  .bClockID           = IN_CLK_ID,
  .bmAttributes       = UAC2_CS_TYPE_INT_FIXED,
  .bmControls         = CONTROL_RDWR,
  .bAssocTerminal     = 0,
};
#endif

static struct uac_input_terminal_descriptor g_fuac_in_it_desc =
{
  .bLength            = sizeof(struct uac_input_terminal_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC_INPUT_TERMINAL,
  .bTerminalID        = IN_IT_ID,
  .wTerminalType      = UAC_IN_MICROPHONE,
  .bAssocTerminal     = 0,
  .bNrChannels        = UAC_CHANNEL_NUM,
  .bmChannelConfig    = UAC_DEF_PCHMASK,
  .iChannelNames      = 0x10,
  .iTerminal          = UAC_USB_IN_IT,
#ifndef UAC_VER_1_0
  .bCSourceID         = IN_CLK_ID,
  .bmControls         = CONTROL_RDWR,
#endif
};

static const struct uac_output_terminal_descriptor g_fuac_in_ot_desc =
{
  .bLength            = sizeof(struct uac_output_terminal_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC_OUTPUT_TERMINAL,
  .wTerminalType      = UAC_TERMINAL_STREAMING,
  .bAssocTerminal     = 0,
  .iTerminal          = UAC_USB_IN_OT,
  .bTerminalID        = IN_OT_ID,
  .bSourceID          = IN_IT_ID,
#ifndef UAC_VER_1_0
  .bCSourceID         = IN_CLK_ID,
  .bmControls         = CONTROL_RDWR,
#endif
};

static struct usb_ifdesc_s g_fuac_as_in_alt_0_desc =
{
  .len      = sizeof(struct usb_ifdesc_s),
  .type     = USB_DESC_TYPE_INTERFACE,
  .ifno     = 1,
  .alt      = 0,
  .neps     = 0,
  .classid  = USB_CLASS_AUDIO,
  .subclass = UAC_AUDIOSTREAMING,
  .protocol = UAC_VERSION,
  .iif      = 0,
};

static struct usb_ifdesc_s g_fuac_as_in_alt_1_desc =
{
  .len      = sizeof(struct usb_ifdesc_s),
  .type     = USB_DESC_TYPE_INTERFACE,
  .ifno     = 1,
  .alt      = 1,
  .neps     = 1,
  .classid  = USB_CLASS_AUDIO,
  .subclass = UAC_AUDIOSTREAMING,
  .protocol = UAC_VERSION,
  .iif      = 0,
};

static struct uac_as_header_descriptor g_fuac_as_in_head_desc =
{
  .bLength            = sizeof(struct uac_as_header_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC_HEADER,
  .bTerminalLink      = IN_OT_ID,
#ifdef UAC_VER_1_0
  .bDelay             = 0,
  .wFormatTag         = 1,
#else
  .bNrChannels        = UAC_CHANNEL_NUM,
  .bmChannelConfig    = UAC_DEF_CCHMASK,
  .bmControls         = 0,
  .bFormatType        = UAC_FORMAT_TYPE_I,
  .bmFormats          = UAC_FORMAT_TYPE_I_PCM,
  .iChannelNames      = 0,
#endif
};

static struct uac_format_type_i_discrete_descriptor g_fuac_as_in_type_i_desc =
{
  .bLength            = sizeof(struct uac_format_type_i_discrete_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC_FORMAT_TYPE,
  .bFormatType        = UAC_FORMAT_TYPE_I,
  .bBitResolution     = 16,
#ifdef UAC_VER_1_0
  .bNrChannels        = UAC_CHANNEL_NUM,
  .bSubFrameSize      = 2,
  .bSamFreqType       = 4,
  HSETM(.tSamFreq[0], UAC_SAMPLING_RATE_8K),
  HSETM(.tSamFreq[1], UAC_SAMPLING_RATE_16K),
  HSETM(.tSamFreq[2], UAC_SAMPLING_RATE_32K),
  HSETM(.tSamFreq[3], UAC_SAMPLING_RATE_48K)
#else
  .bSubslotSize       = 2,
#endif
};

/* Standard ISO IN Endpoint Descriptor */

struct uac_endpoint_descriptor g_fuac_as_in_ep_desc =
{
  .bLength          = sizeof(struct uac_endpoint_descriptor),
  .bDescriptorType  = USB_DESC_TYPE_ENDPOINT,

  /* Hi3516ev200 platform needs to specify endpoint number, otherwise the camera audio works abnormally.
   * This way is compatible with other platforms.
   */

  .bEndpointAddress = USB_DIR_IN | 0x1,
  .bmAttributes     = UAC_EP_TRANSFER_TYPE,
  HSETW(.wMaxPacketSize, ISO_DATA_SIZE),
  .bInterval        = UAC_INTERVAL,
#ifdef UAC_VER_1_0
  .bRefresh         = 0,
  .bSynchAddress    = 0,
#endif
};

static const struct uac_iso_endpoint_descriptor g_fuac_as_iso_in_desc =
{
  .bLength            = sizeof(struct uac_iso_endpoint_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSENDPOINT,
  .bDescriptorSubtype = UAC_EP_GENERAL,
  .bLockDelayUnits    = 0,
  .wLockDelay         = 0,
#ifdef UAC_VER_1_0
  .bmAttributes       = 1,
#else
  .bmAttributes       = 0,
  .bmControls         = 0,
#endif
};

#ifndef UAC_VER_1_0
static const struct uac2_clock_source_descriptor g_fuac2_clk_src_out_desc =
{
  .bLength            = sizeof(struct uac2_clock_source_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC2_CLOCK_SOURCE,
  .bClockID           = OUT_CLK_ID,
  .bmAttributes       = UAC2_CS_TYPE_INT_FIXED,
  .bmControls         = CONTROL_RDWR,
  .bAssocTerminal     = 0,
};
#endif

#if UAC_GADGET_SPEAKER
/* USB out for speaker */

static struct uac_input_terminal_descriptor g_fuac_out_it_desc =
{
  .bLength            = sizeof(struct uac_input_terminal_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC_INPUT_TERMINAL,
  .wTerminalType      = UAC_TERMINAL_STREAMING,
  .bAssocTerminal     = 0,
  .bNrChannels        = UAC_CHANNEL_NUM,
  .bmChannelConfig    = UAC_DEF_PCHMASK,
  .iChannelNames      = 0,
  .iTerminal          = UAC_USB_OUT_IT,
  .bTerminalID        = OUT_IT_ID,
#ifndef UAC_VER_1_0
  .bCSourceID         = OUT_CLK_ID,
  .bmControls         = CONTROL_RDWR,
#endif
};

static const struct uac_output_terminal_descriptor g_fuac_out_ot_desc =
{
  .bLength            = sizeof(struct uac_output_terminal_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC_OUTPUT_TERMINAL,
  .wTerminalType      = UAC_OUT_SPEAKER,
  .iTerminal          = UAC_USB_OUT_OT,
  .bSourceID          = OUT_IT_ID,
  .bAssocTerminal     = 0,
  .bTerminalID        = OUT_OT_ID,
#ifndef UAC_VER_1_0
  .bCSourceID         = OUT_CLK_ID,
  .bmControls         = CONTROL_RDWR,
#endif
};

static struct usb_ifdesc_s g_fuac_as_out_alt_0_desc =
{
  .len      = sizeof(struct usb_ifdesc_s),
  .type     = USB_DESC_TYPE_INTERFACE,
  .ifno     = 1,
  .alt      = 0,
  .neps     = 0,
  .classid  = USB_CLASS_AUDIO,
  .subclass = UAC_AUDIOSTREAMING,
  .protocol = UAC_VERSION,
  .iif      = 0,
};

static struct usb_ifdesc_s g_fuac_as_out_alt_1_desc =
{
  .len      = sizeof(struct usb_ifdesc_s),
  .type     = USB_DESC_TYPE_INTERFACE,
  .ifno     = 1,
  .alt      = 1,
  .neps     = 1,
  .classid  = USB_CLASS_AUDIO,
  .subclass = UAC_AUDIOSTREAMING,
  .protocol = UAC_VERSION,
  .iif      = 0,
};

static struct uac_as_header_descriptor g_fuac_as_out_head_desc =
{
  .bLength            = sizeof(struct uac_as_header_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC_AS_GENERAL,
  .bTerminalLink      = OUT_IT_ID,
#ifdef UAC_VER_1_0
  .bDelay             = 0,
  .wFormatTag         = 1,
#else
  .bmControls         = 0,
  .bFormatType        = UAC_FORMAT_TYPE_I,
  .bmFormats          = UAC_FORMAT_TYPE_I_PCM,
  .bNrChannels        = UAC_CHANNEL_NUM,
  .bmChannelConfig    = UAC_DEF_PCHMASK,
  .iChannelNames      = 0,
#endif
};

static struct uac_format_type_i_discrete_descriptor g_fuac_as_out_type_i_desc =
{
  .bLength            = sizeof(struct uac_format_type_i_discrete_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSINTERFACE,
  .bDescriptorSubtype = UAC_FORMAT_TYPE,
  .bFormatType        = UAC_FORMAT_TYPE_I,
  .bBitResolution     = 16,
#ifdef UAC_VER_1_0
  .bNrChannels        = UAC_CHANNEL_NUM,
  .bSubFrameSize      = 2,
  .bSamFreqType       = 4,
  HSETM(.tSamFreq[0], UAC_SAMPLING_RATE_8K),
  HSETM(.tSamFreq[1], UAC_SAMPLING_RATE_16K),
  HSETM(.tSamFreq[2], UAC_SAMPLING_RATE_32K),
  HSETM(.tSamFreq[3], UAC_SAMPLING_RATE_48K),
#else
  .bSubslotSize       = 2,
#endif
};

/* Standard ISO OUT Endpoint Descriptor */

static struct uac_endpoint_descriptor g_fuac_as_out_ep_desc =
{
  .bLength            = sizeof(struct uac_endpoint_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_ENDPOINT,
  .bEndpointAddress   = 0,
  .bmAttributes       = USB_EP_ATTR_XFER_ISOC | USB_EP_ATTR_SYNC,
  HSETW(.wMaxPacketSize, ISO_DATA_SIZE),
  .bInterval          = UAC_INTERVAL,
#ifdef UAC_VER_1_0
  .bRefresh           = 0,
  .bSynchAddress      = 0,
#endif
};

static const struct uac_iso_endpoint_descriptor g_fuac_as_iso_out_desc =
{
  .bLength            = sizeof(struct uac_iso_endpoint_descriptor),
  .bDescriptorType    = USB_DESC_TYPE_CSENDPOINT,
  .bDescriptorSubtype = UAC_EP_GENERAL,
  .bLockDelayUnits    = 0,
  .wLockDelay         = 0,
#ifdef UAC_VER_1_0
  .bmAttributes       = 1,
#else
  .bmAttributes       = 0,
  .bmControls         = 0,
#endif
};
#endif

const uint8_t *g_fuac_descriptors_array[] =
{
  (const uint8_t *)&g_fuac_config_desc,
  (const uint8_t *)&g_fuac_iad,
  (const uint8_t *)&g_fuac_ac_intf_desc,
  (const uint8_t *)&g_fuac_ac_head_desc,

#ifndef UAC_VER_1_0
  (const uint8_t *)&g_fuac2_clk_src_in_desc,
  (const uint8_t *)&g_fuac2_clk_src_out_desc,
#endif

#if UAC_GADGET_SPEAKER
  (const uint8_t *)&g_fuac_out_it_desc,
  (const uint8_t *)&g_fuac_out_ot_desc,
#endif

  (const uint8_t *)&g_fuac_in_it_desc,
  (const uint8_t *)&g_fuac_in_ot_desc,

#if UAC_GADGET_SPEAKER
  (const uint8_t *)&g_fuac_as_out_alt_0_desc,
  (const uint8_t *)&g_fuac_as_out_alt_1_desc,
  (const uint8_t *)&g_fuac_as_out_head_desc,
  (const uint8_t *)&g_fuac_as_out_type_i_desc,
  (const uint8_t *)&g_fuac_as_out_ep_desc,
  (const uint8_t *)&g_fuac_as_iso_out_desc,
#endif

  (const uint8_t *)&g_fuac_as_in_alt_0_desc,
  (const uint8_t *)&g_fuac_as_in_alt_1_desc,
  (const uint8_t *)&g_fuac_as_in_head_desc,
  (const uint8_t *)&g_fuac_as_in_type_i_desc,
  (const uint8_t *)&g_fuac_as_in_ep_desc,
  (const uint8_t *)&g_fuac_as_iso_in_desc,
  NULL,
};

static uint8_t *link_fuac_descriptors(uint16_t *total_size)
{
  int i;
  uint8_t *des;
  uint8_t *temp_des;
  uint16_t cs;
  errno_t err;
  uint16_t temp_des_len;
  uint16_t ds = 0;

  /* Add the length of descriptors one by one */

  for (i = 0; g_fuac_descriptors_array[i]; i++)
    {
      ds += (uint16_t)(*g_fuac_descriptors_array[i]);
    }

  *total_size = ds;

  temp_des_len = skb_data_align(ds);
  des = memalign(USB_CACHE_ALIGN_SIZE, temp_des_len);
  if (des == NULL)
    {
      usb_err("System out of memory! Descriptors length: %u\n", ds);
      return NULL;
    }
  (void)memset_s(des, temp_des_len, 0, temp_des_len);

  temp_des = des;

  /* Configuration descriptor needs to have the full length of rest of descriptors */

  USB_SETW(g_fuac_config_desc.totallen, ds);

  /* Add the length of audio control descriptors */

  cs  = (uint16_t)g_fuac_ac_head_desc.bLength;
#ifndef UAC_VER_1_0
  cs += (uint16_t)g_fuac2_clk_src_in_desc.bLength;
  cs += (uint16_t)g_fuac2_clk_src_out_desc.bLength;
#endif
#if UAC_GADGET_SPEAKER
  cs += (uint16_t)g_fuac_out_it_desc.bLength;
  cs += (uint16_t)g_fuac_out_ot_desc.bLength;
#endif
  cs += (uint16_t)g_fuac_in_it_desc.bLength;
  cs += (uint16_t)g_fuac_in_ot_desc.bLength;

  g_fuac_ac_head_desc.wTotalLength = cs;

  for (i = 0; g_fuac_descriptors_array[i]; i++)
    {
      const uint8_t *des_src = g_fuac_descriptors_array[i];
      uint8_t des_len = *des_src;
      err = memcpy_s(temp_des, temp_des_len, des_src, des_len);
      if (err != EOK)
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

#if UAC_GADGET_SPEAKER

#define MAX_QUEUE_NUM   8

static void uac_free_out_queue(struct uac_dev_s *uac)
{
  struct uac_queue_node *node;

  while (!LOS_ListEmpty(&uac->out_queue))
    {
      node = LOS_LIST_FIRST_ENTRY(&uac->out_queue, struct uac_queue_node, irqqueue);
      LOS_ListDelInit(&node->irqqueue);
      uac_queue_node_free(node);
    }
  uac->out_queue_num = 0;
}

static void configure_out_endpoint(struct uac_dev_s *uac)
{
  struct usbdev_req_s *req = &uac->outputreq;

  if (uac->out_ep_enabled == true)
    {
      uac->out_ep_enabled = false;
      EP_DISABLE(uac->out_ep);
    }
  (void)EP_CONFIGURE(uac->out_ep, (const struct usb_epdesc_s *)&g_fuac_as_out_ep_desc, false);
  uac->out_ep_enabled = true;

  spin_lock(&uac->lock);
  uac_free_out_queue(uac);
  spin_unlock(&uac->lock);

  (void)EP_SUBMIT(uac->out_ep, req);
}

static void uac_receive_data(struct uac_dev_s *uac, const void *buf, int len)
{
  struct uac_queue_node *node;

  if (!uac->out_start)
    {
      return;
    }

  node = uac_queue_node_alloc(len);
  if (node == NULL)
    {
      usb_err("UAC_ERROR_MEMORY\n");
      return;
    }

  (void)memcpy_s(node->buf, len, buf, len);
  if (uac->out_queue_num > MAX_QUEUE_NUM)
    {
      uac_free_out_queue(uac);
    }

  LIST_ADD_TAIL(&node->irqqueue, &uac->out_queue);
  uac->out_queue_num++;
}

static void fuac_output_request_complete(struct usbdev_ep_s *ep,
                                         struct usbdev_req_s *req)
{
  struct uac_dev_s *uac = (struct uac_dev_s *)req->priv;
  uint32_t ret;

  (void)ep;

  if (req->result != 0)
    {
      return;
    }
  spin_lock(&uac->lock);
  if (req->xfrd > 0 && req->xfrd < ISO_DATA_SIZE)
    {
      uac_receive_data(uac, req->buf, req->xfrd);
      ret = LOS_EventWrite(&uac->out_event, UAC_READ_EVENT);
      if (ret != LOS_OK)
        {
          usb_err("write event failed :%u\n", ret);
        }
    }
  spin_unlock(&uac->lock);
  (void)EP_SUBMIT(uac->out_ep, req);
}
#else
static void configure_out_endpoint(struct uac_dev_s *uac)
{
  /* Speaker with audio disabled */

  (void)uac;
}
#endif

static void configure_in_endpoint(struct uac_dev_s *uac)
{
  uint32_t interval;

  spin_lock(&uac->lock);
  uac->busy_flag = 0;
  uac_queue_free(uac);
  interval = UAC_FACTOR >> (g_fuac_as_in_ep_desc.bInterval - 1);
#ifdef UAC_VER_1_0
  g_uac_iso_data_size = DIV_ROUND_UP(g_uac_opts.p_srate, interval) * g_uac_opts.p_ssize *
                        fuac_channel_num_get(g_uac_opts.p_chmask);

#else
  g_uac_iso_data_size = DIV_ROUND_UP(g_uac2_opts.p_srate_curr, interval) * g_uac2_opts.p_ssize *
                        fuac_channel_num_get(g_uac2_opts.p_chmask);
#endif
  spin_unlock(&uac->lock);

  if (uac->in_ep_enabled == true)
    {
      uac->in_ep_enabled = false;
      (void)EP_DISABLE(uac->in_ep);
      EP_FLUSH(uac->in_ep);
    }
  uac->in_ep_enabled = true;

  (void)EP_CONFIGURE(uac->in_ep, (const struct usb_epdesc_s *)&g_fuac_as_in_ep_desc, 0);
}

#ifdef UAC_VER_1_0
static void uac_set_sampling_rate(struct uac_dev_s *uac, uint32_t rate)
{
  int uac_rate = rate;
  if (uac->dir == USB_DIR_IN && g_uac_opts.p_srate != uac_rate)
    {
      g_uac_opts.p_srate = uac_rate;
      configure_in_endpoint(uac);
    }
  else if (uac->dir == USB_DIR_OUT && g_uac_opts.c_srate != uac_rate)
    {
      g_uac_opts.c_srate = uac_rate;
      configure_out_endpoint(uac);
    }
  uac->dir = UAC_DIR_INVALID;
}
#else
static void uac2_set_sampling_rate(struct uac_dev_s *uac, uint32_t rate)
{
  if (uac->clock_id == IN_CLK_ID && g_uac2_opts.p_srate_curr != rate)
    {
      g_uac2_opts.p_srate_curr = rate;
      configure_in_endpoint(uac);
    }
  if (uac->clock_id == OUT_CLK_ID && g_uac2_opts.c_srate_curr != rate)
    {
      g_uac2_opts.c_srate_curr = rate;
      configure_out_endpoint(uac);
    }
}
#endif

static void uac_setcur_complete(struct uac_dev_s *uac, uint32_t rate)
{
#ifdef UAC_VER_1_0
  uac_set_sampling_rate(uac, rate);
#endif
  if (uac->connected == 0)
    {
#ifdef UAC_VER_1_0
      PRINTK("<<<rate:%d, size:%u>>>\n", g_uac_opts.p_srate, g_uac_iso_data_size);
#else
      PRINTK("usb connected, out ep rate:%u, in ep rate:%u\n",
             g_uac2_opts.c_srate_curr, g_uac2_opts.p_srate_curr);
#endif
      spin_lock(&uac->lock);
      uac->connected = 1;
      spin_unlock(&uac->lock);
      configure_in_endpoint(uac);
      configure_out_endpoint(uac);
    }
}

static void fuac_request_complete(struct usbdev_ep_s *ep, struct usbdev_req_s *req)
{
  struct uac_dev_s *uac = (struct uac_dev_s *)req->priv;
  uint8_t *buf = (uint8_t *)req->buf;
  uint32_t rate = UAC_GETRATE(buf);
  (void)ep;

  switch (uac->control)
    {
    case UAC_SETCUR_COMPLETE:
      uac_setcur_complete(uac, rate);
      break;

#ifndef UAC_VER_1_0
    case UAC2_SET_SAMPLING_RATE:
      uac2_set_sampling_rate(uac, rate);
      break;
#endif

    default:
      break;
    }
  uac->control = ~0;
}

void fuac_input_req_complete(struct usbdev_ep_s *ep, struct usbdev_req_s *req)
{
  struct uac_dev_s *uac = (struct uac_dev_s *)req->priv;
  struct uac_queue_node *node_tx;
  struct usbdev_req_s *req_temp = &uac->inputreq;

  spin_lock(&uac->lock);
  if (req->result != 0)
    {
      uac_queue_free(uac);
      spin_unlock(&uac->lock);
      return;
    }

  if (uac->cur_node != NULL)
    {
      node_tx = uac->cur_node;
      if (node_tx->buf_len == 0)
        {
          uac_queue_node_free(node_tx);
          uac->cur_node = NULL;
        }
      else
        {
          uac_send_data_sub(uac);
          spin_unlock(&uac->lock);
          return;
        }
    }

  if (!LOS_ListEmpty(&uac->uac_queue))
    {
      node_tx = LOS_LIST_FIRST_ENTRY(&uac->uac_queue, struct uac_queue_node, irqqueue);
      uac->cur_node = node_tx;
      LOS_ListDelInit(&node_tx->irqqueue);
      uac->uac_queue_len--;
      uac_send_data_sub(uac);
    }
  else
    {
      req_temp->len = 0;
      (void)EP_SUBMIT(ep, req_temp);
    }
  spin_unlock(&uac->lock);
}

static void audio_set_endpoint_req(struct usbdev_s *dev, struct usbdev_req_s *req,
                                   const struct usb_ctrlreq_s *ctrl)
{
  struct uac_dev_s *uac = (struct uac_dev_s *)req->priv;
  uint8_t request = ctrl->req;
  uint8_t new_req = 0;

  uac->dir = UAC_DIR_INVALID;
  req->len = USB_GETW(ctrl->len);

  switch (request)
    {
#ifdef UAC_VER_1_0
    case SET_CUR_UAC:
    case SET_MIN_UAC:
    case SET_MAX_UAC:
    case SET_RES_UAC:
      uac->dir = USB_GETW(ctrl->index) & 0xF0;
#else
    case UAC2_CS_CUR:
      if (ctrl->value[1] == UAC2_CS_CONTROL_SAM_FREQ)
        {
          uac->clock_id = ctrl->index[1];
        }
#endif
      new_req++;
      break;

    default:
      usb_err("Unknown request: %#x\n", request);
      break;
    }

  if (new_req)
    {
      (void)EP_SUBMIT(dev->ep0, req);
    }
}

#ifdef UAC_VER_1_0
static void uac_get_rate_range(uint8_t *buf, int dir)
{
  if (dir == USB_DIR_IN)
    {
      buf[0] = (uint8_t)g_uac_opts.p_srate;
      buf[1] = (uint8_t)(g_uac_opts.p_srate >> 8);
      buf[2] = (uint8_t)(g_uac_opts.p_srate >> 16);
    }
  else if (dir == USB_DIR_OUT)
    {
      buf[0] = (uint8_t)g_uac_opts.c_srate;
      buf[1] = (uint8_t)(g_uac_opts.c_srate >> 8);
      buf[2] = (uint8_t)(g_uac_opts.c_srate >> 16);
    }
}
#else
static uint32_t uac2_get_rate_range(uint8_t *buf, uint32_t buf_len, uint8_t clock_id)
{
  errno_t ret;
  uint32_t len;

  struct ctl_ranges_lay3 rs = {0};
  for (int i = 0; i < UAC2_MAX_RATES; i++)
    {
      if (clock_id == IN_CLK_ID)
        {
          USB_SETDW(rs.range[i].dMIN, g_uac2_opts.p_srate[i]);
          USB_SETDW(rs.range[i].dMAX, g_uac2_opts.p_srate[i]);
        }
      else if (clock_id == OUT_CLK_ID)
        {
          USB_SETDW(rs.range[i].dMIN, g_uac2_opts.c_srate[i]);
          USB_SETDW(rs.range[i].dMAX, g_uac2_opts.c_srate[i]);
        }
    }

  USB_SETW(rs.wNumSubRanges, UAC2_MAX_RATES);
  len = MIN(buf_len, sizeof(rs));
  ret = memcpy_s(buf, buf_len, &rs, len);
  if (ret != EOK)
    {
      usb_err("memcpy_s fail!, ret:%d\n", ret);
      return 0;
    }
  return len;
}
#endif

static uint32_t audio_get_endpoint_req(struct usbdev_s *dev, struct usbdev_req_s *req,
                                       const struct usb_ctrlreq_s *ctrl)
{
  uint8_t request = ctrl->req;
  uint8_t new_req = 0;
  uint8_t *buf = (uint8_t *)req->buf;
  uint32_t ret = 0;

  req->len = USB_GETW(ctrl->len);

  switch (request)
    {
#ifdef UAC_VER_1_0
    case GET_CUR_UAC:
    case GET_MIN_UAC:
    case GET_MAX_UAC:
    case GET_RES_UAC:
      uac_get_rate_range(buf, USB_GETW(ctrl->index) & 0xF0);
      new_req++;
      break;

    case GET_MEM_UAC:
      break;
#else
    case UAC2_CS_RANGE:
      if (ctrl->value[1] == UAC2_CS_CONTROL_SAM_FREQ)
        {
          ret = uac2_get_rate_range(buf, req->len, ctrl->index[1]);
          new_req++;
          req->len = ret;
        }
      break;
#endif

    default:
      usb_err("Unknown request: %#x\n", request);
      break;
    }

  if (new_req)
    {
      (void)EP_SUBMIT(dev->ep0, req);
    }

  return ret;
}

static void fuac_source_free(const struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  const struct uac_driver_s *drvr;
  struct uac_dev_s *uac;

  drvr = (const struct uac_driver_s *)driver;
  uac  = drvr->dev;
  if (uac == NULL)
    {
      return;
    }

#if UAC_GADGET_SPEAKER
  if (uac->outputreq.buf != NULL)
    {
      free(uac->outputreq.buf);
      uac->outputreq.buf = NULL;
    }
  if (uac->out_ep != NULL)
    {
      DEV_FREEEP(dev, uac->out_ep);
      uac->out_ep = NULL;
    }

  (void)LOS_EventDestroy(&uac->out_event);
#endif

  if (uac->in_ep != NULL)
    {
      DEV_FREEEP(dev, uac->in_ep);
      uac->in_ep = NULL;
    }
}

static int usbclass_uac_bind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  struct usbdev_ep_s *ep;
  struct uac_driver_s *drvr;
  struct composite_dev_s *cdev;
  struct uac_dev_s *uac;
  struct composite_devobj_s *devobj;
  struct usbdev_devinfo_s *devinfo;

  if (driver == NULL || dev == NULL)
    {
      return -1;
    }

  cdev = dev->ep0->priv;
  if (cdev == NULL)
    {
      return -1;
    }

  drvr = (struct uac_driver_s *)driver;
  uac  = drvr->dev;
  if (uac == NULL)
    {
      return -1;
    }

  LOS_ListInit(&uac->uac_queue);
  uac->busy_flag     = 0;
  uac->uac_queue_len = 0;
  uac->cur_node      = NULL;
  uac->count         = 0;
  uac->dir           = UAC_DIR_INVALID;
  spin_lock_init(&uac->lock);

  devobj = usbclass_devobj_get(cdev, DEV_UAC);
  if (devobj == NULL)
    {
      return -1;
    }
  devinfo = &devobj->compdesc.devinfo;

#if UAC_GADGET_SPEAKER

  /* Initialize AudioStreaming OUT endpoint */

  LOS_ListInit(&uac->out_queue);
  (void)LOS_EventInit(&uac->out_event);
  uac->out_start     = false;
  uac->out_queue_num = 0;

  ep = DEV_ALLOCEP(dev, g_fuac_as_out_ep_desc.bEndpointAddress,
                   (struct usb_epdesc_s *)&g_fuac_as_out_ep_desc);
  if (ep == NULL)
    {
      (void)LOS_EventDestroy(&uac->out_event);
      return -1;
    }
  ep->priv    = (void *)uac;
  uac->out_ep = ep;

  (void)memset_s(&(uac->outputreq), sizeof(struct usbdev_req_s), 0, sizeof(struct usbdev_req_s));
  uac->outputreq.callback = fuac_output_request_complete;
  uac->outputreq.priv     = (void *)uac;
  uac->outputreq.buf      = memalign(USB_CACHE_ALIGN_SIZE, ISO_DATA_SIZE);
  if (uac->outputreq.buf == NULL)
    {
      usb_err("Malloc output buf failed!\n");
      goto fail;
    }
  uac->outputreq.len      = ISO_DATA_SIZE;
  ep->handle_req          = &uac->outputreq;
  devinfo->epno[0]        = ep->eplog;
#endif

  /* Initialize AudioStreaming IN endpoint */

  ep = DEV_ALLOCEP(dev, g_fuac_as_in_ep_desc.bEndpointAddress,
                   (struct usb_epdesc_s *)&g_fuac_as_in_ep_desc);
  if (ep == NULL)
    {
      goto fail;
    }
  (void)memset_s(&uac->inputreq, sizeof(struct usbdev_req_s), 0, sizeof(struct usbdev_req_s));
  uac->inputreq.callback            = fuac_input_req_complete;
  uac->inputreq.priv                = (void *)uac;
  uac->inputreq.buf                 = NULL;
  ep->priv                          = (void *)uac;
  ep->handle_req                    = &uac->inputreq;
  uac->in_ep                        = ep;
  devinfo->epno[UAC_GADGET_SPEAKER] = ep->eplog;
  uac->control                      = ~0;

  return 0;
fail:
  (void)usbclass_uac_unbind(driver, dev);
  return -1;
}

static int usbclass_uac_unbind(struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  struct composite_dev_s *cdev;
  struct composite_devobj_s *devobj;
  struct usbdev_devinfo_s *devinfo;

  if (driver == NULL || dev == NULL)
    {
      return -1;
    }

  usbclass_uac_disconnect(driver, dev);

  cdev = dev->ep0->priv;
  if (cdev == NULL)
    {
      return -1;
    }

  devobj = usbclass_devobj_get(cdev, DEV_UAC);
  if (devobj == NULL)
    {
      return -1;
    }
  devinfo = &devobj->compdesc.devinfo;
  (void)memset_s(devinfo, sizeof(struct usbdev_devinfo_s), 0, sizeof(struct usbdev_devinfo_s));

  fuac_source_free(driver, dev);
  return 0;
}

static int usbclass_uac_set_alt(struct uac_dev_s *dev, unsigned intf, unsigned alt)
{
  (void)intf;
  (void)alt;

  dev->busy_flag = 0;
  if (dev->in_ep_enabled == true)
    {
      (void)EP_DISABLE(dev->in_ep);
      dev->in_ep_enabled = false;
    }

  (void)EP_CONFIGURE(dev->in_ep, (const struct usb_epdesc_s *)&g_fuac_as_in_ep_desc, 0);
  dev->in_ep_enabled = true;

  return 0;
}

static int usbclass_uac_setup(struct usbdevclass_driver_s *driver, struct usbdev_s *dev,
                              const struct usb_ctrlreq_s *ctrl, uint8_t *dataout, size_t outlen)
{
  uint8_t req_type;
  struct uac_dev_s *uac;
  struct uac_driver_s *drvr;
  struct usbdev_req_s *req;

  (void)dataout;
  (void)outlen;

  if (dev == NULL || driver == NULL || ctrl == NULL)
    {
      return -1;
    }

  drvr = (struct uac_driver_s *)driver;
  uac  = drvr->dev;
  if (uac == NULL)
    {
      return -1;
    }

  req           = dev->ep0->handle_req;
  req_type      = ctrl->type;
  req->callback = fuac_request_complete;
  req->priv     = uac;

  if ((req_type & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_STANDARD)
    {
      switch (ctrl->req)
        {
        case USB_REQ_SETCONFIGURATION:
        case USB_REQ_SETINTERFACE:
          (void)usbclass_uac_set_alt(uac, USB_GETW(ctrl->index), USB_GETW(ctrl->value));
          break;

        default:
          break;
        }
    }
  else
    {
      switch (req_type)
        {
        case USB_DIR_OUT | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE:
        case USB_DIR_OUT | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_ENDPOINT:
          audio_set_endpoint_req(dev, req, ctrl);
          uac->control = UAC_SET_EP_CONTROL;
          break;

        case USB_DIR_IN | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE:
        case USB_DIR_IN | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_ENDPOINT:

          /* The return value is applicable only to UAC2.0. */

          if (audio_get_endpoint_req(dev, req, ctrl) > 0)
            {
              uac->control = UAC_SETCUR_COMPLETE;
            }
          break;

        default:
          break;
        }
    }
  return 0;
}

static void usbclass_uac_disconnect(struct usbdevclass_driver_s *driver, struct usbdev_s *dev)
{
  struct uac_driver_s *uac_drvr;
  struct uac_dev_s *uac_dev;

  uac_drvr = (struct uac_driver_s *)driver;
  uac_dev  = uac_drvr->dev;
  if (uac_dev == NULL)
    {
      return;
    }

  if (uac_dev->in_ep_enabled == true)
    {
      (void)EP_DISABLE(uac_dev->in_ep);
      uac_dev->in_ep_enabled = false;
    }

#if UAC_GADGET_SPEAKER
  if (uac_dev->out_ep_enabled == true)
    {
      (void)EP_DISABLE(uac_dev->out_ep);
      uac_dev->out_ep_enabled = false;
    }

  spin_lock(&uac_dev->lock);
  uac_free_out_queue(uac_dev);
  spin_unlock(&uac_dev->lock);
#endif

  spin_lock(&uac_dev->lock);
  uac_dev->count = 0;
  uac_dev->connected = 0;
  uac_queue_free(uac_dev);
  spin_unlock(&uac_dev->lock);
}

static struct usbd_string g_fuac_device_strings[] =
{
  { 0,  g_fuac_str_lang },
  { 1,  NULL },
  { 2,  NULL },
  { 3,  NULL },
  { 4,  g_fuac_str_ac_if },
  { 8,  g_fuac_usb_in_it },
  { 9,  g_fuac_usb_in_ot },
  { 10, g_fuac_usb_out_it },
  { 11, g_fuac_usb_out_ot },
  USBD_DEVICE_STRINGS_END
};

void uac_get_device_dec_info(struct usbd_string **device_strings, struct usb_devdesc_s **device_desc)
{
  *device_strings = &g_fuac_device_strings[1];
  *device_desc    = &g_fuac_device_desc;
}

void uac_mkdevdesc(uint8_t *buf)
{
  errno_t ret;

  if (USB_GETW(g_fuac_device_desc.vendor) == 0)
    {
      usb_err("VID is not set!\n");
      return;
    }

  ret = memcpy_s(buf, USB_COMP_EP0_BUFSIZ, &g_fuac_device_desc, sizeof(g_fuac_device_desc));
  if (ret != EOK)
    {
      usb_err("memcpy_s fail!, ret:%d\n", ret);
      return;
    }
}

#ifdef UAC_VER_1_0
static void uac_set_channel_num(void)
{
  uint8_t p_channel_num = fuac_channel_num_get(g_uac_opts.p_chmask);
  g_fuac_in_it_desc.bNrChannels         = p_channel_num;
  g_fuac_as_in_type_i_desc.bNrChannels  = p_channel_num;
#if UAC_GADGET_SPEAKER
  uint8_t c_channel_num = fuac_channel_num_get(g_uac_opts.c_chmask);
  g_fuac_out_it_desc.bNrChannels        = c_channel_num;
  g_fuac_as_out_type_i_desc.bNrChannels = c_channel_num;
#endif
}
#else
static void uac_set_channel_num(void)
{
  uint8_t p_channel_num = fuac_channel_num_get(g_uac2_opts.p_chmask);
  uint8_t c_channel_num = fuac_channel_num_get(g_uac2_opts.c_chmask);
  g_fuac_in_it_desc.bNrChannels       = p_channel_num;
  g_fuac_as_in_head_desc.bNrChannels  = p_channel_num;
  g_fuac_out_it_desc.bNrChannels      = c_channel_num;
  g_fuac_as_out_head_desc.bNrChannels = c_channel_num;
}
#endif

int16_t uac_mkcfgdesc(uint8_t *buf, struct usbdev_devinfo_s *devinfo)
{
  uint16_t total_len;
  uint8_t *des;
  errno_t ret;

  g_fuac_iad.firstif = devinfo->ifnobase;
  g_fuac_ac_intf_desc.ifno = devinfo->ifnobase;
#if UAC_GADGET_SPEAKER
  g_fuac_as_out_alt_0_desc.ifno = devinfo->ifnobase + 1;
  g_fuac_as_out_alt_1_desc.ifno = devinfo->ifnobase + 1;
#endif
  g_fuac_as_in_alt_0_desc.ifno = devinfo->ifnobase + 1 + UAC_GADGET_SPEAKER;
  g_fuac_as_in_alt_1_desc.ifno = devinfo->ifnobase + 1 + UAC_GADGET_SPEAKER;
#ifdef UAC_VER_1_0
  g_fuac_ac_head_desc.bIfnr[0] = devinfo->ifnobase + 1;
  g_fuac_ac_head_desc.bIfnr[UAC_GADGET_SPEAKER] = devinfo->ifnobase + 1 + UAC_GADGET_SPEAKER;
#endif
  uac_set_channel_num();

  des = link_fuac_descriptors(&total_len);
  if (des != NULL)
    {
      ret = memcpy_s(buf, USB_COMP_EP0_BUFSIZ, des, total_len);
      if (ret != EOK)
        {
          usb_err("memcpy_s fail!, ret:%d\n", ret);
          free(des);
          return 0;
        }
      free(des);
    }

  return (int16_t)total_len;
}

int uac_mkstrdesc(uint8_t id, struct usb_strdesc_s *buf)
{
  errno_t ret;
  const char *str;
  uint32_t i;

  for (i = 0; i < UAC_NSTRIDS; i++)
    {
      str = g_fuac_device_strings[i].s;
      if (g_fuac_device_strings[i].id == id && str != NULL)
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

void uac_get_composite_devdesc(struct composite_devdesc_s *dev)
{
  (void)memset_s(dev, sizeof(struct composite_devdesc_s), 0, sizeof(struct composite_devdesc_s));

  dev->mkdevdesc  = uac_mkdevdesc;
  dev->mkconfdesc = uac_mkcfgdesc;
  dev->mkstrdesc  = uac_mkstrdesc;

  dev->nconfigs = UAC_NCONFIGS;           /* Number of configurations supported */
  dev->configid = UAC_CONFIGID;           /* The only supported configuration ID */

  dev->devinfo.ninterfaces = UAC_NINTERFACES; /* Number of interfaces in the configuration */
  dev->devinfo.nstrings    = UAC_NSTRIDS;     /* Number of Strings */
  dev->devinfo.nendpoints  = UAC_NUM_EPS;
}

int uac_classobject(int minor, struct usbdev_devinfo_s *devinfo,
                    struct usbdevclass_driver_s **classdev)
{
  struct uac_softc *uac_s;
  struct uac_dev_s *priv;
  struct uac_driver_s *drvr;

  (void)minor;
  (void)devinfo;

  /* Allocate the structures needed */

  uac_s = (struct uac_softc *)malloc(sizeof(struct uac_softc));
  if (uac_s == NULL)
    {
      return -1;
    }
  g_fuac_softc = uac_s;

  /* Convenience pointers into the allocated blob */

  priv = &uac_s->dev;
  drvr = &uac_s->drvr;

  /* Initialize the USB serial driver structure */

  (void)memset_s(priv, sizeof(struct uac_dev_s), 0, sizeof(struct uac_dev_s));

  /* Initialize the USB class driver structure */

  drvr->drvr.speed = USB_SPEED_HIGH;
  drvr->drvr.ops   = &g_uac_driverops;
  drvr->dev        = priv;

  *classdev = &drvr->drvr;
  return 0;
}

void uac_uninitialize(struct usbdevclass_driver_s *classdev)
{
  struct uac_driver_s *uac_drvr = (struct uac_driver_s *)classdev;
  struct uac_dev_s *priv;
  struct uac_softc *uac_s;

  dev_str_desc_free(&g_fuac_device_strings[1]);

  if (uac_drvr == NULL)
    {
      return;
    }

  priv = uac_drvr->dev;
  if (priv == NULL)
    {
      return;
    }

  uac_s = container_of(uac_drvr, struct uac_softc, drvr);
  free(uac_s);
  g_fuac_softc = NULL;
}

void usbdev_uac_initialize_sub(struct composite_devdesc_s *dev, int ifnobase, int minor)
{
  /* Ask the UAC driver to fill in the constants we didn't
   * know here.
   */

  uac_get_composite_devdesc(dev);

  /* The callback functions for the UAC class */

  dev->classobject  = uac_classobject;
  dev->uninitialize = uac_uninitialize;

  dev->devinfo.ifnobase = ifnobase; /* Offset to Interface-IDs */
  dev->minor            = minor;    /* The minor interface number */

  dev->devinfo.strbase = 0;         /* Offset to String Numbers */
}

int usbdev_uac_initialize(void *handle)
{
  struct composite_devdesc_s dev;
  int ret;

  usbdev_uac_initialize_sub(&dev, 0, DEV_UAC);

  ret = composite_initialize_softc(1, &dev, handle);
  if (ret < 0)
    {
      return -1;
    }

  PRINTK("  ** uac device initialized successfully! **\n");
  return 0;
}
