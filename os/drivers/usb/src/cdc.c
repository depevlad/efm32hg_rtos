/*
 * cdc.c
 *
 *  Created on: Apr 30, 2020
 *      Author: depevlad
 */

#include "../inc/cdc.h"
#include "../inc/usbconfig.h"

#include "../../../kernel/inc/os_msg.h"
#include "../../../kernel/inc/os_syscall.h"

uint8_t usbRxBuffer[CDC_USB_RX_BUF_SIZ] __attribute((aligned(4)));
uint32_t gloXferred;

int receiveData(USB_Status_TypeDef usbStatus, uint32_t xferred, uint32_t remaining) {
	if (usbStatus == USB_STATUS_OK && xferred > 0) {
		gloXferred = xferred;
		USBD_Read(CDC_EP_DATA_OUT, (void*)usbRxBuffer, CDC_USB_RX_BUF_SIZ, receiveData);
	}
	return USB_STATUS_OK;
}

int CDC_SetupCmd(const USB_Setup_TypeDef *setup) {
    return USB_STATUS_REQ_UNHANDLED;
}

void CDC_StateChangeEvent(USBD_State_TypeDef oldState, USBD_State_TypeDef newState) {
    if (newState == USBD_STATE_CONFIGURED) {
        USBD_Read(CDC_EP_DATA_OUT, (void*)usbRxBuffer, CDC_USB_RX_BUF_SIZ, receiveData);
    }
}

