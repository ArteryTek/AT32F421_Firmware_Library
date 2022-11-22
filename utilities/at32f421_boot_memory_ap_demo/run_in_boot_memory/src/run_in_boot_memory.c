/**
  **************************************************************************
  * @file     run_in_boot_memory.c
  * @brief    code for run in boot memmory
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f421_board.h"
#include "at32f421_clock.h"
#include "run_in_boot_memory.h"
#include "stdio.h"


/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup AP_run_in_boot_memory
  * @{
  */

void run_in_boot_memory_test(void)
{

  /* initialize delay function */
  delay_init();

  /* configure led in at_start_board */
  at32_led_init(LED2);

  while(1)
  {
    /* toggle led2 */
    at32_led_toggle(LED2);
    delay_ms(500);
  }
}


/**
  * @}
  */

/**
  * @}
  */

