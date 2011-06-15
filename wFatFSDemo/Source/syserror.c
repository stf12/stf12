/**
 * @file syserror.c
 *
 * @date Jan 27, 2011
 * @author Stefano Oliveri
 */

#include "syserror.h"
#include "stm32f10x.h"

gts_error_t g_nGtsError = {0};

// stm32f10x_conf.h #define  USE_FULL_ASSERT
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  for (int a=10;a>0;a--)
	printf("%c",file[strlen(file)-a]);

  printf("%d\r\n", line) ;

/* Infinite loop */
  while (1)
  {
  }
}
#endif
