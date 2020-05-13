#ifndef __SILICON_LABS_USBCONFIG_H__
#define __SILICON_LABS_USBCONFIG_H__

#define USB_DEVICE

#define NUM_EP_USED 3

#define CDC_CTRL_INTERFACE_NO (0)
#define CDC_DATA_INTERFACE_NO (1)

#define CDC_EP_DATA_OUT       (0x01)
#define CDC_EP_DATA_IN        (0x81)
#define CDC_EP_NOTIFY         (0x82)

#endif /* __SILICON_LABS_USBCONFIG_H__ */
