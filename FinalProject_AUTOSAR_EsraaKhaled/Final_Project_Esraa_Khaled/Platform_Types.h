/*
 * Module: Common - Platform Abstraction
 * File Name: Platform_Types.h
 * Description: Platform Types for ARM Cortex-M4F
 * Author: Esraa Khaled
 */

#ifndef PLATFORM_TYPES_H_
#define PLATFORM_TYPES_H_

/*ID for the Company in AUTOSAR
 * Ex: ID = 999*/
#define PLATFORM_VENDOR_ID                      (999U)


/* Module Version 1.0.0 */
#define PLATFORM_SW_MAJOR_VERSION               (1U)
#define PLATFORM_SW_MINOR_VERSION               (0U)
#define PLATFORM_SW_PATCH_VERSION               (0U)


/* AUTOSAR Version 4.0.3*/
#define PLATFORM_AR_RELEASE_MAJOR_VERSION       (4U)
#define PLATFORM_AR_RELEASE_MINOR_VERSION       (0U)
#define PLATFORM_AR_RELEASE_PATCH_VERSION       (3U)


/*CPU Register Type Width*/
#define CPU_TYPE_8                              (8U)
#define CPU_TYPE_16                             (16U)
#define CPU_TYPE_32                             (32U)


/*Bit Order Definition*/
#define MSB_FIRST                               (0U)  /*Big Endian Bit Ordering*/
#define LSB_FIRST                               (1U)  /*Little Endian Bit Ordering*/


/*Byte Order Definition*/
#define HIGH_BYTE_FIRST                         (0U)  /*Big Endian Byte Ordering*/
#define LOW_BYTE_FIRST                          (1U)  /*Little Endian Byte Ordering*/


/*Platform type and endianess definitions -> specific for ARM Cortex-M4F*/
#define CPU_TYPE                                CPU_TYPE_32
#define CPU_BIT_ORDER                           LSB_FIRST
#define CPU_BYTE_ORDER                          LOW_BYTE_FIRST


/*Boolean Values*/
#ifndef FALSE
#define FALSE                                   (0U)
#endif

#ifndef TRUE
#define TRUE                                    (1U)
#endif

typedef unsigned char         boolean;


/*Types Definitions*/

typedef unsigned char         uint8;          /*           0 .. 255             */
typedef signed char           sint8;          /*        -128 .. +127            */
typedef unsigned short        uint16;         /*           0 .. 65535           */
typedef signed short          sint16;         /*      -32768 .. +32767          */
typedef unsigned long         uint32;         /*           0 .. 4294967295      */
typedef signed long           sint32;         /* -2147483648 .. +2147483647     */
typedef unsigned long long    uint64;         /*       0..18446744073709551615  */
typedef signed long long      sint64;         /* -9223372036854775808 .. 9223372036854775807 */
typedef float                 float32;
typedef double                float64;





#endif /* PLATFORM_TYPES_H_ */
