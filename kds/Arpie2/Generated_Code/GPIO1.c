/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : GPIO1.c
**     Project     : Arpie2
**     Processor   : MKE02Z64VLC4
**     Component   : GPIO_LDD
**     Version     : Component 01.128, Driver 01.08, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-02-06, 15:31, # CodeGen: 45
**     Abstract    :
**         The HAL GPIO component will provide a low level API for unified
**         access to general purpose digital input/output pins across
**         various device designs.
**
**         RTOS drivers using HAL GPIO API will be simpler and more
**         portable to various microprocessors.
**     Settings    :
**          Component name                                 : GPIO1
**          Port                                           : GPIOA
**          Port width                                     : 32 bits
**          Mask of allocated pins                         : 0xC1
**          Interrupt service/event                        : Disabled
**          Bit fields                                     : 3
**            Bit field                                    : 
**              Field name                                 : BEAT_LED
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : PTA0/KBI0_P0/FTM0_CH0/ACMP0_IN0/ADC0_SE0
**                  Pin signal                             : 
**                  Initial pin direction                  : Input
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : RX_LED
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : PTA6/FTM2_FLT1/ACMP1_IN0/ADC0_SE2
**                  Pin signal                             : 
**                  Initial pin direction                  : Output
**                    Initial output state                 : 0
**                  Initial pin event                      : Disabled
**            Bit field                                    : 
**              Field name                                 : TX_LED
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : PTA7/FTM2_FLT2/ACMP1_IN1/ADC0_SE3
**                  Pin signal                             : 
**                  Initial pin direction                  : Output
**                    Initial output state                 : 0
**                  Initial pin event                      : Disabled
**          Initialization                                 : 
**            Auto initialization                          : yes
**            Event mask                                   : 
**              OnPortEvent                                : Disabled
**     Contents    :
**         Init                   - LDD_TDeviceData* GPIO1_Init(LDD_TUserData *UserDataPtr);
**         SetPortValue           - void GPIO1_SetPortValue(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Value);
**         GetPortValue           - GPIO1_TPortValue GPIO1_GetPortValue(LDD_TDeviceData *DeviceDataPtr);
**         ClearPortBits          - void GPIO1_ClearPortBits(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask);
**         SetPortBits            - void GPIO1_SetPortBits(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask);
**         TogglePortBits         - void GPIO1_TogglePortBits(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue...
**         SetPortInputDirection  - void GPIO1_SetPortInputDirection(LDD_TDeviceData *DeviceDataPtr,...
**         SetPortOutputDirection - void GPIO1_SetPortOutputDirection(LDD_TDeviceData *DeviceDataPtr,...
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file GPIO1.c
** @version 01.08
** @brief
**         The HAL GPIO component will provide a low level API for unified
**         access to general purpose digital input/output pins across
**         various device designs.
**
**         RTOS drivers using HAL GPIO API will be simpler and more
**         portable to various microprocessors.
*/         
/*!
**  @addtogroup GPIO1_module GPIO1 module documentation
**  @{
*/         

/* MODULE GPIO1. */

#include "GPIO1.h"
/* {Default RTOS Adapter} No RTOS includes */
#include "IO_Map.h"

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct {
  LDD_TUserData *UserData;             /* RTOS device data structure */
} GPIO1_TDeviceData, *GPIO1_TDeviceDataPtr; /* Device data structure type */
/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static GPIO1_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;
/*
** ===================================================================
**     Method      :  GPIO1_Init (component GPIO_LDD)
*/
/*!
**     @brief
**         This method initializes the associated peripheral(s) and the
**         component internal variables. The method is called
**         automatically as a part of the application initialization
**         code.
**     @param
**         UserDataPtr     - Pointer to the RTOS device
**                           structure. This pointer will be passed to
**                           all events as parameter.
**     @return
**                         - Pointer to the dynamically allocated private
**                           structure or NULL if there was an error.
*/
/* ===================================================================*/
LDD_TDeviceData* GPIO1_Init(LDD_TUserData *UserDataPtr)
{
  /* Allocate LDD device structure */
  GPIO1_TDeviceData *DeviceDataPrv;

  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  /* Save RTOS Device structure */
  DeviceDataPrv->UserData = UserDataPtr; /* Store the RTOS device structure */
  /* GPIOA_PDOR: PDO&=~0xC0 */
  GPIOA_PDOR &= (uint32_t)~(uint32_t)(GPIO_PDOR_PDO(0xC0));
  /* GPIOA_PIDR: PID&=~1 */
  GPIOA_PIDR &= (uint32_t)~(uint32_t)(GPIO_PIDR_PID(0x01));
  /* GPIOA_PDDR: PDD&=~1,PDD|=0xC0 */
  GPIOA_PDDR = (uint32_t)((GPIOA_PDDR & (uint32_t)~(uint32_t)(
                GPIO_PDDR_PDD(0x01)
               )) | (uint32_t)(
                GPIO_PDDR_PDD(0xC0)
               ));
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_GPIO1_ID,DeviceDataPrv);
  return ((LDD_TDeviceData *)DeviceDataPrv);
}

/*
** ===================================================================
**     Method      :  GPIO1_SetPortValue (component GPIO_LDD)
*/
/*!
**     @brief
**         This method writes the output data value to the port. Only
**         configured pins (through all of bit fields in this component)
**         are affected.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Value           - Unaligned data value to write to the
**                           port. The bit 0 corresponds with the pin
**                           which has index 0 within the port, the bit
**                           1 corresponds with the pin which has index
**                           1 within the port, etc. Only configured
**                           pins (through all of bit fields in this
**                           component) are affected.
*/
/* ===================================================================*/
void GPIO1_SetPortValue(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Value)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  GPIO_PDD_SetPortDataOutput(GPIO1_MODULE_BASE_ADDRESS,
    (GPIO_PDD_GetPortDataOutput(GPIO1_MODULE_BASE_ADDRESS)
      & ((GPIO1_TPortValue)(~((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK))))
    | (((GPIO1_TPortValue)(Value)) & ((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK)));
}

/*
** ===================================================================
**     Method      :  GPIO1_GetPortValue (component GPIO_LDD)
*/
/*!
**     @brief
**         This method returns the current port input data value. Only
**         configured pins (through all of bit fields in this component)
**         are returned.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Unaligned current port input data value
**                           masked for allocated pins of the port. The
**                           bit 0 corresponds with the pin which has
**                           index 0 within the port, the bit 1
**                           corresponds with the pin which has index 1
**                           within the port, etc. Only configured pins
**                           (through all of bit fields in this
**                           component) are returned. The others are
**                           zeros.
*/
/* ===================================================================*/
GPIO1_TPortValue GPIO1_GetPortValue(LDD_TDeviceData *DeviceDataPtr)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  return (GPIO1_TPortValue)(GPIO_PDD_GetPortDataInput(GPIO1_MODULE_BASE_ADDRESS)
    & ((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK));
}

/*
** ===================================================================
**     Method      :  GPIO1_ClearPortBits (component GPIO_LDD)
*/
/*!
**     @brief
**         This method drives the specified bits of the port to the
**         inactive level. Only configured pins (through all of bit
**         fields in this component) are affected.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Mask            - Unaligned mask of bits to setting the
**                           inactive level. Each port pin has
**                           corresponding bit in the mask. Bit value 0
**                           means not selected bit, bit value 1 means
**                           selected bit. The bit 0 corresponds with
**                           the pin which has index 0 within the port,
**                           the bit 1 corresponds with the pin which
**                           has index 1 within the port, etc. Only
**                           configured pins (through all of bit fields
**                           in this component) are affected.
*/
/* ===================================================================*/
void GPIO1_ClearPortBits(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  GPIO_PDD_ClearPortDataOutputMask(GPIO1_MODULE_BASE_ADDRESS,
    ((GPIO1_TPortValue)(((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK) & ((GPIO1_TPortValue)(Mask)))));
}

/*
** ===================================================================
**     Method      :  GPIO1_SetPortBits (component GPIO_LDD)
*/
/*!
**     @brief
**         This method drives the specified bits of the port to the
**         active level. Only configured pins (through all of bit
**         fields in this component) are affected.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Mask            - Unaligned mask of bits to setting the
**                           active level. Each port pin has
**                           corresponding bit in the mask. Bit value 0
**                           means not selected bit, bit value 1 means
**                           selected bit. The bit 0 corresponds with
**                           the pin which has index 0 within the port,
**                           the bit 1 corresponds with the pin which
**                           has index 1 within the port, etc. Only
**                           configured pins (through all of bit fields
**                           in this component) are affected.
*/
/* ===================================================================*/
void GPIO1_SetPortBits(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  GPIO_PDD_SetPortDataOutputMask(GPIO1_MODULE_BASE_ADDRESS,
    ((GPIO1_TPortValue)(((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK) & ((GPIO1_TPortValue)(Mask)))));
}

/*
** ===================================================================
**     Method      :  GPIO1_TogglePortBits (component GPIO_LDD)
*/
/*!
**     @brief
**         This method inverts the specified bits of the port to
**         another level. Only configured pins (through all of bit
**         fields in this component) are affected.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Mask            - Unaligned mask of bits to inverting the
**                           current level. Each port pin has
**                           corresponding bit in the mask. Bit value 0
**                           means not selected bit, bit value 1 means
**                           selected bit. The bit 0 corresponds with
**                           the pin which has index 0 within the port,
**                           the bit 1 corresponds with the pin which
**                           has index 1 within the port, etc. Only
**                           configured pins (through all of bit fields
**                           in this component) are affected.
*/
/* ===================================================================*/
void GPIO1_TogglePortBits(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  GPIO_PDD_TogglePortDataOutputMask(GPIO1_MODULE_BASE_ADDRESS,
    ((GPIO1_TPortValue)(((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK) & ((GPIO1_TPortValue)(Mask)))));
}

/*
** ===================================================================
**     Method      :  GPIO1_SetPortInputDirection (component GPIO_LDD)
*/
/*!
**     @brief
**         This method sets the specified pins of the port to the input
**         direction. Only configured pins (through all of bit fields
**         in this component) are affected.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Mask            - Mask of bits to setting the input
**                           direction. Each port pin has corresponding
**                           bit in the mask. Bit value 0 means not
**                           selected bit, bit value 1 means selected
**                           bit. The bit 0 corresponds with the pin
**                           which has index 0 within the port, the bit
**                           1 corresponds with the pin which has index
**                           1 within the port, etc. Only configured
**                           pins (through all of bit fields in this
**                           component) are affected.
*/
/* ===================================================================*/
void GPIO1_SetPortInputDirection(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  GPIO_PDD_SetPortInputDirectionMask(GPIO1_MODULE_BASE_ADDRESS,
    ((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK) & ((GPIO1_TPortValue)(Mask)));
}

/*
** ===================================================================
**     Method      :  GPIO1_SetPortOutputDirection (component GPIO_LDD)
*/
/*!
**     @brief
**         This method sets the specified pins of the port to the
**         output direction. The specified pins of the port will be
**         driven to specified states. Only configured pins (through
**         all of bit fields in this component) are affected.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Mask            - Mask of bits to setting the output
**                           direction. Each port pin has corresponding
**                           bit in the mask. Bit value 0 means not
**                           selected bit, bit value 1 means selected
**                           bit. The bit 0 corresponds with the pin
**                           which has index 0 within the port, the bit
**                           1 corresponds with the pin which has index
**                           1 within the port, etc. Only configured
**                           pins (through all of bit fields in this
**                           component) are affected.
**     @param
**         Value           - Unaligned port data value to appear on
**                           the specified port pins after they have
**                           been switched to the output direction.
*/
/* ===================================================================*/
void GPIO1_SetPortOutputDirection(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask, GPIO1_TPortValue Value)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  GPIO_PDD_SetPortDataOutput(GPIO1_MODULE_BASE_ADDRESS,
    ((GPIO1_TPortValue)(GPIO_PDD_GetPortDataOutput(GPIO1_MODULE_BASE_ADDRESS)
      & ((GPIO1_TPortValue)(~((GPIO1_TPortValue)(((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK)
        & ((GPIO1_TPortValue)(Mask))))))))
    | (((GPIO1_TPortValue)(Value))
      & ((GPIO1_TPortValue)(((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK) & ((GPIO1_TPortValue)(Mask))))));
  GPIO_PDD_SetPortOutputDirectionMask(GPIO1_MODULE_BASE_ADDRESS,
    ((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK) & ((GPIO1_TPortValue)(Mask)));
}

/* END GPIO1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/