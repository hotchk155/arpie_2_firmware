/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : GPIO1.h
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
** @file GPIO1.h
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

#ifndef __GPIO1_H
#define __GPIO1_H

/* MODULE GPIO1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "IO_Map.h"
#include "GPIO_PDD.h"
#include "PORT_PDD.h"

#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif 


/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define GPIO1_PRPH_BASE_ADDRESS  0x400FF000U
  
/*! Device data structure pointer used when auto initialization property is enabled. This constant can be passed as a first parameter to all component's methods. */
#define GPIO1_DeviceData  ((LDD_TDeviceData *)PE_LDD_GetDeviceStructure(PE_LDD_COMPONENT_GPIO1_ID))

/* Methods configuration constants - generated for all enabled component's methods */
#define GPIO1_Init_METHOD_ENABLED      /*!< Init method of the component GPIO1 is enabled (generated) */
#define GPIO1_SetPortValue_METHOD_ENABLED /*!< SetPortValue method of the component GPIO1 is enabled (generated) */
#define GPIO1_GetPortValue_METHOD_ENABLED /*!< GetPortValue method of the component GPIO1 is enabled (generated) */
#define GPIO1_ClearPortBits_METHOD_ENABLED /*!< ClearPortBits method of the component GPIO1 is enabled (generated) */
#define GPIO1_SetPortBits_METHOD_ENABLED /*!< SetPortBits method of the component GPIO1 is enabled (generated) */
#define GPIO1_TogglePortBits_METHOD_ENABLED /*!< TogglePortBits method of the component GPIO1 is enabled (generated) */
#define GPIO1_SetPortInputDirection_METHOD_ENABLED /*!< SetPortInputDirection method of the component GPIO1 is enabled (generated) */
#define GPIO1_SetPortOutputDirection_METHOD_ENABLED /*!< SetPortOutputDirection method of the component GPIO1 is enabled (generated) */

/* Events configuration constants - generated for all enabled component's events */

/* Definition of bit field constants */
#define BEAT_LED ((LDD_GPIO_TBitField)0)
#define RX_LED ((LDD_GPIO_TBitField)1)
#define TX_LED ((LDD_GPIO_TBitField)2)

/* Definition of implementation constants */
#define GPIO1_ALLOCATED_PINS_MASK 0xC1U /*!< Mask of all allocated pins from the port */
#define GPIO1_MODULE_BASE_ADDRESS GPIOA_BASE_PTR /*!< Name of macro used as the base address */
#define GPIO1_PORTCONTROL_BASE_ADDRESS PORT_BASE_PTR /*!< Name of macro used as the base address */
#define GPIO1_AVAILABLE_EVENTS_MASK 0x00U /*!< Mask of all available events */
#define GPIO1_FIELD_0_PIN_0 LDD_GPIO_PIN_0 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPIO1_FIELD_1_PIN_0 LDD_GPIO_PIN_6 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPIO1_FIELD_2_PIN_0 LDD_GPIO_PIN_7 /*!< Constant for the pin in the field used in the method ConnectPin */
#define GPIO1_BEAT_LED_START_BIT 0u    /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPIO1_BEAT_LED_MASK 0x01u      /*!< Mask of the bits allocated by the bit field (within the port) */
#define GPIO1_RX_LED_START_BIT 6u      /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPIO1_RX_LED_MASK 0x40u        /*!< Mask of the bits allocated by the bit field (within the port) */
#define GPIO1_TX_LED_START_BIT 7u      /*!< Index of the starting bit of the bit field (0 represents LSB) */
#define GPIO1_TX_LED_MASK 0x80u        /*!< Mask of the bits allocated by the bit field (within the port) */
/* Representation of unaligned data value of the port.
   Unsigned integer of proper width depending on the size of the GPIO port allocated.
   Typically the value of n-th bit represents the data for the n-th pin within the port.
   Such value is not abstracted from the physical position of the bits within the port. */
typedef uint32_t GPIO1_TPortValue;

/* Representation of right-aligned data value of the bit field.
   Typically the value of n-th bit represents the data of the n-th bit within the bit field (not within the port).
   There are used only so many lowest bits, as it is denoted by the width of the bit field.
   Such value is abstracted from the physical position of the bit field pins within the port.
   Equals to the type <compId>_TPortValue. */
typedef GPIO1_TPortValue GPIO1_TFieldValue;



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
LDD_TDeviceData* GPIO1_Init(LDD_TUserData *UserDataPtr);

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
void GPIO1_SetPortValue(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Value);

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
GPIO1_TPortValue GPIO1_GetPortValue(LDD_TDeviceData *DeviceDataPtr);

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
void GPIO1_ClearPortBits(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask);

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
void GPIO1_SetPortBits(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask);

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
void GPIO1_TogglePortBits(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask);

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
void GPIO1_SetPortInputDirection(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask);

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
void GPIO1_SetPortOutputDirection(LDD_TDeviceData *DeviceDataPtr, GPIO1_TPortValue Mask, GPIO1_TPortValue Value);

/* END GPIO1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif
/* ifndef __GPIO1_H */
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