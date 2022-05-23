/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.6
  * @date     2022-05-20
  * @brief    main program
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

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup AP_read_write_boot_memory
  * @{
  */

#define TEST_BUFEER_SIZE                 0x200
#define TEST_FLASH_ADDRESS_START         0x1FFFE400

uint16_t buffer_write[TEST_BUFEER_SIZE];
uint16_t buffer_read[TEST_BUFEER_SIZE];

error_status buffer_compare(uint16_t* p_buffer1, uint16_t* p_buffer2, uint16_t buffer_length);

/**
  * @brief  compares two buffers.
  * @param  p_buffer1, p_buffer2: buffers to be compared.
  * @param  buffer_length: buffer's length
  * @retval SUCCESS: p_buffer1 identical to p_buffer2
  *         failed: p_buffer1 differs from p_buffer2
  */
error_status buffer_compare(uint16_t* p_buffer1, uint16_t* p_buffer2, uint16_t buffer_length)
{
  while(buffer_length--)
  {
    if(*p_buffer1 != *p_buffer2)
    {
      return ERROR;
    }
    p_buffer1++;
    p_buffer2++;
  }
  return SUCCESS;
}
/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  uint32_t index=0;
  system_clock_config();
  at32_board_init();
  /* fill buffer_write data to test */
  for(index = 0; index < TEST_BUFEER_SIZE; index++)
  {
    buffer_write[index] = index;
  }

  flash_unlock();

  /* erase one sector in boot memory */
  flash_sector_erase(TEST_FLASH_ADDRESS_START);

  /* write data to boot memory */
  for(index = 0; index < TEST_BUFEER_SIZE; index++)
  {
    flash_halfword_program(TEST_FLASH_ADDRESS_START + index * sizeof(uint16_t), buffer_write[index]);
  }

  /* read data from boot memory */
  for(index = 0; index < TEST_BUFEER_SIZE; index++)
  {
    buffer_read[index] = *(uint16_t*)(TEST_FLASH_ADDRESS_START + index * sizeof(uint16_t));
  }

  /* compare the buffer */
  if(buffer_compare(buffer_write, buffer_read, TEST_BUFEER_SIZE) == SUCCESS)
  {
    at32_led_on(LED2);
    at32_led_on(LED3);
    at32_led_on(LED4);
  }

  while(1)
  {
  }
}

/**
  * @}
  */

/**
  * @}
  */


