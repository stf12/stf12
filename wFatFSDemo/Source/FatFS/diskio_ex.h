/**
 * @file diskio_ex.h
 * @ingroup SERVICE_IF
 *
 * @version 1.0.1
 * @date May 23, 2011
 * @author IMS Systems Lab - ART Team
 * @note Module: FAT IF.
 *
 * @brief It is part of <b>FAT IF</b> module. Extension API for FAT IF.
 *
 * This file declares the API to manage more than one volume. For example, if the system
 * uses two external FLASH chips, they are managed as two independent drive, A and B.
 */

/*
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 */

#ifndef DISKIO_EX_H_
#define DISKIO_EX_H_

#include "ff.h"
#include "diskio.h"

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * Specifies the number of the available logical drives.
 */
#define FAT_DRIVE_COUNT 1

/**
 * Special extension command for the disk_ioctl() in order to erase all chip in an efficient way.
 * \code
 * // Example
 * disk_ioctl(0, CTRL_ERASE_CHIP, NULL);
 * \endcode
 */
#define CTRL_ERASE_CHIP 5

/**
 * Get the handle to a FATFS object. The application use this function to retrieve a
 * FATFS handler to pass to the FatFS API like f_mount() f_chdrive() and f_mkfs().
 *
 * @param nVol Specifies the logical drive identifier. Valid value are in the range [0, FAT_DRIVE_COUNT[
 * @return An handle to a FATFA object or NULL if nVol is invalid.
 */
FATFS *FatGetDriveHandler(BYTE nVol);

#ifdef __cplusplus
 }
#endif


#endif /* DISKIO_EX_H_ */
