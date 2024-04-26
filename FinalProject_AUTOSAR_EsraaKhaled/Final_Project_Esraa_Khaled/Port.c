/*
 * Module: Port
 * File Name: Port.c
 * Description: Source File for TM4C123GH6PM Microcontroller - Port Driver
 * Author: Esraa Khaled
 */


#include "Port.h"
#include "tm4c123gh6pm_registers.h"

#if(PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"

/*AUTOSAR Version Checking between Det and PORT Module*/
#if ((DET_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
  || (DET_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
  || (DET_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif

/*Static Global Variables Definition*/
STATIC const Port_ConfigPin *Port_configPtr = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

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
void Port_Init(const Port_ConfigType* ConfigPtr)
{
    /*Steps for Initialization:
     * Get the Base Address of the Port of every pin
     * Check if this pin needs unlocking the commit or if it is from the JTAG pins
     * Check the mode of the Pin DIO, ADC, Rest of Modes
     * Set the Direction of the Pin and the Resistor Type
     * Set the Initial Value for the Pin
     * */
    volatile Port_PinType pinIndex = PORT_PIN0_ID;
    volatile uint32 *Port_Base_Address_Ptr = NULL_PTR;

    /*Check for Configuration pointing to Null*/
#if(PORT_DEV_ERROR_DETECT == STD_ON)
    if(NULL_PTR == ConfigPtr)
    {
        Det_ReportError(PORT_MODULE_ID,
                        PORT_INSTANCE_ID,
                        PORT_INIT_SID,
                        PORT_E_PARAM_CONFIG);
    }
    else
#endif
    {
        Port_Status = PORT_INITIALIZED;
        Port_configPtr = ConfigPtr->Pin;
    }

    for(pinIndex = PORT_PIN0_ID; pinIndex < PORT_CONFIGURED_PINS; pinIndex++)
    {
        /*Saving the Base Address of the port of the specified pin*/
        switch(Port_configPtr[pinIndex].port_num)
        {
        case PORT_PORTA_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTA_BASE_ADDRESS;
            break;
        case PORT_PORTB_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTB_BASE_ADDRESS;
            break;
        case PORT_PORTC_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTC_BASE_ADDRESS;
            break;
        case PORT_PORTD_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTD_BASE_ADDRESS;
            break;
        case PORT_PORTE_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTE_BASE_ADDRESS;
            break;
        case PORT_PORTF_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTF_BASE_ADDRESS;
            break;
        default:
            break;
        }

        /*Checking for the locked pins to unlock the commit*/
        if((Port_configPtr[pinIndex].port_num == PORT_PORTD_ID) && (Port_configPtr[pinIndex].pin_num == PORT_PIN7_ID)\
          || (Port_configPtr[pinIndex].port_num == PORT_PORTF_ID) && (Port_configPtr[pinIndex].pin_num == PORT_PIN0_ID))
        {
            /*Unlock the GPIOCR Register*/
            *(volatile uint32*)((volatile uint8*)Port_Base_Address_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;

            /*Set the corresponding bit in GPIOCR register to allow changes to this pin*/
            SET_BIT(*(volatile uint32*)((volatile uint8*)Port_Base_Address_Ptr + PORT_LOCK_REG_OFFSET), Port_configPtr[pinIndex].pin_num);
        }
        else if((Port_configPtr[pinIndex].port_num == PORT_PORTC_ID) && (Port_configPtr[pinIndex].pin_num <= PORT_PIN3_ID))
        {
            /*Do Nothing since these are the JTAG pins*/
            continue;
        }
        else
        {
            /*Do Nothing Other pins does not need unlocking the Commit Register*/
        }

        /*Check Pin Modes*/
        if(Port_configPtr[pinIndex].pin_mode == PORT_PIN_MODE_DIO)
        {
            /*
             * 1. Disable Analog Functionality
             * 2. Clear the 4 PMCx bits in GPIOPCTL
             * 3. Disable Alternative Functionality
             * 4. Enable Digital Functionality
            */

            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AMSEL_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);

            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_PCTL_REG_OFFSET) &= ~(0x0000000F << (Port_configPtr[pinIndex].pin_num * 4));

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AFSEL_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DEN_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
        }
        else if(Port_configPtr[pinIndex].pin_mode == PORT_PIN_MODE_ADC)
        {
            /*
             * 1. Enable Analog Functionality
             * 2. Clear the 4 PMCx bits in GPIOPCTL
             * 3. Disable Alternative Functionality
             * 4. Disable Digital Functionality
            */

            /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AMSEL_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);

            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_PCTL_REG_OFFSET) &= ~(0x0000000F << (Port_configPtr[pinIndex].pin_num * 4));

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AFSEL_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);

            /* clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DEN_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
        }
        else
        {
            /*
             * Any Other Mode Steps:
             * 1. Disable Analog Functionality
             * 2. Enable Alternative Functionality
             * 3. Write the Alternative Function ID in PMCx bits for this pin
             * 4. Disable Digital Functionality
            */

            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AMSEL_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);

            /* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AFSEL_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);

            /* Write the Alternative Function ID in PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_PCTL_REG_OFFSET) |= ~(0x0000000F << (Port_configPtr[pinIndex].pin_num * 4));

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DEN_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
        }

        /*Setting the Direction of the pin*/
        if(Port_configPtr[pinIndex].pin_direction == PORT_PIN_OUT)
        {
            /*Set the Corresponding bit in GPIODIR to configure it as output pin*/
            SET_BIT(*(volatile uint32*)((volatile uint8*)Port_Base_Address_Ptr + PORT_DIR_REG_OFFSET), Port_configPtr[pinIndex].pin_num);

            /*Initializes the pin with the initial value*/
            if(Port_configPtr[pinIndex].pin_initial_value == PORT_PIN_LEVEL_LOW)
            {
                /*Clear the corresponding bit in GPIO Data Register*/
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DATA_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
            }
            else if(Port_configPtr[pinIndex].pin_initial_value == PORT_PIN_LEVEL_HIGH)
            {
               /*Set the Corresponding bit in GPIO Data Register*/
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DATA_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
            }
        }
        else if(Port_configPtr[pinIndex].pin_direction == PORT_PIN_IN)
        {
            /*Clear the Corresponding bit in GPIODIR to configure it as input pin*/
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)Port_Base_Address_Ptr + PORT_DIR_REG_OFFSET), Port_configPtr[pinIndex].pin_num);

            /*Check for the resistor*/
            if(Port_configPtr[pinIndex].pin_resistor == PULL_UP)
            {
                /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_PUR_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
            }
            else if(Port_configPtr[pinIndex].pin_resistor == PULL_DOWN)
            {
                /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_PDR_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
            }
            else
            {
                /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_PUR_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);

                /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_PDR_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
            }
        }
        else
        {
            /*Do Nothing*/
        }
    }
}

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

