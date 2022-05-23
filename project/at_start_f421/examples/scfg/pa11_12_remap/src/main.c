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

/** @addtogroup AT32F421_periph_examples
  * @{
  */

/** @addtogroup 421_SCFG_pa11_12_remap SCFG_pa11_12_remap
  * @{
  */

tmr_output_config_type tmr_oc_init_structure;

uint16_t c1dt_val = 333;
uint16_t prescaler_value = 0;

void crm_configuration(void);
void gpio_configuration(void);

/**
  * @brief  configure the TMR1 pins.
  * @param  none
  * @retval none
  */
void gpio_configuration(void)
{
  gpio_init_type gpio_init_struct;

  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_11;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE11, GPIO_MUX_2);
}

/**
  * @brief  configures the different peripheral clocks.
  * @param  none
  * @retval none
  */
void crm_configuration(void)
{
  /* tmr1 clock enable */
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

  /* gpioa clock enable */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* scfg clock enable */
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  /* peripheral clocks configuration */
  crm_configuration();

  /* config function tmr1ch4 with pa11 remap pin pa9 */
  scfg_pa11pa12_pin_remap(SCFG_PA11PA12_TO_PA9PA10);

  /* gpio configuration */
  gpio_configuration();

  /* compute the prescaler value */
  prescaler_value = (uint16_t)(system_core_clock / 24000000) - 1;

  /* TMR1 time base configuration */
  tmr_base_init(TMR1, 665, prescaler_value);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);

  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_4, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_4, c1dt_val);
  tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_4, TRUE);

  tmr_period_buffer_enable(TMR1, TRUE);

  /* tmr enable counter */
  tmr_counter_enable(TMR1, TRUE);

  tmr_output_enable(TMR1, TRUE);
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
