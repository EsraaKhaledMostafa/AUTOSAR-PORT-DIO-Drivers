/*
 * Module: DIO
 * File Name: DIO_Cfg.h
 * Description: Pre-Compile Configuration Header File for TM4C123GH6PM Microcontroller - Dio Driver
 * Author: Esraa Khaled
 */


#ifndef DIO_CFG_H_
#define DIO_CFG_H_


/* Module Version 1.0.0 */
#define DIO_CFG_SW_MAJOR_VERSION             (1U)
#define DIO_CFG_SW_MINOR_VERSION             (0U)
#define DIO_CFG_SW_PATCH_VERSION             (0U)


/* AUTOSAR Version 4.0.3*/
#define DIO_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define DIO_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define DIO_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/*Pre-Compile Option for Development Error Detection*/
#define DIO_DEV_ERROR_DETECT                 (STD_ON)

/*Pre-Compile Option for Adding/removing the service Dio_FlipChannel() from the code*/
#define DIO_FLIP_CHANNEL_API                 (STD_ON)

/*Pre-Compile Option for Adding/removing the service Dio_ GetVersionInfo() from the code.*/
#define DIO_VERSION_INFO_API                 (STD_OFF)

/* Number of the configured Dio Channels */
#define DIO_CONFIGURED_CHANNLES              (2U)

/* Channel Index in the array of structures in Dio_PBcfg.c */
#define DioConf_LED1_CHANNEL_ID_INDEX        (uint8)0x00
#define DioConf_SW1_CHANNEL_ID_INDEX         (uint8)0x01

/* DIO Configured Port ID's  */
#define DioConf_LED1_PORT_NUM                (Dio_PortType)5 /* PORTF */
#define DioConf_SW1_PORT_NUM                 (Dio_PortType)5 /* PORTF */

/* DIO Configured Channel ID's */
#define DioConf_LED1_CHANNEL_NUM             (Dio_ChannelType)1 /* Pin 1 in PORTF */
#define DioConf_SW1_CHANNEL_NUM              (Dio_ChannelType)4 /* Pin 4 in PORTF */

#endif /* DIO_CFG_H_ */
