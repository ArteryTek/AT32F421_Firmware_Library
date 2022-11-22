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

/** @addtogroup 421_TMR_encoder_tmr2 TMR_encoder_tmr2
  * @{
  */

gpio_init_type gpio_init_struct = {0};
crm_clocks_freq_type crm_clocks_freq_struct = {0};

void delay(uint32_t time);
uint32_t counter;

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);

  /* turn led2/led3/led4 on */
  at32_led_on(LED2);
  at32_led_on(LED3);
  at32_led_on(LED4);

  /* enable tmr3/gpioa clock */
  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* timer3 output pin Configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_6 | GPIO_PINS_7;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE6, GPIO_MUX_1);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE7, GPIO_MUX_1);

  gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_3;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  /* tmr3 encoder mode configuration
     tmr3 ti1pf1 ,ti2fp2 as encoder input pin, tmr3 counter
     changed each signal edge. */

  tmr_base_init(TMR3, 0xFFFF, 0);
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);

  /* config encoder mode */
  tmr_encoder_mode_config(TMR3, TMR_ENCODER_MODE_C, TMR_INPUT_RISING_EDGE, TMR_INPUT_RISING_EDGE);

  /* enable tmr2 */
  tmr_counter_enable(TMR3, TRUE);

  while(1)
  {
    /* generate encoder signal */
    gpio_bits_set(GPIOA, GPIO_PINS_2);
    delay(150);
    gpio_bits_set(GPIOA, GPIO_PINS_3);
    delay(150);
    gpio_bits_reset(GPIOA, GPIO_PINS_2);
    delay(150);
    gpio_bits_reset(GPIOA, GPIO_PINS_3);
    delay(150);

    /* get current counter value */
    counter = tmr_counter_value_get(TMR3);
  }
}

/**
  * @brief  delay function
  * @param  time: number of time to delay
  * @retval none
  */
void delay(uint32_t time)
{
  uint32_t i;

  for(i = 0; i < time; i++);
}

/**
  * @}
  */

/**
  * @}
  */
