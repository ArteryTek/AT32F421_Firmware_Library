/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *
  * Copyright (c) 2025, Artery Technology, All rights reserved.
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

/** @addtogroup AT32F421_periph_examples
  * @{
  */

/** @addtogroup 421_DMA_data_to_gpio DMA_data_to_gpio
  * @{
  */

gpio_init_type  gpio_init_struct = {0};
dma_init_type dma_init_struct = {0};

#define BUFFER_SIZE                      16
uint16_t src_buffer[BUFFER_SIZE] = {0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
                                    0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F, 0x0010};

/**
  * @brief  this function handles DMA2 channel1 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel5_4_IRQHandler(void)
{
  if(dma_interrupt_flag_get(DMA1_FDT5_FLAG) != RESET)
  {
    /* turn led2/led3/led4 on */
    at32_led_on(LED2);
    at32_led_on(LED3);
    at32_led_on(LED4);
    dma_flag_clear(DMA1_FDT5_FLAG);
  }
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  /* enable dma1/gpiob/tmr1 clock */
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

  /* config gpiob pin for output mode */
  gpio_init_struct.gpio_pins = GPIO_PINS_ALL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOB, &gpio_init_struct);

  tmr_base_init(TMR1, 0xFF, 0);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);

  /* enable tmr1 overflow dma request */
  tmr_dma_request_enable(TMR1, TMR_OVERFLOW_DMA_REQUEST, TRUE);

  /* dma1 channel1 configuration */
  dma_reset(DMA1_CHANNEL5);
  dma_init_struct.buffer_size = BUFFER_SIZE;
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
  dma_init_struct.memory_base_addr = (uint32_t)src_buffer;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)&GPIOB->odt;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(DMA1_CHANNEL5, &dma_init_struct);

  /* enable transfer full data interrupt */
  dma_interrupt_enable(DMA1_CHANNEL5, DMA_FDT_INT, TRUE);

  /* dma1 channel1 interrupt nvic init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(DMA1_Channel5_4_IRQn, 1, 0);

  /* enable dma channel */
  dma_channel_enable(DMA1_CHANNEL5, TRUE);

  /* enable tmr1 */
  tmr_counter_enable(TMR1, TRUE);

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
