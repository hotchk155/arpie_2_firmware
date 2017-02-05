/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : FLASH1.c
**     Project     : Arpie2
**     Processor   : MKE02Z64VLC4
**     Component   : FLASH_LDD
**     Version     : Component 01.106, Driver 01.01, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-02-03, 15:03, # CodeGen: 32
**     Abstract    :
**          This embedded component implements an access to an on-chip flash memory.
**          Using this component the flash memory could be written to, erased,
**          read from. Some other services would be provided, if they are supported
**          by hardware, e.g. changes of the flash memory protection state.
**     Settings    :
**          Component name                                 : FLASH1
**          Device                                         : FTMRH
**          Clock frequency                                : 1 MHz
**          Use user memory areas                          : no
**          Interrupt service                              : Disabled
**            Write batch size                             : Minimal
**            Erase batch size                             : Minimal
**            Read batch size                              : Unlimited
**          Safe launch and wait                           : yes
**            Safe routine location                        : Defined statically by the component
**            Interruptable wait loop                      : no
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnOperationComplete                        : Enabled
**              OnError                                    : Disabled
**            CPU clock/configuration selection            : 
**              Clock configuration 0                      : This component enabled
**              Clock configuration 1                      : This component disabled
**              Clock configuration 2                      : This component disabled
**              Clock configuration 3                      : This component disabled
**              Clock configuration 4                      : This component disabled
**              Clock configuration 5                      : This component disabled
**              Clock configuration 6                      : This component disabled
**              Clock configuration 7                      : This component disabled
**     Contents    :
**         Init  - LDD_TDeviceData* FLASH1_Init(LDD_TUserData *UserDataPtr);
**         Read  - LDD_TError FLASH1_Read(LDD_TDeviceData *DeviceDataPtr, LDD_FLASH_TAddress...
**         Write - LDD_TError FLASH1_Write(LDD_TDeviceData *DeviceDataPtr, LDD_TData *FromPtr,...
**         Erase - LDD_TError FLASH1_Erase(LDD_TDeviceData *DeviceDataPtr, LDD_FLASH_TAddress...
**         Main  - void FLASH1_Main(LDD_TDeviceData *DeviceDataPtr);
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
** @file FLASH1.c
** @version 01.01
** @brief
**          This embedded component implements an access to an on-chip flash memory.
**          Using this component the flash memory could be written to, erased,
**          read from. Some other services would be provided, if they are supported
**          by hardware, e.g. changes of the flash memory protection state.
*/         
/*!
**  @addtogroup FLASH1_module FLASH1 module documentation
**  @{
*/         

/* MODULE FLASH1. */

#include "Events.h"
#include "FLASH1.h"
/* {Default RTOS Adapter} No RTOS includes */
#include "SIM_PDD.h"
#include "FTMRH_PDD.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SAFE_ROUTINE_SIZE 28U

/*
Safe launch and wait routine code. This code launch a command and
waits until command completion or command violation. Routine is
written as array of bytes to be constant size and compiler independent.
C language implementation could look like:

void SafeRoutine(void) {
  FTMRH_PDD_LaunchCommand(FTMRH_BASE_PTR);
  while (!FTMRH_PDD_GetCommandCompleteInterruptFlag(FTMRH_BASE_PTR)) {};
}
*/

typedef void (* LDD_FLASH_TSafeRoutinePtr)(void); /* Safe routine pointer type */

typedef struct {
  uint8_t SafeRoutine[SAFE_ROUTINE_SIZE]; /* Safe routine buffer */
} LDD_FLASH_TSafeRoutine;              /* Safe routine buffer type */

static const uint8_t SafeRoutine[SAFE_ROUTINE_SIZE - 4U] = {
  0x00U,0xb5U,               /*  push	{lr}                                         */
  0x04U,0x4aU,               /*  ldr	r2, [pc, #16]	; (bc <SafeRoutine___+0x14>) */
  0x80U,0x23U,               /*  movs	r3, #128	; 0x80                       */
  0x13U,0x70U,               /*  strb	r3, [r2, #0]                                 */
  0x11U,0x78U,               /*  ldrb	r1, [r2, #0]                                 */
  0x48U,0xb2U,               /*  sxtb	r0, r1                                       */
  0x00U,0x28U,               /*  cmp	r0, #0                                       */
  0xfbU,0xdaU,               /*  bge.n	b0 <SafeRoutine___+0x8>                      */
  0x00U,0xbdU,               /*  pop	{pc}                                         */
  0xc0U,0x46U,               /*  nop			; (mov r8, r8)               */
  (uint8_t)((0x40020006U>>0)&0xFFU),  /* FTMRH_FSTAT register address                 */
  (uint8_t)((0x40020006U>>8)&0xFFU),
  (uint8_t)((0x40020006U>>16)&0xFFU),
  (uint8_t)((0x40020006U>>24)&0xFFU)
};

typedef struct {
  LDD_FLASH_TSafeRoutine     SafeRoutine;        /* Safe routine buffer */
  LDD_FLASH_TSafeRoutinePtr  SafeRoutinePtr;     /* Safe routine pointer */
  LDD_FLASH_TOperationType   CurrentOperation;   /* Current operation type */
  LDD_FLASH_TOperationStatus CurrentOperationStatus; /* Current operation status */
  LDD_FLASH_TAddress         CurrentFlashAddress; /* Address of destination/source in the flash memory for the operation step */
  LDD_FLASH_TAddress         CurrentFlashAddressIncrement; /* Address increment for next step */
  uint32_t                   CurrentUserAddress; /* Address of destination/source in the user memory for the operation step */
  LDD_FLASH_TDataSize        CurrentDataSize;    /* Data size of the current operation step */
  LDD_FLASH_TErrorFlags      CurrentErrorFlags;  /* Current error flags */
  LDD_TUserData             *UserDataPtr;        /* Device mode user device data structure */
} FLASH1_TDeviceData, *FLASH1_TDeviceDataPtr; /* Device data structure type */

/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static FLASH1_TDeviceData DevDataPtr__DEFAULT_RTOS_ALLOC;

/* Internal method prototypes */
void FLASH1_MainEngine(LDD_TDeviceData *DeviceDataPtr);
static LDD_TError CheckStateAndRange(FLASH1_TDeviceDataPtr DevDataPtr, LDD_FLASH_TAddress Address, LDD_FLASH_TDataSize Size);

/*
** ===================================================================
**     Method      :  CheckStateAndRange (component FLASH_LDD)
**
**     Description :
**         Range check internal routine. This routine is used by the 
**         component's method to test the input address range.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static LDD_TError CheckStateAndRange(FLASH1_TDeviceDataPtr DevDataPtr, LDD_FLASH_TAddress Address, LDD_FLASH_TDataSize Size)
{
  if (DevDataPtr->SafeRoutinePtr == NULL) { /* Has the safe routine pointer been initialized? */
    return ERR_NOTAVAIL;               /* If no, returns error */
  }
  /* Input parameter test - this test can be disabled by setting the "Ignore range checking"
     property to the "yes" value in the "Configuration inspector" */
  if (Address >= (FLASH1_BLOCK0_ADDRESS + FLASH1_BLOCK0_SIZE)) {
    if (!((Address >= FLASH1_BLOCK1_ADDRESS) && ((Address < (FLASH1_BLOCK1_ADDRESS + FLASH1_BLOCK1_SIZE))))) {
      return ERR_PARAM_ADDRESS;
    }
  }
  Address += Size;
  if (Address > (FLASH1_BLOCK0_ADDRESS + FLASH1_BLOCK0_SIZE)) {
    if (!((Address >= FLASH1_BLOCK1_ADDRESS) && ((Address <= (FLASH1_BLOCK1_ADDRESS + FLASH1_BLOCK1_SIZE))))) {
      return ERR_PARAM_SIZE;
    }
  }
  if (DevDataPtr->CurrentOperationStatus > LDD_FLASH_IDLE) {
    return ERR_BUSY;
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  FLASH1_Init (component FLASH_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc. If the "Enable
**         in init. code" property is set to "yes" ([Enabled in init.
**         code]) value then the device is also enabled(see the
**         description of the Enable() method). In this case the
**         Enable() method is not necessary and needn't to be generated.
**         This method can be called only once. Before the second call
**         of Init() the Deinit() must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* FLASH1_Init(LDD_TUserData *UserDataPtr)
{
  FLASH1_TDeviceData *DevDataPtr;

  /* Allocate FLASH_LDD device structure */
  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DevDataPtr = &DevDataPtr__DEFAULT_RTOS_ALLOC;
  DevDataPtr->UserDataPtr = UserDataPtr; /* Store the user data */
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_FLASH1_ID,DevDataPtr);
  DevDataPtr->CurrentOperationStatus = LDD_FLASH_IDLE; /* Initialization of the Current operation status variable */
  DevDataPtr->CurrentOperation = LDD_FLASH_NO_OPERATION; /* Initialization of the Current operation type item */
  DevDataPtr->CurrentErrorFlags = 0U;  /* Initialization of the Current error flags item */
  if ((((uint32_t)&DevDataPtr->SafeRoutine) & 3U) != 0U) { /* Is the destination address aligned to 32bit? */
    DevDataPtr->SafeRoutinePtr = (LDD_FLASH_TSafeRoutinePtr)((((uint32_t)&DevDataPtr->SafeRoutine) + 3U) & ~(uint32_t)3); /* If the destination address is not aligned to 4 bytes, and save the destination address into the internal data structure */
  } else {
    DevDataPtr->SafeRoutinePtr = (LDD_FLASH_TSafeRoutinePtr)(void *)&DevDataPtr->SafeRoutine; /* If the destination address is aligned to 4 bytes, save the destination address into the internal data structure */
  }
  *(LDD_FLASH_TSafeRoutine *)(void *)DevDataPtr->SafeRoutinePtr = *(LDD_FLASH_TSafeRoutine *)(void *)&SafeRoutine; /* Copy the safe routine's code to a buffer in the internal data structure to be the code of the routine be runned from */
  #if defined(SIM_PDD_CLOCK_GATE_FTMRH)
  SIM_PDD_SetClockGate(SIM_BASE_PTR, SIM_PDD_CLOCK_GATE_FTMRH, PDD_ENABLE);
  #endif
  /* FTMRH_FCLKDIV: FDIVLD=0,FDIVLCK=0,FDIV=0x0F */
  FTMRH_FCLKDIV = FTMRH_FCLKDIV_FDIV(0x0F); /* Set clock divider */
  return DevDataPtr;
}

/*
** ===================================================================
**     Method      :  FLASH1_Read (component FLASH_LDD)
*/
/*!
**     @brief
**         This method sets up a flash memory read operation. The
**         operation itself is performing by defined batches (property
**         [Read batch size]) by periodical calling the component�s
**         Main method in the user application (higher level OS service).
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         FromAddress     - Address to the flash
**                           memory the output data would be read from.
**     @param
**         ToPtr           - Pointer to a location the data would
**                           be written to.
**     @param
**         Size            - Size of the data (in bytes)  to be read
**                           from the flash memory.
**     @return
**                         - Error code
**                           ERR_OK - OK
**                           ERR_DISABLED - Component is disabled
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_BUSY - Some flash memory operation is
**                           already in progress 
**                           ERR_PARAM_ADDRESS - Desired flash memory
**                           area is out of allowed range
*/
/* ===================================================================*/
LDD_TError FLASH1_Read(LDD_TDeviceData *DeviceDataPtr, LDD_FLASH_TAddress FromAddress, LDD_TData *ToPtr, LDD_FLASH_TDataSize Size)
{
  FLASH1_TDeviceDataPtr DevDataPtr = (FLASH1_TDeviceDataPtr)DeviceDataPtr;
  LDD_TError Result;
  
  Result = CheckStateAndRange(DevDataPtr, FromAddress, Size);
  if (Result != ERR_OK) {
    return Result;
  }
  DevDataPtr->CurrentOperation = LDD_FLASH_READ;
  DevDataPtr->CurrentOperationStatus = LDD_FLASH_START;
  DevDataPtr->CurrentFlashAddress = FromAddress;
  DevDataPtr->CurrentUserAddress = (uint32_t)ToPtr;
  DevDataPtr->CurrentDataSize = Size;
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  FLASH1_Write (component FLASH_LDD)
*/
/*!
**     @brief
**         This method sets up a flash memory write operation. The
**         operation itself is performing by defined batches (property
**         [Write batch size]) by periodical calling the component�s
**         Main method in the user application (higher level OS service)
**         or by the component�s ISR, if an component�s interrupt
**         service is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         FromPtr         - Pointer to data to be written to
**                           the flash memory.
**     @param
**         ToAddress       - Address to the flash memory
**                           the input data would be written to.
**     @param
**         Size            - Size of the input data (in bytes) to be
**                           written to the flash memory.
**     @return
**                         - Error code
**                           ERR_OK - OK
**                           ERR_DISABLED - Component is disabled
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_BUSY - Some flash memory operation is
**                           already in progress 
**                           ERR_PARAM_ADDRESS - Desired flash memory
**                           area is out of allowed range
**                           ERR_NOTAVAIL - When Safe launch and wait
**                           mode is enabled (property Safe launch and
**                           wait) and safe routine location is defined
**                           in runtime (property Safe routine location)
**                           and the safe routine location has not been
**                           specified yet (the SetSafeRoutineLocation
**                           method has not been used to define the
**                           location the safe routine will be copied to).
*/
/* ===================================================================*/
LDD_TError FLASH1_Write(LDD_TDeviceData *DeviceDataPtr, LDD_TData *FromPtr, LDD_FLASH_TAddress ToAddress, LDD_FLASH_TDataSize Size)
{
  FLASH1_TDeviceDataPtr DevDataPtr = (FLASH1_TDeviceDataPtr)DeviceDataPtr;
  LDD_TError Result;
  
  Result = CheckStateAndRange(DevDataPtr, ToAddress, Size);
  if (Result != ERR_OK) {
    return Result;
  }
  if (ToAddress >= FLASH1_EEPROM_ADDRESS) {
    /* EEPROM memory is addressed */
    FTMRH_PDD_SetCommand(FTMRH_BASE_PTR, FTMRH_PDD_CMD_PROGRAM_EEPROM);
  } else {
    /* FLASH memory is addressed */
    FTMRH_PDD_SetCommand(FTMRH_BASE_PTR, FTMRH_PDD_CMD_PROGRAM_FLASH);
  }
  DevDataPtr->CurrentOperation = LDD_FLASH_WRITE;
  DevDataPtr->CurrentOperationStatus = LDD_FLASH_START;
  DevDataPtr->CurrentUserAddress = (uint32_t)FromPtr;
  DevDataPtr->CurrentFlashAddress = ToAddress;
  DevDataPtr->CurrentDataSize = Size;
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  FLASH1_Erase (component FLASH_LDD)
*/
/*!
**     @brief
**         This method sets up a flash memory erase operation. The
**         operation itself is performing by defined batches (property
**         [Erase batch size]) by periodical calling the component�s
**         Main method in the user application (higher level OS service)
**         or by the component�s ISR, if an component�s interrupt
**         service is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         FromAddress     - Address of the flash
**                           memory area (the first erase sector is the
**                           sector the given address belongs to) to be
**                           erased.
**     @param
**         Size            - Size of the flash memory area (in bytes)
**                           to be erased. The flash memory is erased by
**                           the erase sectors. The first erased sector
**                           is a sector the address specified by the
**                           input parameter Address belongs to. The
**                           last erased sector is a sector the address
**                           calculated like an addition of the address
**                           specified by the input parameter Address
**                           and the size specified by the input
**                           parameter Size belongs to.
**     @return
**                         - Error code
**                           ERR_OK - OK
**                           ERR_DISABLED - Component is disabled
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_BUSY - Some flash memory operation is
**                           already in progress 
**                           ERR_PARAM_ADDRESS - Desired flash memory
**                           area is out of allowed range or is not
**                           aligned to erasable units' bounderies
**                           ERR_NOTAVAIL - When Safe launch and wait
**                           mode is enabled (property Safe launch and
**                           wait) and safe routine location is defined
**                           in runtime (property Safe routine location)
**                           and the safe routine location has not been
**                           specified yet (the SetSafeRoutineLocation
**                           method has not been used to define the
**                           location the safe routine will be copied to).
*/
/* ===================================================================*/
LDD_TError FLASH1_Erase(LDD_TDeviceData *DeviceDataPtr, LDD_FLASH_TAddress FromAddress, LDD_FLASH_TDataSize Size)
{
  FLASH1_TDeviceDataPtr DevDataPtr = (FLASH1_TDeviceDataPtr)DeviceDataPtr;
  LDD_TError Result;
  
  Result = CheckStateAndRange(DevDataPtr, FromAddress, Size);
  if (Result != ERR_OK) {
    return Result;
  }
  DevDataPtr->CurrentOperation = LDD_FLASH_ERASE;
  DevDataPtr->CurrentOperationStatus = LDD_FLASH_START;
  DevDataPtr->CurrentDataSize = Size;
  if (FromAddress >= FLASH1_EEPROM_ADDRESS) {
    /* EEPROM memory is addressed */
    DevDataPtr->CurrentFlashAddress = FromAddress;
    DevDataPtr->CurrentFlashAddressIncrement = FLASH1_EEPROM_ERASABLE_UNIT_SIZE;
    FTMRH_PDD_SetCommand(FTMRH_BASE_PTR, FTMRH_PDD_CMD_ERASE_EEPROM_SECTOR);
  } else {
    /* FLASH memory is addressed */
    /* FTMRH_PDD_CMD_ERASE_FLASH_SECTOR requires sector address to be 4 bytes aligned  */
    DevDataPtr->CurrentFlashAddress = FromAddress & ~(uint32_t)FLASH1_FLASH_WRITABLE_UNIT_MASK;
    DevDataPtr->CurrentFlashAddressIncrement = FLASH1_FLASH_ERASABLE_UNIT_SIZE;
    FTMRH_PDD_SetCommand(FTMRH_BASE_PTR, FTMRH_PDD_CMD_ERASE_FLASH_SECTOR);
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  FLASH1_MainEngine (component FLASH_LDD)
**
**     Description :
**         Main programming routine.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void FLASH1_MainEngine(LDD_TDeviceData *DeviceDataPtr)
{
  FLASH1_TDeviceDataPtr DevDataPtr = (FLASH1_TDeviceDataPtr)DeviceDataPtr;

  uint32_t j;
  uint32_t AlignedFlashAddress;
  uint8_t WrittenData[4];
  uint8_t CurrentFlags;
  
  if (DevDataPtr->CurrentOperationStatus == LDD_FLASH_START) {
    /* First run of current operation */
    FTMRH_PDD_ClearFlags(FTMRH_BASE_PTR, FTMRH_PDD_ACCESS_ERROR_FLAG | FTMRH_PDD_PROTECTION_VIOLATION_FLAG);
    CurrentFlags = 0U;
    DevDataPtr->CurrentErrorFlags = 0U;
    DevDataPtr->CurrentOperationStatus = LDD_FLASH_RUNNING;
  } else if(DevDataPtr->CurrentOperation == LDD_FLASH_READ) {
    /* For Read operation HW flags has no meaning */
    CurrentFlags = 0U;
  } else {
    /* After HW command is done HW flags must be checked */
    CurrentFlags = DevDataPtr->CurrentErrorFlags | FTMRH_PDD_GetFlags(FTMRH_BASE_PTR);
  }
  if (CurrentFlags != 0U) {
    /* Command error detected */
    DevDataPtr->CurrentOperationStatus = LDD_FLASH_FAILED;
    FTMRH_PDD_DisableCommandCompleteInterrupt(FTMRH_BASE_PTR); /* Disable the Command complete interrupt */
    return;
  } else if ((int32_t)DevDataPtr->CurrentDataSize <= 0) {
    /* Command done */
    DevDataPtr->CurrentOperationStatus = LDD_FLASH_IDLE;
    FTMRH_PDD_DisableCommandCompleteInterrupt(FTMRH_BASE_PTR); /* Disable the Command complete interrupt */
    FLASH1_OnOperationComplete(DevDataPtr->UserDataPtr);
  } else {
    /* Command in progress */
    switch (DevDataPtr->CurrentOperation) {
      case LDD_FLASH_WRITE:
        if (DevDataPtr->CurrentFlashAddress >= FLASH1_EEPROM_ADDRESS) {
          /* EEPROM memory is addressed */
          FTMRH_PDD_SetAddress(FTMRH_BASE_PTR, DevDataPtr->CurrentFlashAddress);
          if ((*(uint8_t*)DevDataPtr->CurrentFlashAddress ^ *(uint8_t*)DevDataPtr->CurrentUserAddress) & *(uint8_t*)DevDataPtr->CurrentUserAddress) {
            /* Only erase operation can change bits from 0 to 1 value */
            DevDataPtr->CurrentErrorFlags |= LDD_FLASH_MULTIPLE_WRITE_ERROR;
            return;
          }
          FTMRH_PDD_SetProgrammedByte(FTMRH_BASE_PTR, 2, *(uint8_t*)DevDataPtr->CurrentUserAddress++);
          DevDataPtr->CurrentFlashAddress++;
          DevDataPtr->CurrentDataSize--;
        } else {
          /* FLASH memory is addressed */
          FTMRH_PDD_SetAddress(FTMRH_BASE_PTR, DevDataPtr->CurrentFlashAddress & ~(uint32_t)FLASH1_FLASH_WRITABLE_UNIT_MASK);
          AlignedFlashAddress = DevDataPtr->CurrentFlashAddress & ~(uint32_t)FLASH1_FLASH_WRITABLE_UNIT_MASK;
          for (j=0x00U; j<=0x03U; j++) {
            if ((AlignedFlashAddress < DevDataPtr->CurrentFlashAddress) || (DevDataPtr->CurrentDataSize == 0x00U)) {
              WrittenData[j] = *(uint8_t*)AlignedFlashAddress++;
            } else {
              WrittenData[j] = *(uint8_t*)DevDataPtr->CurrentUserAddress++;
              DevDataPtr->CurrentDataSize--;
            }
          }
          AlignedFlashAddress = DevDataPtr->CurrentFlashAddress & ~(uint32_t)FLASH1_FLASH_WRITABLE_UNIT_MASK;
          if ((*(uint32_t*)AlignedFlashAddress ^ *(uint32_t*)(void*)WrittenData) & *(uint32_t*)(void*)WrittenData) {
            /* Only erase operation can change bits from 0 to 1 value */
            DevDataPtr->CurrentErrorFlags |= LDD_FLASH_MULTIPLE_WRITE_ERROR;
            return;
          }
          DevDataPtr->CurrentFlashAddress = AlignedFlashAddress + 4U;
          FTMRH_PDD_SetProgrammedWord(FTMRH_BASE_PTR, 2, WrittenData[1], WrittenData[0]);
          FTMRH_PDD_SetProgrammedWord(FTMRH_BASE_PTR, 3, WrittenData[3], WrittenData[2]);
        }
        break;
      case LDD_FLASH_ERASE:
        FTMRH_PDD_SetAddress(FTMRH_BASE_PTR, DevDataPtr->CurrentFlashAddress);
        DevDataPtr->CurrentFlashAddress += DevDataPtr->CurrentFlashAddressIncrement;
        DevDataPtr->CurrentDataSize -= DevDataPtr->CurrentFlashAddressIncrement;
        break;
      case LDD_FLASH_READ:
        while (DevDataPtr->CurrentDataSize) {
          *(uint8_t*)DevDataPtr->CurrentUserAddress++ = *(uint8_t*)DevDataPtr->CurrentFlashAddress++;
          DevDataPtr->CurrentDataSize--;
        }
        return;
      default:
        DevDataPtr->CurrentDataSize = 0U;
        break;
    }
    ((LDD_FLASH_TSafeRoutinePtr)((uint32_t)(DevDataPtr->SafeRoutinePtr) | 1U))(); /* Run the Safe routine */
  }
}
/*
** ===================================================================
**     Method      :  FLASH1_Main (component FLASH_LDD)
*/
/*!
**     @brief
**         This method is used to perform one batch of a flash memory
**         operation. This method is used to perform batches of all
**         flash memory operations (Write, Read, Erase, EraseBlock,
**         VerifyErasedBlock) when the component works in the polled
**         mode (interrupt service is disabled - property [Interrupt
**         service]). This method performs batches of the read flash
**         memory operation, when the component works in the interrupt
**         triggered mode (interrupt service is enabled). This method
**         is enabled only if the component works in the polled mode or
**         if the Read method is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
*/
/* ===================================================================*/
void FLASH1_Main(LDD_TDeviceData *DeviceDataPtr)
{
  FLASH1_TDeviceDataPtr DevDataPtr = (FLASH1_TDeviceDataPtr)DeviceDataPtr;

  /* Minimal batch size, call execution loop only once */
  if (DevDataPtr->CurrentOperationStatus > LDD_FLASH_IDLE) {
    FLASH1_MainEngine(DevDataPtr);
  }
}

/* END FLASH1. */

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