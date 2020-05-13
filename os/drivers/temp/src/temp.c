/*****************************************************************************
 * @file temp.c
 * @brief Temperature measurements using internal temperature sensor
 * @author Silicon Labs
 * @version 1.02
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include "em_device.h"

#include "em_adc.h"
#include "em_cmu.h"
#include "em_emu.h"

#include "../inc/temp.h"

/* Flag used by the ISR to indicate that the ADC conversion is complete */
static volatile bool conversionDone = false;



/**********************************************************
 * Convert an ADC sample number to a degrees celcius.
 * See equation in Reference Manual
 *
 * @param adcSample
 *   The result of the ADC conversion when measuring
 *   the temperature sensor
 *
 * @returns
 *   The current temperature in degrees celcius
 **********************************************************/
static int32_t convertToCelsius(uint32_t adcSample)
{
  float temp;

  /* Factory calibration temperature from device information page. */
  float cal_temp_0 = (float)((DEVINFO->CAL & _DEVINFO_CAL_TEMP_MASK)
                             >> _DEVINFO_CAL_TEMP_SHIFT);

  float cal_value_0 = (float)((DEVINFO->ADC0CAL2
                               & _DEVINFO_ADC0CAL2_TEMP1V25_MASK)
                              >> _DEVINFO_ADC0CAL2_TEMP1V25_SHIFT);

  /* Temperature gradient (from datasheet) */
  float t_grad = -6.27;

  /* Calculate temperature */
  temp = (cal_temp_0 - ((cal_value_0 - adcSample)  / t_grad));

  /* Round off to integer */
  return (int32_t)(temp + 0.5f);
}

/**********************************************************
 * Initializes ADC for temperature measurement
 **********************************************************/
void adcInit(void) {
  ADC_Init_TypeDef adcInit = ADC_INIT_DEFAULT;

  adcInit.timebase = ADC_TimebaseCalc(0);
  adcInit.prescale = ADC_PrescaleCalc(400000,0);

  ADC_Init(ADC0, &adcInit);

  ADC_InitSingle_TypeDef adcInitSingle = ADC_INITSINGLE_DEFAULT;

  adcInitSingle.acqTime   = adcAcqTime32;
  adcInitSingle.input     = adcSingleInpTemp;
  adcInitSingle.reference = adcRef1V25;

  ADC_InitSingle(ADC0, &adcInitSingle);

  ADC0->IEN = ADC_IEN_SINGLE;
  NVIC_ClearPendingIRQ(ADC0_IRQn);
  NVIC_EnableIRQ(ADC0_IRQn);
}

/**********************************************************
 * Measures the current temperature and sets
 * the temperature factor. The temperature is
 * measured using the internal temperature
 * sensor of the EFM32.
 **********************************************************/
int32_t measureTemperature(void) {
  /* Start ADC conversion and wait until it is complete */
  conversionDone = false;
  ADC_Start(ADC0, adcStartSingle);
  while (!conversionDone) { /* Wait. */ }

  /* Calculate temperature. */
  return convertToCelsius(ADC_DataSingleGet(ADC0));
}

/**********************************************************
 * Interrupt handler for the ADC. Called when the
 * temperature measurement is finished.
 **********************************************************/
void ADC0_IRQHandler(void) {
  uint32_t flags = ADC0->IF;
  ADC0->IFC = flags;
  if (flags & ADC_IF_SINGLE )
  {
    /* Set the flag to tell the main code
     * that measurement is complete. */
    conversionDone = true;
  }
}
