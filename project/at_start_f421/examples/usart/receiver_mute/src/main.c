/**
  **************************************************************************
  * @file     main.c
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

/** @addtogroup AT32F421_periph_examples
  * @{
  */

/** @addtogroup 421_USART_receiver_mute USART_receiver_mute
  * @{
  */

#define BUFFER_SIZE                      7
#define MATCH_ID_VAL                     0x81
#define ERROR_ID_VAL                     0x82
#define DATA1_VAL                        0x7F
#define DATA2_VAL                        0x7A
#define DATA3_VAL                        0x7B
#define DATA4_VAL                        0x7C

uint8_t usart2_tx_buffer[BUFFER_SIZE] = {DATA1_VAL, MATCH_ID_VAL, DATA2_VAL, ERROR_ID_VAL, DATA3_VAL, MATCH_ID_VAL, DATA4_VAL};
uint8_t usart1_rx_buffer[BUFFER_SIZE];
uint8_t tx_counter = 0;
uint8_t rx_counter = 0;

/**
  * @brief  config usart2 and usart1
  * @param  none
  * @retval none
  */
void usart_configuration(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the usart2 and gpio clock */
  crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* enable the usart1 and gpio clock */
  crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the usart2 tx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* config usart2 iomux */
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE2, GPIO_MUX_1);

  /* configure the usart1 rx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  /* config usart1 iomux */
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE7, GPIO_MUX_0);

  /* configure usart2 param */
  usart_init(USART2, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART2, TRUE);
  usart_enable(USART2, TRUE);

  /* config usart1 nvic interrupt */
  nvic_irq_enable(USART1_IRQn, 0 ,0);

  /* configure usart1 param */
  usart_init(USART1, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_receiver_enable(USART1, TRUE);
  usart_interrupt_enable(USART1, USART_RDBF_INT, TRUE);
  usart_wakeup_id_set(USART1, 0x01);
  usart_wakeup_mode_set(USART1, USART_WAKEUP_BY_MATCHING_ID);
  usart_receiver_mute_enable(USART1, TRUE);
  usart_enable(USART1, TRUE);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  at32_board_init();
  usart_configuration();

  /* usart2 transmit data */
  while(tx_counter < BUFFER_SIZE)
  {
    while(usart_flag_get(USART2, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(USART2, usart2_tx_buffer[tx_counter++]);
  }

  /* wait usart2 data transmission complete */
  while(usart_flag_get(USART2, USART_TDC_FLAG) == RESET);

  while(1)
  {
    if((usart1_rx_buffer[0] == MATCH_ID_VAL) && (usart1_rx_buffer[1] == DATA2_VAL) && \
      (usart1_rx_buffer[2] == MATCH_ID_VAL) && (usart1_rx_buffer[3] == DATA4_VAL))
    {
      at32_led_toggle(LED2);
      at32_led_toggle(LED3);
      at32_led_toggle(LED4);
      delay_sec(1);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
