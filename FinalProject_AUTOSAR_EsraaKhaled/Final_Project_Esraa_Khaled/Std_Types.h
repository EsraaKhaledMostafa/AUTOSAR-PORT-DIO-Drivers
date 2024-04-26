/*
 * Module: Common - STD_TYPES
 * File Name: Std_Types.h
 * Description: General Types Definitions
 * Author: Esraa Khaled
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#include "Platform_Types.h"
#include "Compiler.h"

/*ID for the Company in AUTOSAR
 * Ex: ID = 999*/
#define STD_TYPES_VENDOR_ID                      (999U)


/* Module Version 1.0.0 */
#define STD_TYPES_SW_MAJOR_VERSION               (1U)
#define STD_TYPES_SW_MINOR_VERSION               (0U)
#define STD_TYPES_SW_PATCH_VERSION               (0u)


/* AUTOSAR Version 4.0.3*/
#define STD_TYPES_AR_RELEASE_MAJOR_VERSION       (4U)
#define STD_TYPES_AR_RELEASE_MINOR_VERSION       (0U)
#define STD_TYPES_AR_RELEASE_PATCH_VERSION       (3U)


/*The Standard Return Type Definition used in the project*/
typedef uint8 Std_ReturnType;

/*Structure of the Version of the Module
 * This is requested by calling <Module name>_GetVersionInfo()*/
typedef struct
{
    uint16 vendorID;
    uint16 moduleID;
    uint8 sw_major_version;
    uint8 sw_minor_version;
    uint8 sw_patch_version;
}Std_VersionInfoType;

/*Frequently Used Definitions*/
#define STD_HIGH                (0x01U)         /* Standard HIGH */
#define STD_LOW                 (0x00U)         /* Standard LOW */

#define STD_ACTIVE              (0x01U)          /* Standard ACTIVE */
#define STD_IDLE                (0x00U)          /* Standard IDLE */

#define STD_ON                  (0x01U)          /* Standard ON */
#define STD_OFF                 (0x00U)          /* Standard OFF */


#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED

/*Because E_OK is already defined within OSEK, the symbol E_OK has
to be shared. To avoid name clashes and redefinition problems*/
#define E_OK                    ((Std_ReturnType)0x00U)         /* Function Returns OK */
typedef uint8 StatusType;                                       /* OSEK compliance */
#endif

#define E_NOT_OK                ((Std_ReturnType)0x01U)         /* Function Returns NOT OK */


#endif /* STD_TYPES_H_ */
