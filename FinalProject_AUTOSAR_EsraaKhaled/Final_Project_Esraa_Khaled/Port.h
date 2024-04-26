/*
 * Module: Port
 * File Name: Port.h
 * Description: Header File for TM4C123GH6PM Microcontroller - Port Driver
 * Author: Esraa Khaled
 */

#ifndef PORT_H_
#define PORT_H_

/*ID for the Company in AUTOSAR
 * Ex: ID = 999*/
#define PORT_VENDOR_ID                      (999U)

/*PORT Module ID*/
#define PORT_MODULE_ID                      (124U)

/*PORT Instance ID*/
#define PORT_INSTANCE_ID                    (0U)


/*Macros for PORT Status*/
#define PORT_INITIALIZED                    (1U)
#define PORT_NOT_INITIALIZED                (0U)

/* Module Version 1.0.0 */
#define PORT_SW_MAJOR_VERSION               (1U)
#define PORT_SW_MINOR_VERSION               (0U)
#define PORT_SW_PATCH_VERSION               (0U)


/* AUTOSAR Version 4.0.3*/
#define PORT_AR_RELEASE_MAJOR_VERSION       (4U)
#define PORT_AR_RELEASE_MINOR_VERSION       (0U)
#define PORT_AR_RELEASE_PATCH_VERSION       (3U)

/*Including Standard AUTOSAR Types*/
#include "Std_Types.h"

/*AUTOSAR version Checking between Std Types and PORT Modules*/
#if   ((PORT_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
    || (PORT_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
    || (PORT_AR_RELEASE_PATCH_VERSION != STD_TYPES_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Std_Types.h does not match the expected version"
#endif

/*Including PORT Pre-Compile Header Filer*/
#include "Port_Cfg.h"

/*AUTOSAR version Checking between PORT_Cfg.h and PORT.h Files*/
#if   ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
    || (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
    || (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of PORT_Cfg.h does not match the expected version"
#endif

/*Software version Checking between PORT_Cfg.h and PORT.h Files*/
#if   ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
    || (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
    || (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
#error "The AR version of PORT_Cfg.h does not match the expected version"
#endif

/*Including Non AUTOSAR Files*/
#include "common_macros.h"


                            /*API Service ID Macros*/

/*Service ID for Port Init */
#define PORT_INIT_SID                             (uint8)0x00

/*Service ID for Port Set Pin Direction */
#define PORT_SET_PIN_DIRECTION_SID                (uint8)0x01

/*Service ID for Port Refresh Port Direction*/
#define PORT_REFRESH_PORT_DIRECTION_SID           (uint8)0x02

/*Service ID for Port get version Info */
#define PORT_GET_VERSION_INFO_SID                 (uint8)0x03

/*Service ID for Port Set Pin Mode */
#define PORT_SET_PIN_MODE_SID                     (uint8)0x04


                            /*DET Error Code*/

/*Invalid Port Pin ID requested*/
#define PORT_E_PARAM_PIN                          (uint8)0x0A

/*Port Pin not configured as changeable*/
#define PORT_E_DIRECTION_UNCHANGEABLE             (uint8)0x0B

/*API Port_Init service called with wrong parameter.*/
#define PORT_E_PARAM_CONFIG                       (uint8)0x0C

/*API Port_SetPinMode service called when mode is unchangeable.*/
#define PORT_E_PARAM_INVALID_MODE                 (uint8)0x0D
#define PORT_E_MODE_UNCHANGEABLE                  (uint8)0x0E

/*API service called without module initialization*/
#define PORT_E_UNINIT                             (uint8)0x0F

/*APIs called with a Null Pointer*/
#define PORT_E_PARAM_POINTER                      (uint8)0x10



                            /*Module Data Types*/

/*Pin Direction Changeable or not*/
#define PORT_PIN_DIRECTION_CHANGEABLE             (STD_ON)
#define PORT_PIN_DIRECTION_NOT_CHANGEABLE         (STD_OFF)

/*Pin Mode Changeable or not*/
#define PORT_PIN_MODE_CHANGEABLE                  (STD_ON)
#define PORT_PIN_MODE_NOT_CHANGEABLE              (STD_OFF)

/*Data type for the symbolic name of a port pin.*/
typedef uint8 Port_PinType;

/*Different port pin modes*/
typedef uint8 Port_PinModeType;

/*Initial Value for a port Pin*/
typedef enum
{
    PORT_PIN_LEVEL_LOW, PORT_PIN_LEVEL_HIGH
}Port_PinLevelValueType;

/*Possible directions of a port pin*/
typedef enum
{
    PORT_PIN_IN, PORT_PIN_OUT
}Port_PinDirectionType;

/*Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistorType;

/*Port pin mode from mode list for use with Port_Init() function*/
typedef enum
{
    PORT_PIN_MODE_ADC,
    PORT_PIN_MODE_CAN,
    PORT_PIN_MODE_DIO,
    PORT_PIN_MODE_DIO_GPT,
    PORT_PIN_MODE_DIO_WDG,
    PORT_PIN_MODE_FLEXRAY,
    PORT_PIN_MODE_ICU,
    PORT_PIN_MODE_LIN,
    PORT_PIN_MODE_MEM,
    PORT_PIN_MODE_PWM,
    PORT_PIN_MODE_SPI
}Port_PinInitialModeType;

/*Type of the external data structure containing the initialization data for this module.*/
typedef struct
{
    uint8 port_num;
    Port_PinType pin_num;
    Port_PinDirectionType pin_direction;
    Port_InternalResistorType pin_resistor;
    Port_PinModeType pin_mode;
    uint8 pin_direction_changeable;
    uint8 pin_mode_changeable;
    Port_PinLevelValueType pin_initial_value;
}Port_ConfigPin;

/*Array of Structures for all the configured pins*/
typedef struct
{
    Port_ConfigPin Pin[PORT_CONFIGURED_PINS];
}Port_ConfigType;


                        /*Function Prototypes*/

/*
 * Service name: Port_Init
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): ConfigPtr Pointer to configuration set.
 * Parameters(inout):None
 * Parameters (out): None
 * Return value:     None
 * Description: Initializes the Port Driver module.
*/
void Port_Init(const Port_ConfigType* ConfigPtr);



/*
 * Service name: Port_SetPinDirection
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin       -> Port Pin ID number
                    Direction -> Port Pin Direction
 * Parameters (inout): None
 * Parameters (out):   None
 * Return value:       None
 * Description: Sets the port pin direction
*/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction);
#endif


/*
 * Service name: Port_RefreshPortDirection
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in):    None
 * Parameters (inout): None
 * Parameters (out):   None
 * Return value:       None
 * Description: Refreshes port direction.
*/
void Port_RefreshPortDirection(void);



/*
 * Service name: Port_GetVersionInfo
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in):     None
 * Parameters (inout):  None
 * Parameters (out): versioninfo Pointer to where to store the version information of this module.
 * Return value:        None
 * Description: Returns the version information of this module.
*/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif


/*
 * Service name: Port_SetPinMode
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in):     Pin  ->  Port Pin ID number
                        Mode -> New Port Pin mode to be set on port pin.
 * Parameters (inout):  None
 * Parameters (out):    None
 * Return value:        None
 * Description: Sets the port pin mode.
*/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode);
#endif

                        /*External Global Variables*/

/*External PB Structures to be used by Dio and other Modules*/
extern const Port_ConfigType Port_Configuration;


#endif /* PORT_H_ */
