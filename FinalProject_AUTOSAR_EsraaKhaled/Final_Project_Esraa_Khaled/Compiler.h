/*
 * Module: Common - Compiler Abstraction
 * File Name: Compiler.h
 * Description: This file contains the definitions and macros specified by
 *               AUTOSAR for the abstraction of compiler specific keywords.
 * Author: Esraa Khaled
 */

#ifndef COMPILER_H_
#define COMPILER_H_

/*ID for the Company in AUTOSAR
 * Ex: ID = 999*/
#define COMPILER_VENDOR_ID                      (999U)


/* Module Version 1.0.0 */
#define COMPILER_SW_MAJOR_VERSION               (1U)
#define COMPILER_SW_MINOR_VERSION               (0U)
#define COMPILER_SW_PATCH_VERSION               (0U)


/* AUTOSAR Version 4.0.3*/
#define COMPILER_AR_RELEASE_MAJOR_VERSION       (4U)
#define COMPILER_AR_RELEASE_MINOR_VERSION       (0U)
#define COMPILER_AR_RELEASE_PATCH_VERSION       (3U)


/*Memory Class used for declarations of local pointers*/
#define AUTOMATIC

/*Memory CLass used within type definitions where no memory qualifier can be specified*/
#define TYPEDEF

/*Void Pointer defined to zero definition to define the NULL Pointer*/
#define NULL_PTR                                ((void*)0)

/*Define the Abstraction of the Compiler Keyword inline*/
#define INLINE                                  inline

/*Define local inline functon*/
#define LOCAL_INLINE                            static inline

/*Define the Abstraction of the Compiler Keyword static*/
#define STATIC                                  static


#endif /* COMPILER_H_ */
