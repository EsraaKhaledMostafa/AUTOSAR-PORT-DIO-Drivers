/*
 * Module: Det
 * File Name: Det.h
 * Description: Det stores the deelopment errors reported by other modules
 * Author: Esraa Khaled
 */

#ifndef DET_H_
#define DET_H_

/*ID for the Company in AUTOSAR
 * Ex: ID = 999*/
#define DET_VENDOR_ID                      (999U)


/* Module Version 1.0.0 */
#define DET_SW_MAJOR_VERSION               (1U)
#define DET_SW_MINOR_VERSION               (0U)
#define DET_SW_PATCH_VERSION               (0U)


/* AUTOSAR Version 4.0.3*/
#define DET_AR_RELEASE_MAJOR_VERSION       (4U)
#define DET_AR_RELEASE_MINOR_VERSION       (0U)
#define DET_AR_RELEASE_PATCH_VERSION       (3U)


/*Including AUTOSAR Standard Types*/
#include "Std_Types.h"

/*AUTOSAR Checking between Std Types and Det Modules*/
#if ((DET_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
    || (DET_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
    || (DET_AR_RELEASE_PATCH_VERSION != STD_TYPES_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Std_Types.h does not match the expected version"
#endif

/*Function Prototypes*/
Std_ReturnType Det_ReportError( uint16 ModuleId,
                                uint8  InstanceId,
                                uint8  ApiId,
                                uint8  ErrorId);

#endif /* DET_H_ */