void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction)
{
    volatile uint32 *Port_Base_Address_Ptr = NULL_PTR;
    uint8 error = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* checks if the pin ID valid or not */
    if ( Pin >= PORT_CONFIGURED_PINS)
    {
        Det_ReportError(PORT_MODULE_ID,
                PORT_INSTANCE_ID,
                PORT_SET_PIN_DIRECTION_SID,
                PORT_E_PARAM_PIN);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }

    /* checks if the pin direction is changeable or not */
    if (Port_configPtr[Pin].pin_direction_changeable == PORT_PIN_DIRECTION_NOT_CHANGEABLE)
    {
        Det_ReportError(PORT_MODULE_ID,
                PORT_INSTANCE_ID,
                PORT_SET_PIN_DIRECTION_SID,
                PORT_E_DIRECTION_UNCHANGEABLE);

        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }

    /* checks if the port initialized or not */
    if (Port_Status == PORT_NOT_INITIALIZED) {
        Det_ReportError(PORT_MODULE_ID,
                PORT_INSTANCE_ID,
                PORT_SET_PIN_DIRECTION_SID,
                PORT_E_UNINIT);

        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }
#endif

    if(FALSE == error)
    {
        /*Saving the Base Address of the port of the specified pin*/
        switch(Port_configPtr[Pin].port_num)
        {
        case PORT_PORTA_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTA_BASE_ADDRESS;
            break;
        case PORT_PORTB_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTB_BASE_ADDRESS;
            break;
        case PORT_PORTC_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTC_BASE_ADDRESS;
            break;
        case PORT_PORTD_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTD_BASE_ADDRESS;
            break;
        case PORT_PORTE_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTE_BASE_ADDRESS;
            break;
        case PORT_PORTF_ID:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTF_BASE_ADDRESS;
            break;
        default:
            break;
        }

        if(Direction == PORT_PIN_OUT)
        {
            /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DIR_REG_OFFSET) , Port_configPtr[Pin].pin_num);

        }
        else if(PORT_PIN_IN == Direction)
        {
            /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DIR_REG_OFFSET) , Port_configPtr[Pin].pin_num);
        }
        else
        {
            /* Do Nothing */
        }
    }
   else
   {
       /*Do Nothing*/
   }
}
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
void Port_RefreshPortDirection(void)
{
    uint8 error = FALSE;
    volatile uint32 * Port_Base_Address_Ptr = NULL_PTR;
    volatile Port_PinType pinIndex = PORT_PIN0_ID;


    #if (PORT_DEV_ERROR_DETECT == STD_ON)

        /* check if the port initialized or not */
        if (Port_Status == PORT_NOT_INITIALIZED)
        {
            Det_ReportError(PORT_MODULE_ID,
                    PORT_INSTANCE_ID,
                    PORT_REFRESH_PORT_DIRECTION_SID,
                    PORT_E_UNINIT);
            error = TRUE;
        }
        else
        {
            /* Do Nothing */
        }
    #endif

        if(FALSE == error)
        {
            /*Saving the Base Address of the port of the specified pin*/
            switch(Port_configPtr[pinIndex].port_num)
            {
            case PORT_PORTA_ID:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTA_BASE_ADDRESS;
                break;
            case PORT_PORTB_ID:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTB_BASE_ADDRESS;
                break;
            case PORT_PORTC_ID:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTC_BASE_ADDRESS;
                break;
            case PORT_PORTD_ID:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTD_BASE_ADDRESS;
                break;
            case PORT_PORTE_ID:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTE_BASE_ADDRESS;
                break;
            case PORT_PORTF_ID:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTF_BASE_ADDRESS;
                break;
            default:
                break;
            }

            for(pinIndex = PORT_PIN0_ID; pinIndex < PORT_CONFIGURED_PINS; pinIndex++)
            {
                if(Port_configPtr[pinIndex].pin_direction_changeable == PORT_PIN_DIRECTION_CHANGEABLE)
                {
                    if(Port_configPtr[pinIndex].pin_direction == PORT_PIN_OUT)
                    {
                        /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DIR_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
                    }
                    else if(Port_configPtr[pinIndex].pin_direction == PORT_PIN_IN)
                    {
                        /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DIR_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
                    }
                    else
                    {
                        /*Do Nothing*/
                    }
                }
               else
               {
                   /*Do Nothing*/
               }
            }
        }
        else
        {
            /*Do Nothing*/
        }
}



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
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if input pointer is not Null pointer */
    if(NULL_PTR == versioninfo)
    {
        /* Report to DET  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
        /* Copy the module Id */
        versioninfo->moduleID = (uint16)PORT_MODULE_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
    }
}
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
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode)
{
    uint8 error = FALSE;
    volatile uint32 * Port_Base_Address_Ptr = NULL_PTR;

#if (PORT_DEV_ERROR_DETECT == STD_ON)

    /* check if the pin ID valid or not */
    if ( Pin >= PORT_CONFIGURED_PINS)
    {
        Det_ReportError(PORT_MODULE_ID,
                PORT_INSTANCE_ID,
                PORT_SET_PIN_MODE_SID,
                PORT_E_PARAM_PIN);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }

    /* check if the pin Mode is changeable or not */
    if (Port_configPtr[Pin].pin_mode_changeable == PORT_PIN_MODE_NOT_CHANGEABLE)
    {
        Det_ReportError(PORT_MODULE_ID,
                PORT_INSTANCE_ID,
                PORT_SET_PIN_MODE_SID,
                PORT_E_MODE_UNCHANGEABLE);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }

    /* check if the port initialized or not */
    if (Port_Status == PORT_NOT_INITIALIZED) {
        Det_ReportError(PORT_MODULE_ID,
                PORT_INSTANCE_ID,
                PORT_SET_PIN_MODE_SID,
                PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }
#endif

    if(FALSE == error)
    {
        /*Check Pin Modes*/
        if(Mode == PORT_PIN_MODE_DIO)
        {
            /*
             * 1. Disable Analog Functionality
             * 2. Clear the 4 PMCx bits in GPIOPCTL
             * 3. Disable Alternative Functionality
             * 4. Enable Digital Functionality
            */

            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AMSEL_REG_OFFSET) , Port_configPtr[Pin].pin_num);

            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_PCTL_REG_OFFSET) &= ~(0x0000000F << (Port_configPtr[Pin].pin_num * 4));

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AFSEL_REG_OFFSET) , Port_configPtr[Pin].pin_num);

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DEN_REG_OFFSET) , Port_configPtr[Pin].pin_num);
        }
        else if(Mode == PORT_PIN_MODE_ADC)
        {
            /*
             * 1. Enable Analog Functionality
             * 2. Clear the 4 PMCx bits in GPIOPCTL
             * 3. Disable Alternative Functionality
             * 4. Disable Digital Functionality
            */

            /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AMSEL_REG_OFFSET) , Port_configPtr[Pin].pin_num);

            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_PCTL_REG_OFFSET) != ~(0x0000000F << (Port_configPtr[Pin].pin_num * 4));

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AFSEL_REG_OFFSET) , Port_configPtr[Pin].pin_num);

            /* clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DEN_REG_OFFSET) , Port_configPtr[Pin].pin_num);
        }
        else
        {
            /*
             * Any Other Mode Steps:
             * 1. Disable Analog Functionality
             * 2. Enable Alternative Functionality
             * 3. Write the Alternative Function ID in PMCx bits for this pin
             * 4. Disable Digital Functionality
            */

            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AMSEL_REG_OFFSET) , Port_configPtr[Pin].pin_num);

            /* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_AFSEL_REG_OFFSET) , Port_configPtr[Pin].pin_num);

            /* Write the Alternative Function ID in PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_PCTL_REG_OFFSET) |= ~(0x0000000F << (Port_configPtr[Pin].pin_num * 4));

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_DEN_REG_OFFSET) , Port_configPtr[Pin].pin_num);
        }
    }
    else
    {
        /*Do Nothing*/
    }

}
#endif



