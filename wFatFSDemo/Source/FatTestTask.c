/**
 * @file FatTestTask.c
 * @ingroup APPLICATION_TASK
 *
 * @version 1.0.1
 * @date Jun 14, 2011
 * @author Stefano Oliveri
 * @note Module: Application Task.
 *
 * @brief It is part of <b>Application Task</b> module. FatFS test task.
 *
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
 * <h2><center>&copy; COPYRIGHT 2011 Stefano Oliveri</center></h2>
 */

#include "FatTestTask.h"
#include "task.h"
#include "ff.h"
#include "stm3210e_eval_lcd.h"
#include <string.h>
#include <assert.h>

#define FDISK_FORMAT	0
#define SFD_FORMAT		1
#define DRIVE_A			0

/**
 * Specifies the File System Object.
 */
static FATFS s_xVolume;

static void FatTaskFunc(void *pParams);
static void FatClearDisplay();

void FatHardwareInit() {
	LCD_DeInit();
	STM3210E_LCD_Init();
	LCD_SetColors(LCD_COLOR_WHITE, LCD_COLOR_BLUE);
	FatClearDisplay();
}

void FatStartTask(portBASE_TYPE nPriority, portBASE_TYPE nStackDepth) {
	FRESULT xRes = f_mount(0, &s_xVolume);
	if (xRes == FR_OK) {
		xTaskCreate(FatTaskFunc, (signed char*)"FAT", nStackDepth, NULL, nPriority, NULL);
	}
}

/**
 * FAT Test task control function.
 *
 * @param pParams NOT USED.
 */
static void FatTaskFunc(void *pParams) {
	FRESULT xRes;
	FIL xFile;
	const char cWBuffer[] = "Hello FAT World!";


#ifdef FAT_MAKEFS_TEST
	// this delay is to prevent the FAT corruption after a systenm reset.
	vTaskDelay(5000 / portTICK_RATE_MS);
	LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"MAKE FS Test");
	LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)"format the ext FLASH");
	LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"please wait...");
	xRes = f_mkfs(0, SFD_FORMAT, 1024);
	assert(xRes == FR_OK);
#endif

#ifdef FAT_WRITE_TEST
	vTaskDelay(2000 / portTICK_RATE_MS);
	FatClearDisplay();
	LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"WRITE Test");
	LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)"open file: W+CA");
	xRes = f_open(&xFile, "pippo.txt", FA_WRITE|FA_CREATE_ALWAYS);
	assert(xRes == FR_OK);
	UINT n;
	LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"write file");
	xRes = f_write(&xFile, cWBuffer, strlen(cWBuffer), &n);
	assert(xRes == FR_OK);
	assert(n == strlen(cWBuffer));
	LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"close file");
	xRes = f_close(&xFile);
	assert(xRes == FR_OK);
#endif

#ifdef FAT_READ_TEST
	vTaskDelay(2000 / portTICK_RATE_MS);
	FatClearDisplay();
	LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"READ Test");
	LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)"open file: R+OE");
	xRes = f_open(&xFile, "pippo.txt", FA_READ|FA_OPEN_EXISTING);
	assert(xRes == FR_OK);
	UINT m;
	char cRBuffer[128];
	LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"read file");
	xRes = f_read(&xFile, cRBuffer, sizeof(cRBuffer), &m);
	assert(xRes == FR_OK);
	assert(m == strlen(cWBuffer));
	LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"close file");
	LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)"file content:");
	LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)cRBuffer);
	xRes = f_close(&xFile);
	assert(xRes == FR_OK);
#endif
	LCD_DisplayStringLine(LCD_LINE_9, (uint8_t *)"test success!!!");

	vTaskDelete(NULL);

}

static void FatClearDisplay() {
	LCD_Clear(LCD_COLOR_BLUE);
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"FatFS Demo");
}
