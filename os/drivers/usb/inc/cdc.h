#pragma once

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "em_msc.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "em_chip.h"
#include "em_core.h"
#include "em_device.h"

#include "descriptors.h"

/* USB CDC definitions */

#define CDC_BULK_EP_SIZE    USB_FS_BULK_EP_MAXSIZE
#define CDC_USB_RX_BUF_SIZ  CDC_BULK_EP_SIZE
#define CDC_USB_TX_BUF_SIZ  CDC_BULK_EP_SIZE

extern uint32_t gloXferred;
extern uint8_t usbRxBuffer[];

/* USB CDC functions */

int CDC_SetupCmd(const USB_Setup_TypeDef *setup);
void CDC_StateChangeEvent(USBD_State_TypeDef oldState, USBD_State_TypeDef newState);
int UsbDataReceived(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);

/* USB callbacks */

static const USBD_Callbacks_TypeDef callbacks = {
    .usbReset        = NULL,
    .usbStateChange  = CDC_StateChangeEvent,
    .setupCmd        = CDC_SetupCmd,
    .isSelfPowered   = NULL,
    .sofInt          = NULL
};

static const USBD_Init_TypeDef usbInitStruct = {
    .deviceDescriptor    = &USBDESC_deviceDesc,
    .configDescriptor    = USBDESC_configDesc,
    .stringDescriptors   = USBDESC_strings,
    .numberOfStrings     = sizeof(USBDESC_strings) / sizeof(void*),
    .callbacks           = &callbacks,
    .bufferingMultiplier = USBDESC_bufferingMultiplier,
    .reserved            = 0
};
